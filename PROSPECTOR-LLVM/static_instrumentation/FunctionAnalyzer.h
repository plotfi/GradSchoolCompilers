// Author: Puyan Lotfi <puyan@gatech.edu>

#pragma once
#include "llvm/Pass.h"
#include "llvm/Function.h"

using namespace llvm;

namespace
{
    class FunctionAnalyzer : public FunctionPass
    {
        public:
            static char ID;

            FunctionAnalyzer() : FunctionPass(ID) { }
            virtual ~FunctionAnalyzer()            { }
            virtual bool runOnFunction(Function &F);
            void InstrumentFunctionEntry(Function &F, uint64_t functionid);
            void InstrumentFunctionExits(Function &F, uint64_t functionid);
    };
}

