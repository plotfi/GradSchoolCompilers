#include "pin.H"
#include <string>
#include <string.h>
#include <map>
#include <sstream>
#include <stdio.h>
#include <stdint.h>
#include <iostream>

#include <time.h>

using namespace std;

KNOB<UINT64> KNOB_SAMPLE_RATE(KNOB_MODE_WRITEONCE, "pintool", "samplerate", "1000", "How often to sample load instructions.");
KNOB<UINT64> KNOB_SAMPLE_BURST(KNOB_MODE_WRITEONCE, "pintool", "sampleburst", "100", "How many instructions to sample per sample period.");

KNOB<string> KNOB_BENCH_NAME(KNOB_MODE_WRITEONCE, "pintool", "benchname", "foobar",  "Benchmark name, for CSV file.");

KNOB<bool> KNOB_ENABLE_SAMPLING(KNOB_MODE_WRITEONCE, "pintool", "enable_sampling", "false",  "Enable sample or not.");

KNOB<UINT64> KNOB_PREDICT_FOR_THREADS(KNOB_MODE_WRITEONCE, "pintool", "predict_for_threads", "0", "Number of threads to run the predictor for.");

KNOB<UINT64> KNOB_IGNORE_LOAD_COUNT(KNOB_MODE_WRITEONCE, "pintool", "ignore_loads", "0", "Number of loads to ignore.");


#include "LRUStack.H"

map<THREADID, LRUStack*> g_lrustacks;

THREADID g_rr_fake_tid = 0;

LRUStack g_main_lrustack;

#define TOTAL_DRAND48_NUMBERS 10000
double g_drand48_numbers[TOTAL_DRAND48_NUMBERS];

VOID MeasureMemoryAccess(uint64_t blockAddress, int64_t currentLogicalHibernateInterval, bool isHibernating, THREADID threadid)
{
    // round robin thread prediction
    if (KNOB_PREDICT_FOR_THREADS.Value())
    {
        //assert(false && "not ready");
        if (!g_rr_fake_tid)
        {
            g_rr_fake_tid = KNOB_PREDICT_FOR_THREADS.Value() - 1;
        }
        else
        {
            g_rr_fake_tid--;
        }

        threadid = g_rr_fake_tid;
    }

    LRUStack* lrustack = NULL;
    auto iter = g_lrustacks.find(threadid);

    if (iter == g_lrustacks.end())
    {
        lrustack = new LRUStack();
        lrustack->rehash(1000000);
        g_lrustacks.insert(pair<THREADID, LRUStack*>(threadid, lrustack));
    }
    else
    {
        lrustack = iter->second;
    }

    MemoryReference* memoryReference =
        lrustack->
        SearchAndCount(
            blockAddress,
            currentLogicalHibernateInterval,
            isHibernating);

    if (memoryReference) lrustack->Update(memoryReference);

    if (KNOB_PREDICT_FOR_THREADS.Value())
    {
        MemoryReference* memoryReference =
            g_main_lrustack.
            SearchAndCount(
                blockAddress,
                currentLogicalHibernateInterval,
                isHibernating);

        if (memoryReference) g_main_lrustack.Update(memoryReference);
    }
}

VOID RecordMemRead(VOID * ip, VOID * addr, UINT32 size, UINT32 lineNum, VOID* filename, THREADID threadid)
{
    // if (!strstr((char*)filename, "_ap")) return;

    for (unsigned i = 0; i < size; i++)
    {
        char* charAddr = (char*)addr;
        uint64_t byteAddr = ((uint64_t)(&charAddr[i]));

        if (KNOB_ENABLE_SAMPLING.Value())
        {
            static uint64_t count = 0;
            static double   burst = 1;
            static unsigned drand_index = 0;
            static uint64_t currentLogicalHibernateInterval = 0;

            count++;

            if (count < KNOB_SAMPLE_RATE.Value())
            {
                // Hibernate Interval
                MeasureMemoryAccess(byteAddr >> 6, currentLogicalHibernateInterval, true, threadid);
                return;
            }
            else if (count == KNOB_SAMPLE_RATE.Value())
            {
                currentLogicalHibernateInterval++;
            }


            burst++;

            // If Random Burst period is finished, reset counters.
            if ( burst > (g_drand48_numbers[drand_index] * ((double)KNOB_SAMPLE_BURST.Value())))
            {
                count = 1;
                burst = 0;
                drand_index = (drand_index + 1) % TOTAL_DRAND48_NUMBERS;


                MeasureMemoryAccess(byteAddr >> 6, currentLogicalHibernateInterval, true, threadid);
                return;
            }
        }

        MeasureMemoryAccess(byteAddr >> 6, -1, false, threadid);
    }
}

