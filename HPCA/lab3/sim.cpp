#include <list>
#include <math.h>
#include <stdlib.h>

#include "common.h"
#include "sim.h"
#include "trace.h"
#include "userknob.h"

#include <ctype.h>  /* Library for useful character operations */
#include <stdio.h>  /* standard input/output library           */
#include <string.h> /* String operations library               */

#define HIT   1
#define MISS  0
#define TRUE  1
#define FALSE 0

using namespace std;

bool run_a_cycle();
void init_structures();


bool g_clearbranch                [MAX_THREAD];

Op*    br_stall                   [MAX_THREAD];
UINT64 bp_miss_thread             [MAX_THREAD];
UINT64 bp_corr_predict_thread     [MAX_THREAD];
UINT64 dcache_hit_count_thread    [MAX_THREAD];
UINT64 dcache_miss_count_thread   [MAX_THREAD];
UINT64 retired_instruction_thread [MAX_THREAD];

UINT64 bp_miss           = 0;
UINT64 bp_corr_predict   = 0;
UINT64 dcache_miss_count = 0;

uint32_t free_op_num;
uint32_t active_op_num;

Op *op_pool;
Op *op_pool_free_head = NULL;

#define DUMPOPS (oplist, listname)                      \
cout << listname << "\n";                               \
for (list<Op*>::iterator                                \
iter  = oplist.begin();                                 \
iter != oplist.end();                                   \
iter ++) {                                              \
Op* op = *iter; cout                                    \
<< " instid: "           << op->inst_id                 \
<< " opcode:     "       << (int)op->opcode             \
<< " ptraddress: "       << op                          \
<< " inscheduler: "      << op->in_scheduler            \
<< " infu: "             << op->in_fu                   \
<< " isready: "          << IsOpReady(op)               \
<< " src_newname[0]: "   << op->src_newnames[0]         \
<< " src_newname[1]: "   << op->src_newnames[1]         \
<< " reg_map[0]: "       << reg_map[op->src[0]].inst_id \
<< " reg_map[1]: "       << reg_map[op->src[1]].inst_id \
<< " src_rdy_cycles[0] " << op->src_rdy_cycles[0]       \
<< " src_rdy_cycles[1] " << op->src_rdy_cycles[1]       \
<< " dst: "              << (int)op->dst                \
<< " src1: "             << (int)op->src[0]             \
<< " src2: "             << (int)op->src[1]             \
<< " cycle:      "       << cycle_count                 \
<< " done cycle: "       << op->done_cycle              \
<< " \n"; }                                             \
cout << endl;

bool icache_access(uint32_t addr);
bool dcache_access(uint32_t addr, Op* op);
void init_latches(void);
void init_register_file();

// cache entry structure
typedef struct Cache_Entry_Struct
{
    uint32_t tag;   /* address tag    */
    uint32_t lru;   /* replace policy */
    uint32_t valid; /* valid bit      */
} Cache_Entry;

typedef struct Cache_Struct
{
    Cache_Entry** cache_entry;
    uint32_t      cache_size;
    uint32_t      block_size;
    uint32_t      entry_size;
    uint32_t      assoc;
    char          name[128];
} Cache;

void     cache_init();
bool     cache_access(uint32_t addr, Op* op);
uint32_t cache_read  (uint32_t addr);

void print_results();


#define LOG2(x) ( (x == 1 ) ? 0  : \
    ((x == (0x1 << 1 )) ? 1  :     \
    ((x == (0x1 << 2 )) ? 2  :     \
    ((x == (0x1 << 3 )) ? 3  :     \
    ((x == (0x1 << 4 )) ? 4  :     \
    ((x == (0x1 << 5 )) ? 5  :     \
    ((x == (0x1 << 6 )) ? 6  :     \
    ((x == (0x1 << 7 )) ? 7  :     \
    ((x == (0x1 << 8 )) ? 8  :     \
    ((x == (0x1 << 9 )) ? 9  :     \
    ((x == (0x1 << 10)) ? 10 :     \
    ((x == (0x1 << 11)) ? 11 :     \
    ((x == (0x1 << 12)) ? 12 :     \
    ((x == (0x1 << 13)) ? 13 :     \
    ((x == (0x1 << 14)) ? 14 :     \
    ((x == (0x1 << 15)) ? 15 :     \
    ((x == (0x1 << 16)) ? 16 :     \
    ((x == (0x1 << 17)) ? 17 :     \
    ((x == (0x1 << 18)) ? 18 :     \
    ((x == (0x1 << 19)) ? 19 :     \
    ((x == (0x1 << 20)) ? 20 :     \
    ((x == (0x1 << 21)) ? 21 :     \
    ((x == (0x1 << 22)) ? 22 :     \
    ((x == (0x1 << 23)) ? 23 :     \
    ((x == (0x1 << 24)) ? 24 :     \
    ((x == (0x1 << 25)) ? 25 :     \
    ((x == (0x1 << 26)) ? 26 :     \
    ((x == (0x1 << 27)) ? 27 :     \
    ((x == (0x1 << 28)) ? 28 :     \
    ((x == (0x1 << 29)) ? 29 :     \
    ((x == (0x1 << 30)) ? 30 :     \
    ((x == (0x1 << 31)) ? 31 : 1))))))))))))))))))))))))))))))))


