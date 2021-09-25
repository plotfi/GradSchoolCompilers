/* ===================================================================== */
/* Commandline Switches */
/* ===================================================================== */

KNOB<string> KNOB_OUTPUT_FILE(KNOB_MODE_WRITEONCE,    "pintool",
                              "o", "sim.out", "specify output file name");

KNOB<string> KNOB_TRACE_NAME (KNOB_MODE_WRITEONCE, "pintool", 
                              "tracename", "trace.pzip",  "trace output filename");
KNOB<BOOL> KNOB_READ_TRACE (KNOB_MODE_WRITEONCE, "pintool", 
                            "readtrace", "0", "trace read mode");

KNOB<BOOL> KNOB_WRITE_TRACE (KNOB_MODE_WRITEONCE, "pintool", 
                             "writetrace", "0", "trace write mode");
KNOB<BOOL> KNOB_PRINT_INST (KNOB_MODE_WRITEONCE, "pintool",
                            "printinst", "1", "trace print"); 
KNOB<UINT64> KNOB_MAX_SIM_COUNT (KNOB_MODE_WRITEONCE, "pintool", 
                                 "max_sim_count", "0", "maximum simulation cycle count"); 

KNOB<UINT64> KNOB_MAX_INST_COUNT (KNOB_MODE_WRITEONCE, "pintool", 
                                  "max_inst_count", "1000", "maximum simulation instruction count"); 

KNOB<BOOL> KNOB_PERFECT_ICACHE (KNOB_MODE_WRITEONCE, "pintool",
                                "perfect_icache", "1", "perfect icache");

KNOB<BOOL> KNOB_PERFECT_DCACHE (KNOB_MODE_WRITEONCE, "pintool",
                                "perfect_dcache", "1", "perfect icache");

KNOB<BOOL> KNOB_PERFECT_L2     (KNOB_MODE_WRITEONCE, "pintool",
                                "perfect_l2", "0", "perfect icache");

KNOB<BOOL> KNOB_PERFECT_BR     (KNOB_MODE_WRITEONCE, "pintool",
                                "perfect_br", "0", "perfect branch prediction");
KNOB<UINT32> KNOB_ISSUE_WIDTH  (KNOB_MODE_WRITEONCE, "pintool",
                                "issue_width", "1", "width of the pipeline"); 

KNOB<UINT32> KNOB_ICACHE_LATENCY (KNOB_MODE_WRITEONCE, "pintool",
                                  "icache_latency", "5", "I-cache access latency"); 

KNOB<UINT32> KNOB_DCACHE_LATENCY (KNOB_MODE_WRITEONCE, "pintool",
                                  "dcache_latency", "5", "D-cache access latency"); 

KNOB<UINT32> KNOB_L2CACHE_LATENCY (KNOB_MODE_WRITEONCE, "pintool",
                                   "l2cache_latency", "20", "L2-cache access latency"); 

KNOB<UINT32> KNOB_MEM_LATENCY (KNOB_MODE_WRITEONCE, "pintool",
                               "mem_latency", "200", "mem access latency"); 
KNOB<UINT64> KNOB_PRINT_PIPE_FREQ(KNOB_MODE_WRITEONCE, "pintool",
                                  "print_pipe_freq", "0", "pipeline print frequency"); 
KNOB<UINT64> KNOB_ROB_SIZE(KNOB_MODE_WRITEONCE, "pintool", 
			    "rob_size", "64", "rob size"); 
KNOB<UINT64> KNOB_SCHED_SIZE(KNOB_MODE_WRITEONCE, "pintool",
			    "sched_size", "8", "scheuler size"); 
KNOB<UINT64> KNOB_GHR_LENGTH(KNOB_MODE_WRITEONCE, "pintool",
			     "ghr_length", "8", "gshare predictor length");
KNOB<BOOL> KNOB_DEBUG_PRINT(KNOB_MODE_WRITEONCE, "pintool",
			      "debug_print", "0", "print debug message"); 

KNOB<BOOL> KNOB_OOO_SCHEDULER(KNOB_MODE_WRITEONCE, "pintool", "ooo_scheduler", 
			      "0",  "0: in order scheduler, 1: ooo scheduler");
