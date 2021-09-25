/*BEGIN_LEGAL 
Intel Open Source License 

Copyright (c) 2002-2007 Intel Corporation 
All rights reserved. 
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.  Redistributions
in binary form must reproduce the above copyright notice, this list of
conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.  Neither the name of
the Intel Corporation nor the names of its contributors may be used to
endorse or promote products derived from this software without
specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE INTEL OR
ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
END_LEGAL */
//
//

#include "common.h"
#include "sim.h"
#include "trace.h"
#include "userknob.h"
#include "simknob.h"

void simulator_main(void);
void print_stats(); 

/* global variables */ 
gzFile stream;
uint64_t inst_count =0; 

/* Beginning of Pin supporting functions (you do not need to understand these functions) */ 
LOCALFUN VOID Fini(int code, VOID * v)
{
  if (KNOB_WRITE_TRACE.Value()) std::cout << "write inst " << inst_count << endl; 
  print_stats();

}

LOCALFUN VOID Instruction(INS ins, VOID *v)
{
  ins_decode(ins, stream); 
}

int main(int argc, char *argv[])
{
  PIN_Init(argc, argv);

  INS_AddInstrumentFunction(Instruction, 0);
  PIN_AddFiniFunction(Fini, 0);

  // Never returns
  if (KNOB_WRITE_TRACE.Value()) { 
    stream=gzopen(KNOB_TRACE_NAME.Value().c_str(),"w");
    if (stream == NULL)
      cerr << "Can't open " << KNOB_TRACE_NAME.Value() << endl;
  }

  if (KNOB_READ_TRACE.Value()) {
    stream=gzopen(KNOB_TRACE_NAME.Value().c_str(),"r");
    if (stream == NULL)
      cerr << "Can't open " << KNOB_TRACE_NAME.Value() << endl;
    simulator_main();
  }
  else {
    PIN_StartProgram();
  }

  cout <<" Simulation is ended " << endl; 
  return 0; // make compiler happy
}

/* End of Pin supporting functions (you do not need to understand these functions) */ 


