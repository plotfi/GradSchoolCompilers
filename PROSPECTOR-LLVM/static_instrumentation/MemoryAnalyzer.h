// Author: Puyan Lotfi <puyan@gatech.edu>

#pragma once
#include "llvm/Pass.h"
#include "llvm/Function.h"

using namespace llvm;

namespace
{
    class MemoryAnalyzer : public FunctionPass
    {
        public:
            static char ID;

            MemoryAnalyzer() : FunctionPass(ID) { }
            virtual ~MemoryAnalyzer()            { }
            virtual bool runOnFunction(Function &F);
            void InstrumentMemoryAccesses(Function &F);
            void InstrumentMemoryLoad(Instruction* inst);
            void InstrumentMemoryStore(Instruction* inst);
    };
}