// Print a memory write record
VOID RecordMemWrite(VOID * ip, VOID * addr)
{
    //fprintf(trace,"%p: W %p\n", ip, addr);
}

// Is called for every instruction and instruments reads and writes
VOID Instruction(INS ins, VOID *v)
{
    if (INS_IsMemoryRead(ins))
    {
        INT32 lineNum;
        string filename;

        const ADDRINT iaddr = INS_Address(ins);
        PIN_LockClient();
        PIN_GetSourceLocation(iaddr, NULL, &lineNum, &filename);
        PIN_UnlockClient();

       // cout <<  "At " << filename << ":" << lineNum << " " <<(((uint64_t)iaddr) >> 6)  << endl;



        // Instruments memory accesses using a predicated call, i.e.
        // the instrumentation is called iff the instruction will actually be executed.
        //
        // The IA-64 architecture has explicitly predicated instructions. 
        // On the IA-32 and Intel(R) 64 architectures conditional moves and REP 
        // prefixed instructions appear as predicated instructions in Pin.
        UINT32 memOperands = INS_MemoryOperandCount(ins);

        // Iterate over each memory operand of the instruction.
        for (UINT32 memOp = 0; memOp < memOperands; memOp++)
        {
            if (INS_MemoryOperandIsRead(ins, memOp))
            {
                INS_InsertPredicatedCall(
                        ins, IPOINT_BEFORE, (AFUNPTR)RecordMemRead,
                        IARG_INST_PTR,
                        IARG_MEMORYOP_EA, memOp,
                        IARG_MEMORYREAD_SIZE,
                        IARG_UINT32, lineNum,
                        IARG_PTR, filename.c_str(), // BUG
                        IARG_THREAD_ID,
                        IARG_END);
            }
            // Note that in some architectures a single memory operand can be 
            // both read and written (for instance incl (%eax) on IA-32)
            // In that case we instrument it once for read and once for write.

            /*
               if (INS_MemoryOperandIsWritten(ins, memOp))
               {
               INS_InsertPredicatedCall(
               ins, IPOINT_BEFORE, (AFUNPTR)RecordMemWrite,
               IARG_INST_PTR,
               IARG_MEMORYOP_EA, memOp,
               IARG_END);
               }
               */
        }
    }
}

VOID Fini(INT32 code, VOID *v)
{
    for (auto iter = g_lrustacks.begin(); iter != g_lrustacks.end(); iter++)
    {
        THREADID     threadid = iter->first;
        LRUStack*    lrustack = iter->second;
        stringstream filename;

        string pred = "";

        if (KNOB_PREDICT_FOR_THREADS.Value())
        {
            pred = "_pred";
        }

        filename << "histogram_" << KNOB_BENCH_NAME.Value() << pred << "_" << threadid << ".csv";

        FILE* trace = fopen(filename.str().c_str(), "w");

        fprintf(trace, "%s", lrustack->DumpHistogram().c_str());
        fclose (trace);
    }

    if (KNOB_PREDICT_FOR_THREADS.Value())
    {
        stringstream filename;

        filename << "histogram_" << KNOB_BENCH_NAME.Value() << "_pred_main_" << ".csv";

        FILE* trace = fopen(filename.str().c_str(), "w");

        fprintf(trace, "%s", g_main_lrustack.DumpHistogram().c_str());
        fclose (trace);
    }
}

/* ===================================================================== */
/* Print Help Message                                                    */
/* ===================================================================== */
   
INT32 Usage()
{
    PIN_ERROR( "This Pintool prints a trace of memory addresses\n" 
              + KNOB_BASE::StringKnobSummary() + "\n");
    return -1;
}

/* ===================================================================== */
/* Main                                                                  */
/* ===================================================================== */

int main(int argc, char *argv[])
{
    for (unsigned i = 0; i < TOTAL_DRAND48_NUMBERS; i++)
        g_drand48_numbers[i] = drand48();

    PIN_InitSymbols();

    srand(time(NULL));

    cout << "Start program" << endl;

    if (PIN_Init(argc, argv)) return Usage();


    INS_AddInstrumentFunction(Instruction, 0);
    PIN_AddFiniFunction(Fini, 0);

    // Never returns
    PIN_StartProgram();
    
    return 0;
}
