#include "common.h"
#include "sim.h"
#include "trace.h" 
#include "userknob.h"
#include <stdlib.h>
/*******************************************************************/
/* Simulator frame */ 
/*******************************************************************/

bool run_a_cycle();
void init_structures(); 



/* uop_pool related variables */ 

uint32_t free_op_num;
uint32_t active_op_num; 
Op *op_pool; 
Op *op_pool_free_head = NULL; 

/* simulator related local functions */ 

bool icache_access(uint32_t addr);
bool dcache_access(uint32_t addr);
void  init_latches(void);
void init_register_file();


// simulator main function is called from outside of this file 

void simulator_main(void) 
{
    // trace driven simulation 
    init_structures(); 
    run_a_cycle();

}
int op_latency[NUM_OP_TYPE]; 

void init_op_latency(void)
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
    op->num_src               = 0; 
    op->src[0]                = -1; 
    op->src[1]                = -1;
    op->dst                   = -1; 
    op->opcode                = 0; 
    op->is_fp                 = false;
    op->cf_type               = NOT_CF;
    op->mem_type              = NOT_MEM;
    op->write_flag             = 0;
    op->inst_size             = 0;
    op->ld_vaddr              = 0;
    op->st_vaddr              = 0;
    op->instruction_addr      = 0;
    op->branch_target         = 0;
    op->actually_taken        = 0;
    op->mem_read_size         = 0;
    op->mem_write_size        = 0;
    op->valid                 = FALSE; 
    /* you might add more features here */ 
}


void init_op_pool(void)
{
    /* initialize op pool */ 
    op_pool = new Op [1024];
    free_op_num = 1024; 
    active_op_num = 0; 
    uint32_t op_pool_entries = 0; 
    int ii;
    for (ii = 0; ii < 1023; ii++) {

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
    /* return a free op from op pool */ 

    if (op_pool_free_head == NULL || (free_op_num == 1)) {
        std::cout <<"ERROR! OP_POOL SIZE is too small!! " << endl; 
        std::cout <<"please check free_op function " << endl; 
        assert(1); 
        exit(1);
    }

    free_op_num--;
    assert(free_op_num); 

    Op *new_op = op_pool_free_head; 
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
    op->valid = FALSE; 
    op->op_pool_next = op_pool_free_head;
    op_pool_free_head = op; 
}



/*******************************************************************/
/*  Data structure */
/*******************************************************************/

typedef struct pipeline_latch_struct {
    Op *op; /* you must update this data structure. */
    bool op_valid;
    /* you might add more data structures. But you should complete the above data elements */ 
}pipeline_latch;


typedef struct Reg_element_struct{
    bool valid;
    // data is not needed 
    /* you might add more data structures. But you should complete the above data elements */ 
}REG_element; 

REG_element register_file[NUM_REG];


/*******************************************************************/
/* These are the functions you'll have to write.  */ 
/*******************************************************************/

void FE_stage();
void ID_stage();
void EX_stage(); 
void MEM_stage();
void WB_stage(); 

/*******************************************************************/
/*  These are the variables you'll have to write.  */ 
/*******************************************************************/

bool sim_end_condition = FALSE;     /* please complete the condition. */ 
UINT64 retired_instruction = 0;    /* number of retired instruction. (only correct instructions) */ 
UINT64 cycle_count = 0;            /* total number of cycles */ 
UINT64 data_hazard_count = 0;  
UINT64 control_hazard_count = 0; 
UINT64 icache_miss_count = 0;      /* total number of icache misses. for Lab #2 and Lab #3 */ 
UINT64 dcache_miss_count = 0;      /* total number of dcache  misses. for Lab #2 and Lab #3 */ 
UINT64 l2_cache_miss_count = 0;    /* total number of L2 cache  misses. for Lab #2 and Lab #3 */  



pipeline_latch *MEM_latch;  
pipeline_latch *EX_latch;
pipeline_latch *ID_latch;
pipeline_latch *FE_latch;
UINT64 ld_st_buffer[LD_ST_BUFFER_SIZE]; 
UINT64 next_pc; 

/*******************************************************************/
/*  Print messages  */
/*******************************************************************/

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
    out << "Total control hazard : " << control_hazard_count << endl; 
    out.close();
}

/*******************************************************************/
/*  Support Functions  */ 
/*******************************************************************/

