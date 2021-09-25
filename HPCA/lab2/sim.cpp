#include <list>
#include <math.h>
#include <stdlib.h>

#include "common.h"
#include "sim.h"
#include "trace.h"
#include "userknob.h"

using namespace std;

bool run_a_cycle();
void init_structures();

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
bool dcache_access(uint32_t addr);
void  init_latches(void);
void init_register_file();

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

    //TODO: init new fields here.

    op->in_scheduler      = FALSE;
    op->in_fu             = FALSE;
    op->src_newnames[0]   = 0;
    op->src_rdy_cycles[0] = 0;
    op->src_newnames[1]   = 0;
    op->src_rdy_cycles[1] = 0;
    op->done = false;
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

#ifdef DEBUGGHR
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

        BranchPredictor(unsigned ghrlength)
        {
            m_ghr       = 0;
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

        bool PredictAndUpdate(Op* op, bool actual) const
        {
            unsigned index  = GetIndex(op);


            bool prediction = PredictTaken(index);

            //if (!prediction && actual)

            UpdateGHT(index, actual);

            return prediction;
        }

    private:

        struct TwoBitCounter { unsigned tbc : 2; };

        unsigned GetIndex(Op* op) const
        {

            unsigned index = (op->instruction_addr ^ m_ghr) & m_ghrmask;
            //cout << "PC:    " << DumpBinary(op->instruction_addr)<< endl;
            //cout << "ghr:   " << DumpBinary(m_ghr) << endl;
            //cout << "mask:  " << DumpBinary(m_ghrmask) << endl;
            //cout << "index: " << DumpBinary(index) << endl;
            //cout << "index: " << index << endl;
            //cout << "\n\n";
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

        void UpdateGHT(unsigned index, bool actual) const
        {
            if (actual) Increment2BC(index);
            else        Decrement2BC(index);
            m_ghr = (m_ghr << 1) | (actual ? 1 : 0);
        }

        mutable TwoBitCounter* m_ght;
        mutable unsigned       m_ghr;


        unsigned m_ghrmask;
        unsigned m_ghtlength;
        unsigned m_ghrlength;
};

struct REG_element { bool valid; };

static bool             g_sim_end_precondition = false;
static Op*              g_mispredicted_branch  = NULL;
static BranchPredictor* g_branchPredictor      = NULL;

map_entry reg_map[NUM_REG];

list<Op*> g_rob;
list<Op*> g_scheduler;

REG_element register_file[NUM_REG];

void FE_stage();
void ID_stage();
void ISSUE_stage();
void EX_stage();
void WB_stage();

bool sim_end_condition      = FALSE;

UINT64 cycle_count          = 0;
UINT64 data_hazard_count    = 0;
UINT64 icache_miss_count    = 0;
UINT64 dcache_miss_count    = 0;
UINT64 retired_instruction  = 0;
UINT64 l2_cache_miss_count  = 0;
UINT64 control_hazard_count = 0;

UINT64 bp_miss;
UINT64 bp_corr_predict;

UINT64 next_pc;
UINT64 ld_st_buffer[LD_ST_BUFFER_SIZE];

list<Op*> FE_latch;


void print_stats()
{
    ofstream out(KNOB_OUTPUT_FILE.Value().c_str());

    float ipc = cycle_count ? ((float)retired_instruction/(float)cycle_count): 0;

    out << "Total instruction:     " << retired_instruction  << endl;
    out << "Total cycles:          " << cycle_count          << endl;
    out << "IPC:                   " << ipc                  << endl;
    out << "Total I-cache miss:    " << icache_miss_count    << endl;
    out << "Total D-cache miss:    " << dcache_miss_count    << endl;
    out << "Total L2-cache miss:   " << l2_cache_miss_count  << endl;
    out << "Total data hazard:     " << data_hazard_count    << endl;
    out << "Total BR_mispred:      " << bp_miss              << endl;
    out << "Total BR_corrpred:     " << bp_corr_predict      << endl;
    out << "BP correctness %:      " << 100 * ((double)bp_corr_predict) /((double) (bp_miss + bp_corr_predict)) << endl;
    out << "Total control hazard : " << control_hazard_count << endl;
    out.close();
}

bool get_op(Op *op)
{
    Trace_op trace_op;

    static UINT64 unique_count = 0;

    bool success = gzread(stream, &trace_op, sizeof(Trace_op)) > 0;

    if (KNOB_PRINT_INST.Value()) dprint_trace(&trace_op);

    if (success)
    {
        copy_trace_op(&trace_op, op);
        op->inst_id  = unique_count++;
        op->valid    = TRUE;
    }

    return success;
}

int get_op_latency (Op *op)
{
    //TODO: TOTAL HACK!
    //op->opcode = op->opcode & 31;
    //if (op->opcode > NUM_OP_TYPE) op->opcode = op->opcode & 15;

    // if (op->opcode >= NUM_OP_TYPE) return 2;

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
  cout << "cycle count : " << dec << cycle_count << " retired_instruction : " << retired_instruction << endl; 
  cout << (int)cycle_count << " FE: " ;

  cout << endl;
  dump_reg();
  cout << "--------------------------------------------" << endl; 
}

void print_heartbeat()
{
    static uint64_t last_cycle ;
    static uint64_t last_inst_count;

    float temp_ipc = float(retired_instruction - last_inst_count) /(float)(cycle_count-last_cycle) ;
    float ipc      = float(retired_instruction) /(float)(cycle_count) ;

    cout << "**Heartbeat** cycle_count: "   << cycle_count << " inst:"
         << retired_instruction << " IPC: " << temp_ipc    << " Overall IPC: " << ipc << endl;

    last_cycle = cycle_count;
    last_inst_count = retired_instruction;
}


bool IsOpReady(Op* op);

bool run_a_cycle()
{
    while (true)
    {
        if (((KNOB_MAX_SIM_COUNT.Value()  && (cycle_count         >= KNOB_MAX_SIM_COUNT.Value()))  ||
             (KNOB_MAX_INST_COUNT.Value() && (retired_instruction >= KNOB_MAX_INST_COUNT.Value())) ||
             (sim_end_condition)))
        {
            print_heartbeat();
            print_stats();

            delete g_branchPredictor;

            return TRUE;
        }

        cycle_count++;

        if (!(cycle_count%5000)) print_heartbeat();

        WB_stage();
        EX_stage();
        ISSUE_stage();
        ID_stage();
        FE_stage();

        if (KNOB_PRINT_PIPE_FREQ.Value() && !(cycle_count%KNOB_PRINT_PIPE_FREQ.Value())) print_pipeline();
    }

    return TRUE;
}


void init_structures(void)
{
    init_op_pool();
    init_op_latency();
    init_latches();
    init_register_file();
}

void broad_cast(Op *op)
{
    assert(NULL != op);

    if (0 <= op->dst && op->dst < NUM_REG)
    if(reg_map[op->dst].inst_id == op->inst_id)
    {
        reg_map[op->dst].rdy_cycle = op->done_cycle;
        reg_map[op->dst].valid     = TRUE;
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

bool IsOpReady(Op* op)
{
    assert(NULL != op);

    bool isOpReady = true;

    for (unsigned i = 0; i < 2; i++)
    {
        if (0 <= op->src[i] && op->src[i] < NUM_REG)
        {
            if (op->src_newnames[i] == reg_map[op->src[i]].inst_id)
            {
                isOpReady =
                    (op->src_rdy_cycles[i] <= cycle_count) &&
                    reg_map[op->src[i]].valid &&
                    isOpReady;
            }
        }
    }

    return isOpReady;
}


bool g_clearbranch = false;

void WB_stage()
{
    sim_end_condition =
        !g_rob.size()       &&
        !FE_latch.size()    &&
        !g_scheduler.size() &&
        g_sim_end_precondition;

    unsigned commit_count = 0;

    if (g_clearbranch)
    {
        g_mispredicted_branch = NULL;
        g_clearbranch = false;
    }


    while (g_rob.size() && commit_count <= KNOB_ISSUE_WIDTH.Value())
    {
        Op* op = g_rob.front();

        assert(NULL != op);

        if (!op->done) return;

        retired_instruction++;

        g_rob.pop_front();

        if (KNOB_DEBUG_PRINT.Value())
        {
            cout << "WB_STAGE OP "          << op->inst_id
                 <<  " is retired at cycle " << cycle_count
                 << endl;
        }

        free_op(op);

        commit_count++;
    }
}

list<Op*> g_functional_units;

/* Keep EX_Stage: */
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

        if (op->cf_type == CF_CBR && op == g_mispredicted_branch)
            g_clearbranch = true;

        if(op->in_fu && op->done_cycle <= cycle_count)
        {
            removeFrom.push_back(op);
            op->done = true;
            broad_cast(op);

            if (!op->mem_type) { }
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

        if (!op->in_scheduler)
        {
            g_scheduler.push_back(op);
            op->in_scheduler = true;
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
            {
                cout << "ID_STAGE OP "            << op->inst_id
                     << " is scheduled at cycle " << cycle_count
                     << endl;
            }

            op->in_fu      = true;
            op->done_cycle = cycle_count + get_op_latency(op) + 1;
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

        /// Rename src registers.
        for (int i = 0 ; i < 2; i++)
        {
            if (0 <= op->src[i] && op->src[i] < NUM_REG)
            {
                op->src_newnames[i]   = reg_map[op->src[i]].inst_id;
                op->src_rdy_cycles[i] = reg_map[op->src[i]].rdy_cycle;
            }
        }

        /// Rename dst registers.
        if (0 <= op->dst && op->dst < NUM_REG)
        {
            reg_map[op->dst].inst_id   = op->inst_id;
            reg_map[op->dst].valid     = FALSE;
            reg_map[op->dst].rdy_cycle = 0;
        }

        g_rob.push_back(op);
    }
}

/* Keep FE_Stage */
void FE_stage()
{
    if (  sim_end_condition)    return;
    if (g_sim_end_precondition) return;

    if (NULL != g_mispredicted_branch) return;

    while (FE_latch.size() < KNOB_ISSUE_WIDTH.Value())
    {
        Op* op = get_free_op();

        if (!get_op(op))
        {
            g_sim_end_precondition = true;
            //retired_instruction++;
            //free_op(op);
            return;
        }

        if (op->cf_type == CF_CBR)
        {
            if (op->actually_taken != g_branchPredictor->PredictAndUpdate(op, op->actually_taken))
            {
                g_mispredicted_branch = op;
                bp_miss++;
            }
            else
            {
                g_mispredicted_branch = NULL;
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
    for (unsigned i = 0; i < NUM_REG; i++)
    {
        reg_map[i].valid     = TRUE;
        reg_map[i].inst_id   = 0;
        reg_map[i].rdy_cycle = 0;
    }

    g_branchPredictor = new BranchPredictor(KNOB_GHR_LENGTH.Value());
}

bool icache_access(uint32_t addr)
{
    bool hit = FALSE;
    if (KNOB_PERFECT_ICACHE.Value()) hit = TRUE;
    return hit;
}

bool dcache_access(uint32_t addr)
{
    bool hit = FALSE;
    if (KNOB_PERFECT_DCACHE.Value()) hit = TRUE;
    return hit;
}