// #define PRINT_DEBUG 1
#define MAX_UNIQUE_MEM_ADDR 14000
#define SIM_NUM             1000000

/* local variables   */

// initial parameter
static uint64_t cycle_time;
static uint32_t assoc;
static uint32_t cache_size;
static uint32_t block_size;
static uint32_t cache_latency;
static uint32_t memory_latency;

// cache for data
Cache *data_cache;

void simulator_main(void)
{
    init_structures();
    run_a_cycle();
}

int op_latency[NUM_OP_TYPE];

void init_op_latency()
{
    op_latency[OP_INV]   = 1;
    op_latency[OP_NOP]   = 1;
    op_latency[OP_CF]    = 1;
    op_latency[OP_CMOV]  = 1;
    op_latency[OP_LDA]   = 1;
    op_latency[OP_LD]    = 1;
    op_latency[OP_ST]    = 1;
    op_latency[OP_IADD]  = 1;
    op_latency[OP_IMUL]  = 2;
    op_latency[OP_IDIV]  = 4;
    op_latency[OP_ICMP]  = 2;
    op_latency[OP_LOGIC] = 1;
    op_latency[OP_SHIFT] = 2;
    op_latency[OP_BYTE]  = 1;
    op_latency[OP_MM]    = 2;
    op_latency[OP_FMEM]  = 2;
    op_latency[OP_FCF]   = 1;
    op_latency[OP_FCVT]  = 4;
    op_latency[OP_FADD]  = 2;
    op_latency[OP_FMUL]  = 4;
    op_latency[OP_FDIV]  = 16;
    op_latency[OP_FCMP]  = 2;
    op_latency[OP_FBIT]  = 2;
    op_latency[OP_FCMP]  = 2;
}

void init_op(Op *op)
{
    op->num_src          =  0;
    op->src[0]           = -1;
    op->src[1]           = -1;
    op->dst              = -1;
    op->opcode           =  0;
    op->write_flag       =  0;
    op->inst_size        =  0;
    op->ld_vaddr         =  0;
    op->st_vaddr         =  0;
    op->instruction_addr =  0;
    op->branch_target    =  0;
    op->actually_taken   =  0;
    op->mem_read_size    =  0;
    op->mem_write_size   =  0;
    op->is_fp            =  false;
    op->valid            =  FALSE;
    op->started_exec     =  FALSE;
    op->cf_type          =  NOT_CF;
    op->mem_type         =  NOT_MEM;

    // TODO: init new fields here.

    op->in_scheduler      = FALSE;
    op->in_fu             = FALSE;
    op->src_newnames[0]   = 0;
    op->src_rdy_cycles[0] = 0;
    op->src_newnames[1]   = 0;
    op->src_rdy_cycles[1] = 0;
    op->done = false;

    op->thread_id         = 0;
}

void init_op_pool(void)
{
    op_pool       = new Op [1024];
    free_op_num   = 1024;
    active_op_num = 0;

    uint32_t op_pool_entries = 0;

    int ii;
    for (ii = 0; ii < 1023; ii++)
    {
        op_pool[ii].op_pool_next = &op_pool[ii+1];
        op_pool[ii].op_pool_id   = op_pool_entries++;
        init_op(&op_pool[ii]);
    }

    op_pool[ii].op_pool_next = op_pool_free_head;
    op_pool[ii].op_pool_id   = op_pool_entries++;
    init_op(&op_pool[ii]);
    op_pool_free_head = &op_pool[0];
}

Op *get_free_op(void)
{
    if (op_pool_free_head == NULL || (free_op_num == 1))
    {
        cout << "ERROR! OP_POOL SIZE is too small!! " << endl;
        cout << "please check free_op function "      << endl;
        assert(1);
        exit(1);
    }

    free_op_num--;

    assert(free_op_num);

    Op *new_op        = op_pool_free_head;
    op_pool_free_head = new_op->op_pool_next;

    assert(!new_op->valid);

    init_op(new_op);
    active_op_num++;

    return new_op;
}

