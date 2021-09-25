#ifndef USERKNOB_H
#define USERKNOB_H 


/* ===================================================================== */
/* Commandline Switches */
/* ===================================================================== */

extern KNOB<string> KNOB_OUTPUT_FILE;
extern KNOB<string> KNOB_TRACE_NAME;
extern KNOB<BOOL>   KNOB_READ_TRACE;
extern KNOB<BOOL>   KNOB_WRITE_TRACE;
extern KNOB<BOOL>   KNOB_PRINT_INST; 
extern KNOB<UINT64> KNOB_MAX_SIM_COUNT; 
extern KNOB<UINT64> KNOB_MAX_INST_COUNT; 
extern KNOB<BOOL>   KNOB_PERFECT_ICACHE;
extern KNOB<BOOL>   KNOB_PERFECT_DCACHE;
extern KNOB<BOOL>   KNOB_PERFECT_L2;
extern KNOB<BOOL>   KNOB_PERFECT_BR;
extern KNOB<UINT32> KNOB_ISSUE_WIDTH; 
extern KNOB<UINT32> KNOB_ICACHE_LATENCY; 
extern KNOB<UINT32> KNOB_DCACHE_LATENCY; 
extern KNOB<UINT32> KNOB_L2CACHE_LATENCY; 
extern KNOB<UINT32> KNOB_MEM_LATENCY; 
extern KNOB<UINT64> KNOB_PRINT_PIPE_FREQ; 
extern KNOB<UINT64> KNOB_ROB_SIZE; 
extern KNOB<UINT64> KNOB_SCHED_SIZE; 
extern KNOB<UINT64> KNOB_GHR_LENGTH;
extern KNOB<BOOL> KNOB_DEBUG_PRINT; 
extern KNOB<BOOL> KNOB_OOO_SCHEDULER; 


#endif 