bool get_op(Op *op)
{
    static UINT64 unique_count = 0; 
    Trace_op trace_op; 
    bool success = FALSE; 
    // read trace 
    // fill out op info 
    // return FALSE if the end of trace 
    success = (gzread(stream, &trace_op, sizeof(Trace_op)) >0 );
    if (KNOB_PRINT_INST.Value()) dprint_trace(&trace_op); 

    /* copy trace structure to op */ 
    if (success) { 
        copy_trace_op(&trace_op, op); 

        op->inst_id  = unique_count++;
        op->valid    = TRUE; 
    }
    return success; 
}
/* return op execution cycle latency */ 

int get_op_latency (Op *op) 
{
    assert (op->opcode < NUM_OP_TYPE); 
    return op_latency[op->opcode];
}


void dump_reg() {
    for (int ii = 0; ii < NUM_REG; ii++) {
        std::cout << cycle_count << ":register[" << ii  << "]: V:" << register_file[ii].valid << endl; 
    }
}

void print_pipeline() {
  std::cout << "--------------------------------------------" << endl; 
  std::cout <<"cycle count : " << dec << cycle_count << " retired_instruction : " << retired_instruction << endl; 
  std::cout << (int)cycle_count << " FE: " ;
  if (FE_latch->op_valid) {
    Op *op = FE_latch->op; 
    cout << (int)op->inst_id ;
  }
  else {
    cout <<"XXXX";
  }
  std::cout << " ID: " ;
  if (ID_latch->op_valid) {
    Op *op = ID_latch->op; 
    cout << (int)op->inst_id ;
  }
  else {
    cout <<"XXXX";
  }
  std::cout << " EX: " ;
  if (EX_latch->op_valid) {
    Op *op = EX_latch->op; 
    cout << (int)op->inst_id ;
  }
  else {
    cout <<"XXXX";
  }


  std::cout << " MEM: " ;
  if (MEM_latch->op_valid) {
    Op *op = MEM_latch->op; 
    cout << (int)op->inst_id ;
  }
  else {
    cout <<"XXXX";
  }
  cout << endl; 
  dump_reg();   
  std::cout << "--------------------------------------------" << endl; 
}

void print_heartbeat()
{
    static uint64_t last_cycle ;
    static uint64_t last_inst_count; 
    float temp_ipc = float(retired_instruction - last_inst_count) /(float)(cycle_count-last_cycle) ;
    float ipc = float(retired_instruction) /(float)(cycle_count) ;
    /* Do not modify this function. This messages will be used for grading */ 
    cout <<"**Heartbeat** cycle_count: " << cycle_count << " inst:" << retired_instruction << " IPC: " << temp_ipc << " Overall IPC: " << ipc << endl; 
    last_cycle = cycle_count;
    last_inst_count = retired_instruction; 
}
/*******************************************************************/
/*                                                                 */
/*******************************************************************/

bool run_a_cycle(){

    for (;;) { 
        if (((KNOB_MAX_SIM_COUNT.Value() && (cycle_count >= KNOB_MAX_SIM_COUNT.Value())) || 
                    (KNOB_MAX_INST_COUNT.Value() && (retired_instruction >= KNOB_MAX_INST_COUNT.Value())) ||  (sim_end_condition))) { 
            // please complete sim_end_condition 
            // finish the simulation 
            print_heartbeat(); 
            print_stats();
            return TRUE; 
        }
        cycle_count++; 
        if (!(cycle_count%5000)) {
            print_heartbeat(); 
        }
        WB_stage(); 
        MEM_stage();
        EX_stage();
        ID_stage();
        FE_stage(); 
        if (KNOB_PRINT_PIPE_FREQ.Value() && !(cycle_count%KNOB_PRINT_PIPE_FREQ.Value())) print_pipeline();
    }
    return TRUE; 
}


/*******************************************************************/
/* Complete the following fuctions.  */
/* You can add new data structures and also new elements to Op, Pipeline_latch data structure */ 
/*******************************************************************/

void init_structures(void)
{
    init_op_pool(); 
    init_op_latency();
    /* please initialize other data stucturs */ 
    /* you must complete the function */
    init_latches();
    init_register_file();
}


static unsigned stall_for_branch     = 0;
static int      remaining_ex_cycles  = 0;
static bool     sim_end_precondition = false;


bool SrcRegsReady(Op* op)
{
    if (0 <= op->src[0] && op->src[0] < NUM_REG &&
        0 <= op->src[1] && op->src[1] < NUM_REG && op->num_src == 2)
    {
        return register_file[op->src[0]].valid &&
        register_file[op->src[1]].valid;
    }

    if (0 <= op->src[0] && op->src[0] < NUM_REG && op->num_src == 1)
    {
        return register_file[op->src[0]].valid;
    }

    if (0 <= op->src[1] && op->src[1] < NUM_REG && op->num_src == 1)
    {
        return register_file[op->src[1]].valid;
    }

    return true;
}