void free_op(Op *op)
{
    free_op_num++;
    active_op_num--;

    op->valid         = FALSE;
    op->op_pool_next  = op_pool_free_head;
    op_pool_free_head = op;
}

struct map_entry
{
    bool     valid;
    uint64_t inst_id;
    uint64_t rdy_cycle;
};

// #define DEBUGGHR
#ifdef DEBUGGHR
#include <sstream>
string DumpBinary(unsigned integer)
{
    string retval = "";
    unsigned mask = 1;

    while (mask)
    {
        stringstream sstr;
        sstr << ((integer & mask) ? 1 : 0);
        retval = sstr.str() + retval;
        mask <<= 1;
    }

    return retval;
}
#endif

class BranchPredictor
{
    public:

        BranchPredictor(unsigned ghrlength, unsigned smtwidth)
        {
            for (unsigned i = 0; i < smtwidth; i++)
            {
                m_ghr.resize(i+1, 0);
            }

            m_ghrlength = ghrlength;
            m_ghtlength = (long)pow(2, ghrlength);
            m_ght       = new TwoBitCounter[m_ghtlength + 1];

            // <XXX>
            m_ghrmask = 1;

            for (unsigned i  = 1; i < m_ghrlength; i++)
                m_ghrmask = (m_ghrmask << 1) | m_ghrmask;

            //</XXX>

            for (unsigned i  = 0; i < m_ghtlength; i++)
                m_ght[i].tbc = 2;

            if (KNOB_DEBUG_PRINT.Value())
            {
                cout << "\nm_ghrlength: " << m_ghrlength
                    << "\nm_ghtlength: " << m_ghtlength
                    << endl;
            }
        }

        virtual ~BranchPredictor()
        {
            #ifdef DEBUGGHR
            if (KNOB_DEBUG_PRINT.Value())
            {
                for (unsigned i = 0; i < m_ghtlength; i++)
                    cout << "2bc: " << m_ght[i].tbc << endl;
            }
            #endif

            delete [] m_ght;
        }

        bool PredictAndUpdate(Op* op, bool actual, int thread_id) const
        {
            unsigned index  = GetIndex(op, thread_id);
            bool prediction = PredictTaken(index);
            UpdateGHT(index, actual, thread_id);
            return prediction;
        }

    private:

        struct TwoBitCounter { unsigned tbc : 2; };

        unsigned GetIndex(Op* op, int thread_id) const
        {
            unsigned index = (op->instruction_addr ^ m_ghr[thread_id]) & m_ghrmask;

            #ifdef DEBUGGHR
            cout << "PC:    " << DumpBinary(op->instruction_addr)<< endl;
            cout << "thread:" << thread_id << endl;
            cout << "ghr:   " << DumpBinary(m_ghr[thread_id]) << endl;
            cout << "mask:  " << DumpBinary(m_ghrmask) << endl;
            cout << "index: " << DumpBinary(index) << endl;
            cout << "index: " << index << endl;
            cout << "\n\n";
            #endif
            assert(0     <= index);
            assert(index <= m_ghtlength);
            return index;
        }

        bool PredictTaken(unsigned index) const
        {
            return m_ght[index].tbc > 1;
        }

        void Increment2BC(unsigned index) const
        {
            if (m_ght[index].tbc < 3) m_ght[index].tbc++;
        }

        void Decrement2BC(unsigned index) const
        {
            if (m_ght[index].tbc > 0) m_ght[index].tbc--;
        }

        void UpdateGHT(unsigned index, bool actual, int thread_id) const
        {
            if (actual) Increment2BC(index);
            else        Decrement2BC(index);
            m_ghr[thread_id] = (m_ghr[thread_id] << 1) | (actual ? 1 : 0);
        }

        mutable TwoBitCounter*   m_ght;
        mutable vector<unsigned> m_ghr;

        unsigned m_ghrmask;
        unsigned m_ghtlength;
        unsigned m_ghrlength;
};

struct REG_element
{
    bool valid;
};

struct MSHREntry
{
    UINT64    m_ready_cycle;
    list<Op*> m_waiting_inst_ids;
};

typedef map<uint32_t, MSHREntry> mshr_t;


