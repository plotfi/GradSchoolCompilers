#ifndef TRACE_H
#define TRACE_H

#include "sim.h"
VOID write_inst(ADDRINT iaddr); 
VOID dprint_inst(ADDRINT iaddr, string *disassemble_info) ;
void ins_decode(INS ins, gzFile stream) ; 
VOID dprint_trace(Trace_op *trace_op); 


#endif 