void WB_stage()
{
    if (!MEM_latch->op && !EX_latch->op && !ID_latch->op && !FE_latch->op && sim_end_precondition)
    {
        //retired_instruction++;
        sim_end_condition = 1;
    }

    if (MEM_latch->op)
    {
        Op* op              = MEM_latch->op;
        MEM_latch->op       = NULL;
        MEM_latch->op_valid = false;


        if (0 <= op->dst && op->dst < NUM_REG)
            register_file[op->dst].valid = true;

        if (op->cf_type >= CF_BR) stall_for_branch--;

        retired_instruction++;
        free_op(op);
    }
}

void MEM_stage()
{
    static Op* mem_op = NULL;

    if (!EX_latch->op) return;

    if (!mem_op)
    {
        mem_op = EX_latch->op;
    }

    EX_latch->op        = NULL;
    EX_latch->op_valid  = false;
    MEM_latch->op       = mem_op;
    MEM_latch->op_valid = true;
    mem_op              = NULL;
}

void EX_stage()
{
    static Op* ex_op = NULL;

    if (!ID_latch->op) return;

    if (!ex_op)
    {
        ex_op = ID_latch->op;
        remaining_ex_cycles = get_op_latency(ex_op);
    }

    remaining_ex_cycles--;

    if (remaining_ex_cycles == 0)
    {
        ID_latch->op       = NULL;
        ID_latch->op_valid = false;
        EX_latch->op       = ex_op;
        EX_latch->op_valid = true;
        ex_op              = NULL;
    }
}

void ID_stage()
{
    static Op* id_op = NULL;

    if (!FE_latch->op) return;

    if (!id_op)
    {
        id_op = FE_latch->op;
    }

    bool src_regs_ready = SrcRegsReady(id_op);

    if (!stall_for_branch && src_regs_ready && !ID_latch->op)
    {
        FE_latch->op                 = NULL;
        FE_latch->op_valid           = false;
        ID_latch->op                 = id_op;
        ID_latch->op_valid           = true;

        if (id_op->cf_type >= CF_BR) stall_for_branch++;

        if (0 <= id_op->dst && id_op->dst < NUM_REG)
            register_file[id_op->dst].valid = false;
    }
    else if (!src_regs_ready && !stall_for_branch && !ID_latch->op)
    {
        if (id_op->cf_type >= CF_BR) control_hazard_count++;
        data_hazard_count++;
    }

    if (stall_for_branch)
    {
        control_hazard_count++;
    }


    id_op = NULL;
}


void FE_stage()
{
    static Op* fe_op = NULL;

    if (sim_end_precondition) return;

    if (!fe_op)
    {
        Op* op = get_free_op();
        sim_end_precondition = !get_op(op);

        if (sim_end_precondition)
        {
            retired_instruction++;
            free_op(op);
            return;
        }

        fe_op = op;
    }

    /*
    ofstream outfile; outfile.open("F00B4R.txt", ios_base::app); outfile
    << " Inst   ID: " << (int)fe_op->inst_id
    << " Src  Regs: " << (int)fe_op->src[0]
    << " "            << (int)fe_op->src[1]
    << " Dst   Reg: " << (int)fe_op->dst
    << " Latency:   " << (int)get_op_latency(fe_op)
    << endl;
    */

    if (!FE_latch->op && !stall_for_branch)
    {
        FE_latch->op = fe_op;
        FE_latch->op_valid = true;
        fe_op = NULL;
    }

    //   next_pc = pc + op->inst_size;  // you need this code for building a branch predictor 
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
    MEM_latch = new pipeline_latch();
    EX_latch = new pipeline_latch();
    ID_latch = new pipeline_latch();
    FE_latch = new pipeline_latch();

    MEM_latch->op = NULL;
    EX_latch->op = NULL;
    ID_latch->op = NULL;
    FE_latch->op = NULL;

    /* you must set valid value correctly  */ 
    MEM_latch->op_valid = false;
    EX_latch->op_valid = false;
    ID_latch->op_valid = false;
    FE_latch->op_valid = false;

}

bool icache_access(uint32_t addr) {

    /* For Lab #1, you assume that all I-cache hit */     
    bool hit = FALSE; 
    if (KNOB_PERFECT_ICACHE.Value()) hit = TRUE; 
    return hit; 
}



bool dcache_access(uint32_t addr) {
    /* For Lab #1, you assume that all D-cache hit */     
    bool hit = FALSE;
    if (KNOB_PERFECT_DCACHE.Value()) hit = TRUE; 
    return hit; 
}