// Hardware Structures:
static list<Op*>        g_rob;
static mshr_t           g_MSHR;
static list<Op*>        FE_latch;
static list<Op*>        g_scheduler;
static Cache*           g_cache = NULL;
static list<Op*>        g_functional_units;
static REG_element      register_file[NUM_REG];
static BranchPredictor* g_branchPredictor = NULL;
static map_entry        reg_map[MAX_THREAD][NUM_REG];

// Pipeline Stages:
void FE_stage();
void ID_stage();
void EX_stage();
void WB_stage();
void ISSUE_stage();
void RetireFromMSHR();

bool IsOpReady(Op* op);

// Stupid end condition flags:
static bool g_sim_end_precondition = false;
static bool sim_end_condition      = FALSE;

// Performance Counters:
UINT64 cycle_count          = 0;
UINT64 data_hazard_count    = 0;
UINT64 icache_miss_count    = 0;
UINT64 retired_instruction  = 0;
UINT64 l2_cache_miss_count  = 0;
UINT64 control_hazard_count = 0;

UINT64 next_pc;
UINT64 ld_st_buffer[LD_ST_BUFFER_SIZE];

void print_stats() {
    std::ofstream out(KNOB_OUTPUT_FILE.Value().c_str());
    /* Do not modify this function. This messages will be used for grading */
    out << "Total instruction: " << retired_instruction << endl;
    out << "Total cycles: " << cycle_count << endl;
    float ipc = (cycle_count ? ((float)retired_instruction/(float)cycle_count): 0 );
    out << "IPC: " << ipc << endl;
    out << "Total I-cache miss: " << icache_miss_count << endl;
    out << "Total D-cache miss: " << dcache_miss_count << endl;
    out << "Total L2-cache miss: " << l2_cache_miss_count << endl;
    out << "Total data hazard: " << data_hazard_count << endl;
    out << "Total BR_mispred: " << bp_miss << endl;
    out << "Total BR_corrpred: " << bp_corr_predict << endl;
    out << "Total control hazard : " << control_hazard_count << endl;

    out << "\n\n\n" << endl;
    for ( int ii = 0;ii < KNOB_MAX_THREAD_NUM.Value();ii++) {
        float thread_ipc = (cycle_count ? ((float)retired_instruction_thread[ii]/(float)cycle_count): 0 );
        out << "THREAD_IPC : " << thread_ipc << " Thread id: " << ii << endl;
        out << "THREAD_D-cache miss: " << dcache_miss_count_thread[ii] << " Thread id: " << ii << endl;
        out << "THREAD_D-cache hit:  " << dcache_hit_count_thread[ii] << " Thread id: " << ii << endl;
        out << "THREAD_BR_mispred: " << bp_miss_thread[ii] << " Thread id: " << ii << endl;
        out << "THREAD_BR_corrpred: " << bp_corr_predict_thread[ii] << " Thread id: " << ii << endl;

    }
    out.close();
}


int get_op(Op *op)
{
    static UINT64 unique_count = 0;
    static UINT64 fetch_arbiter;

    Trace_op trace_op;
    bool success = FALSE;
    int fetch_id = -1;
    bool br_stall_fail = false;
    // read trace
    // fill out op info
    // return FALSE if the end of trace
    for (int jj =0;jj < KNOB_MAX_THREAD_NUM.Value();jj++)
    {
        fetch_id = (fetch_arbiter++%KNOB_MAX_THREAD_NUM.Value());


        if (br_stall[fetch_id])
        {
            br_stall_fail = true;
            continue;
        }

        success = (gzread(stream[fetch_id], &trace_op, sizeof(Trace_op)) > 0 );

        /* copy trace structure to op */
        if (success)
        {
            if (KNOB_PRINT_INST.Value()) dprint_trace(&trace_op);

            if ((KNOB_MAX_THREAD_NUM.Value()> 1)  && (KNOB_PRINT_INST.Value() >0 ))
            {
                std::cout <<"thread_id: " << fetch_id << endl;
            }

            copy_trace_op(&trace_op, op);

            op->inst_id  = unique_count++;
            op->valid    = TRUE;
            op->thread_id = fetch_id;
            return success;// get op so return
        }

        // if not success and go to another trace.
    }
    if (br_stall_fail) return -1;
    return success;
}

int get_op_latency(Op *op)
{
    assert (op->opcode < NUM_OP_TYPE);
    return op_latency[op->opcode];
}

void dump_reg()
{
    for (int ii = 0; ii < NUM_REG; ii++)
        cout << cycle_count << ":register[" << ii << "]: V:" << register_file[ii].valid << endl;
}

void print_pipeline()
{
  cout << "--------------------------------------------" << endl;

  cout << "cycle count : " << dec << cycle_count
       << " retired_instruction : " << retired_instruction << endl;

  cout << (int)cycle_count << " FE: " ;

  cout << endl;

  dump_reg();

  cout << "--------------------------------------------" << endl;
}

void print_heartbeat()
{
    static uint64_t last_cycle ;
    static uint64_t last_inst_count[MAX_THREAD];

    for (int ii = 0;ii < KNOB_MAX_THREAD_NUM.Value();ii++) {
        float temp_ipc = float(retired_instruction_thread[ii] - last_inst_count[ii]) /(float)(cycle_count-last_cycle) ;
        float ipc = float(retired_instruction) /(float)(cycle_count) ;
        /* Do not modify this function. This messages will be used for grading */
        cout <<"**Heartbeat** cycle_count: " << cycle_count << " inst:" << retired_instruction_thread[ii]
             << " IPC: " << temp_ipc << " Overall IPC: " << ipc << " thread_id " << ii << endl;

        last_inst_count[ii] = retired_instruction_thread[ii];
    }
    last_cycle = cycle_count;
}

bool IsOpReady(Op* op);

bool run_a_cycle()
{
    while (true)
    {
        if (((KNOB_MAX_SIM_COUNT.Value()     &&
              (cycle_count                   >=
               KNOB_MAX_SIM_COUNT.Value()))  ||
             (KNOB_MAX_INST_COUNT.Value()    &&
              (retired_instruction           >=
               KNOB_MAX_INST_COUNT.Value())) ||
               (sim_end_condition)))
        {
            print_heartbeat();
            print_stats();

            delete g_branchPredictor;

            return TRUE;
        }

        cycle_count++;

        if (!(cycle_count%5000)) print_heartbeat();

        RetireFromMSHR();
        WB_stage();
        EX_stage();
        ISSUE_stage();
        ID_stage();
        FE_stage();

        if (KNOB_PRINT_PIPE_FREQ.Value() &&
            !(cycle_count%KNOB_PRINT_PIPE_FREQ.Value()))
        {
            print_pipeline();
        }
    }

    return TRUE;
}


void init_structures(void)
{
    init_op_pool();
    init_op_latency();
    init_latches();
    init_register_file();

    cycle_time = 0;
    cache_init();

    for (unsigned i = 0; i < MAX_THREAD; i++)
    {
        br_stall[i]      = NULL;
        g_clearbranch[i] = false;
    }
}

void broad_cast(Op *op)
{
    assert(NULL != op);

    if (0 <= op->dst && op->dst < NUM_REG)
    if(reg_map[op->thread_id][op->dst].inst_id == op->inst_id)
    {
        reg_map[op->thread_id][op->dst].rdy_cycle = op->done_cycle;
        reg_map[op->thread_id][op->dst].valid     = TRUE;
    }

    // traverse the scheduler and check all the source ids
    // if source ids are the same as op's inst_id
    // then set op->src_rdy_cycles to the current rdy cycles
    for (list<Op*>::iterator
         iter  = g_scheduler.begin();
         iter != g_scheduler.end();
         iter++)
    {
        Op* sched_op = *iter;

        for (unsigned i = 0; i < 2; i++)
            if (0 <= op->src[i] && op->src[i] < NUM_REG)
            if (sched_op->src_newnames[i]  == op->inst_id)
                sched_op->src_rdy_cycles[i] = op->done_cycle;
    }

}

void RetireFromMSHR()
{
    list<uint32_t> removeFromMSHR;

    for (map<uint32_t, MSHREntry>::iterator mshr_iter = g_MSHR.begin();
         mshr_iter != g_MSHR.end();
         mshr_iter++)
    {
        if (mshr_iter->second.m_ready_cycle <= cycle_count)
        {
            removeFromMSHR.push_back(mshr_iter->first);

            for (list<Op*>::iterator op_iter =
                    mshr_iter->second.m_waiting_inst_ids.begin();
                 op_iter != mshr_iter->second.m_waiting_inst_ids.end();
                 op_iter++)
            {
                Op* op = *op_iter;
                op->done = true;
                broad_cast(op);
            }
        }
    }

    for (list<uint32_t>::iterator iter =
            removeFromMSHR.begin();
         iter != removeFromMSHR.end();
         iter++)
    {
        g_MSHR.erase(*iter);

        uint32_t lru_index  = 0;
        uint32_t cache_line = *iter;
        uint32_t line_num   = cache_line % g_cache->entry_size;
        uint32_t tag        = cache_line / g_cache->entry_size;

        /*
        for(uint32_t ii = 0;ii < g_cache->assoc;ii++)
        {
            // tag match
            if(g_cache->cache_entry[line_num][ii].tag == tag)
            {
                // update lru time
                g_cache->cache_entry[line_num][ii].lru   = cycle_time;
                g_cache->cache_entry[line_num][ii].valid = TRUE;
                replace = false;
                break;
            }
        }
        */

        // Searching for oldest  cache data PERFECT LRU method.
        for (uint32_t ii = 0;ii < g_cache->assoc;ii++)
        {
            if(g_cache->cache_entry[line_num][ii].lru <
                    g_cache->cache_entry[line_num][lru_index].lru)
            {
                lru_index = ii;
            }
        }

        /* cache replacement */
        g_cache->cache_entry[line_num][lru_index].tag   = tag;
        g_cache->cache_entry[line_num][lru_index].lru   = cycle_time;
        g_cache->cache_entry[line_num][lru_index].valid = TRUE;

        #if PRINT_DEBUG
        printf("%d: replace  %d\n ",cycle_time, lru_index);
        #endif
    }
}

void RequestFromMSHR(uint32_t addr, Op* op)
{
    uint32_t block_addr = (uint32_t)(addr >> 6); 
    //(LOG2(g_cache->block_size)));

    map<uint32_t, MSHREntry>::iterator iter = g_MSHR.find(block_addr);

    if (iter == g_MSHR.end())
    {
        MSHREntry entry;
        entry.m_ready_cycle = cycle_count + KNOB_MEM_LATENCY.Value() + 1;
        entry.m_waiting_inst_ids.push_back(op);

        g_MSHR.insert(pair<uint32_t, MSHREntry>(block_addr, entry));
    }
    else
    {
        iter->second.m_waiting_inst_ids.push_back(op);
    }
}

bool IsOpReady(Op* op)
{
    assert(NULL != op);

    bool isOpReady = true;

    for (unsigned i = 0; i < 2; i++)
    {
        if (0 <= op->src[i] && op->src[i] < NUM_REG)
        {
            if (op->src_newnames[i] == reg_map[op->thread_id][op->src[i]].inst_id)
            {
                isOpReady =
                    (op->src_rdy_cycles[i] <= cycle_count) &&
                    reg_map[op->thread_id][op->src[i]].valid &&
                    isOpReady;
            }
        }
    }

    return isOpReady;
}

bool thread_retire_stop[MAX_THREAD];

void WB_stage()
{
    sim_end_condition =
        !g_rob.size()       &&
        !FE_latch.size()    &&
        !g_scheduler.size() &&
        g_sim_end_precondition;

    unsigned commit_count = 0;

    for (unsigned i = 0; i < MAX_THREAD; i++)
    {
        thread_retire_stop[i] = false;

        if (g_clearbranch[i])
        {
            br_stall[i]      = NULL;
            g_clearbranch[i] = false;
        }
    }

    list<Op*> toRemove;

    for (list<Op*>::iterator iter = g_rob.begin();
            commit_count <=
            KNOB_ISSUE_WIDTH.Value() &&
            iter != g_rob.end();
            iter++)
    {
        Op* op = *iter;

        assert(NULL != op);

        if (!op->done)
        {
            thread_retire_stop[op->thread_id] = true;

            bool is_all_threads_retire_stop = true;

            for (int i = 0; i < KNOB_MAX_THREAD_NUM.Value(); i++)
            {
                is_all_threads_retire_stop =
                    thread_retire_stop[i] &&
                    is_all_threads_retire_stop;
            }

            if (is_all_threads_retire_stop) break;

            continue;
        }

        retired_instruction++;
        retired_instruction_thread[op->thread_id]++;
        toRemove.push_back(op);
        commit_count++;

        if (KNOB_DEBUG_PRINT.Value())
        {
            cout << "WB_STAGE OP " << op->inst_id <<  " is retired at cycle "
                << cycle_count << " thread_id " << op->thread_id << endl;
        }

    }

    for (list<Op*>::iterator iter = toRemove.begin();
            iter != toRemove.end();
            iter++)
    {
        Op* op = *iter;
        free_op(op);
        g_rob.remove(op);
    }
}

void EX_stage()
{
    list<Op*> removeFrom;

    for (list<Op*>::iterator
         iter  = g_functional_units.begin();
         iter != g_functional_units.end();
         iter++)
    {
        Op* op = *iter;
        g_scheduler.remove(*iter);

        if (op->cf_type == CF_CBR && op == br_stall[op->thread_id])
            g_clearbranch[op->thread_id] = true;

        if(op->in_fu && op->done_cycle <= cycle_count)
        {
            removeFrom.push_back(op);
            op->done = true;
            broad_cast(op);
        }
    }

    for (list<Op*>::iterator
         iter  = removeFrom.begin();
         iter != removeFrom.end();
         iter++)
    {
        g_functional_units.remove(*iter);
    }
}

void ISSUE_stage()
{
    unsigned issue_count = 0;

    for (list<Op*>::iterator
         iter  = g_rob.begin();
         iter != g_rob.end();
         iter++)
    {
        if (g_scheduler.size() >= KNOB_SCHED_SIZE.Value())  break;
        if (issue_count        >= KNOB_ISSUE_WIDTH.Value()) break;

        Op* op = *iter;

        if (!op->in_scheduler &&
            (!op->mem_type || KNOB_PERFECT_DCACHE.Value()))
        {
            g_scheduler.push_back(op);
            op->in_scheduler = true;
            ++issue_count;
        }
        else if (!op->in_scheduler && op->mem_type)
        {
            uint32_t addr = op->mem_type == MEM_LD ? op->ld_vaddr : op->st_vaddr;

            if (!dcache_access(addr, op))
            {
                RequestFromMSHR(addr, op);
                op->in_scheduler = true;
            }
            else
            {
                g_scheduler.push_back(op);
                op->in_scheduler = true;
            }
            ++issue_count;
        }
    }

    unsigned ready_count = 0;

    for (list<Op*>::iterator
         iter  = g_scheduler.begin();
         iter != g_scheduler.end();
         iter++)
    {
        if (ready_count >= KNOB_ISSUE_WIDTH.Value()) break;

        Op* op = *iter;
        bool isOpReady = IsOpReady(op);

        if (op->in_scheduler && !op->in_fu && isOpReady)
        {
            if (KNOB_DEBUG_PRINT.Value())
                cout << " ID_STAGE OP " << op->inst_id
                     << " sched cycle " << cycle_count << endl;

            op->in_fu      = true;
            op->done_cycle = (!op->mem_type || KNOB_PERFECT_DCACHE.Value()) ?
                             cycle_count + get_op_latency(op)          + 1  :
                             cycle_count + KNOB_DCACHE_LATENCY.Value() + 1  ;

            g_functional_units.push_back(op);
            ++ready_count;
        }

        if (!isOpReady && !KNOB_OOO_SCHEDULER.Value()) break;
    }
}

void ID_stage()
{
    for (unsigned i = 0; i < KNOB_ISSUE_WIDTH.Value(); i++)
    {
        if (g_rob.size()    >= KNOB_ROB_SIZE.Value()) return;
        if (FE_latch.size() == 0) return;

        Op* op = FE_latch.front();
        FE_latch.pop_front();

        if (KNOB_DEBUG_PRINT.Value())
        {
            cout << "ID_STAGE OP " << op->inst_id <<  " is scheduled at cycle "
                << cycle_count <<  " thread_id " << op->thread_id <<  endl;
        }

        /// Rename src registers.
        for (int i = 0 ; i < 2; i++)
        {
            if (0 <= op->src[i] && op->src[i] < NUM_REG)
            {
                op->src_newnames[i]   = reg_map[op->thread_id][op->src[i]].inst_id;
                op->src_rdy_cycles[i] = reg_map[op->thread_id][op->src[i]].rdy_cycle;
            }
        }

        /// Rename dst registers.
        if (0 <= op->dst && op->dst < NUM_REG)
        {
            reg_map[op->thread_id][op->dst].inst_id   = op->inst_id;
            reg_map[op->thread_id][op->dst].valid     = FALSE;
            reg_map[op->thread_id][op->dst].rdy_cycle = 0;
        }

        g_rob.push_back(op);
    }
}

/* Keep FE_Stage */
void FE_stage()
{
    if (  sim_end_condition)    return;
    if (g_sim_end_precondition) return;

    while (FE_latch.size() < KNOB_ISSUE_WIDTH.Value())
    {
        Op* op = get_free_op();

        int result = get_op(op);

        if (0 >= result)
        {
            free_op(op);

            if (result != -1)
                g_sim_end_precondition = true;

            return;
        }

        if (op->cf_type == CF_CBR)
        {
            if (op->actually_taken != g_branchPredictor->PredictAndUpdate(op, op->actually_taken, op->thread_id))
            {
                br_stall[op->thread_id] = op;
                bp_miss_thread[op->thread_id]++;
                bp_miss++;
            }
            else
            {
                br_stall[op->thread_id] = NULL;
                bp_corr_predict_thread[op->thread_id]++;
                bp_corr_predict++;
            }
        }

        FE_latch.push_back(op);
    }
}

void init_register_file()
{
    for (int i = 0; i < NUM_REG; i++)
    {
        register_file[i].valid = true;
    }
}

void  init_latches()
{
    for (unsigned j = 0; j < MAX_THREAD; j++)
    {
        for (unsigned i = 0; i < NUM_REG; i++)
        {
            reg_map[j][i].valid     = TRUE;
            reg_map[j][i].inst_id   = 0;
            reg_map[j][i].rdy_cycle = 0;
        }
    }

    g_branchPredictor = new BranchPredictor(KNOB_GHR_LENGTH.Value(), KNOB_MAX_THREAD_NUM.Value());
}

bool icache_access(uint32_t addr)
{
    bool hit = FALSE;
    if (KNOB_PERFECT_ICACHE.Value()) hit = TRUE;
    return hit;
}

bool dcache_access(uint32_t addr, Op* op)
{
    if (KNOB_PERFECT_DCACHE.Value()) return TRUE;

    return cache_access(addr, op);
}


/////////////////////////////////////////////////////////////
// Cache Functions                                         //
/////////////////////////////////////////////////////////////

/* initialize the cache */
void cache_init()
{
    uint32_t ii;
    uint32_t jj;

    g_cache           = (Cache *)malloc(sizeof(Cache));
    Cache* cache      = g_cache;
    memory_latency    = KNOB_MEM_LATENCY.Value();
    cache_latency     = KNOB_DCACHE_LATENCY.Value();
    cache_size        = KNOB_DCACHE_SIZE.Value() * 1024;
    block_size        = 64;
    assoc             = KNOB_DCACHE_WAY.Value();

    cache->cache_size = cache_size;
    cache->block_size = block_size;
    cache->assoc      = assoc;
    cache->entry_size = cache_size/(block_size)/assoc;
    /* number of cache entry */

    strcpy(cache->name, "data_cache");

    /* cache entry dynmaic memory allocation */
    cache->cache_entry = (Cache_Entry**) malloc(sizeof(Cache_Entry *)*(cache_size/block_size));

    for( ii = 0; ii < cache->entry_size; ii++)
    {
        /* create 2dimensional array size */
        cache->cache_entry[ii] = (Cache_Entry *) malloc(sizeof(Cache_Entry) * assoc);

        for(jj = 0 ; jj < assoc; jj++)
        {
            cache->cache_entry[ii][jj].valid = FALSE;
            cache->cache_entry[ii][jj].lru = 0;
        }
    }

    #if PRINT_DEBUG
    printf("cache_init:%s is done, cache_entry size is %d\n", cache->name, cache->entry_size);
    #endif
}

bool cache_access(uint32_t addr, Op* op)
{
    int32_t cache_hit;

    cycle_time++;
    cache_hit = cache_read(addr);

    if(cache_hit) dcache_hit_count_thread [op->thread_id]++;
    else
    {
        dcache_miss_count++;
        dcache_miss_count_thread[op->thread_id]++;
    }

    return cache_hit;
}

/* cache read and update lru time and if cache miss then cache insert */

uint32_t cache_read(uint32_t addr)
{
    Cache* cache   = g_cache;
    uint32_t ii;
    uint32_t cache_line = addr >> 6;
        //(LOG2(cache->block_size));    /* cache line address */
    uint32_t line_num   = cache_line % cache->entry_size;                /* cache index        */
    uint32_t tag        = cache_line / cache->entry_size;                /* cache tag          */
    uint32_t cache_cold = FALSE;

    #if PRINT_DEBUG
    printf("[%s] addr:%5u cache_line:%5d line_num:%5d tag:%5d log2_block:%3d ",
       cache->name, addr, cache_line, line_num, tag , LOG2(cache->block_size));
    #endif

    /* set associative */
    for(ii = 0; ii < cache->assoc; ii++)
    {
        if (cache->cache_entry[line_num][ii].tag == tag )
        {   // tag match

            if (cache->cache_entry[line_num][ii].valid == FALSE)
            {  // data should be valid
                cache_cold = TRUE;
            }

            if(!cache_cold)
            {
                return HIT;
            }
            else
            {
                return MISS;
                // cache block was empty
                // so we don't need to
                // evict cache block
            }
        }
    }

   return MISS;
   // We evict in MSHR code.
}

