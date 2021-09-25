// Author: Puyan Lotfi <puyan@gatech.edu>

#pragma once
#include <map>
#include <list>
#include <stdint.h>
#include "llvm/BasicBlock.h"
#include "llvm/Analysis/LoopPass.h"

using namespace std;
using namespace llvm;

namespace
{
    class LoopAnalyzer : public LoopPass
    {
        public:
            static char ID;

            LoopAnalyzer() : LoopPass(ID) { }
            virtual ~LoopAnalyzer()        { }

            virtual bool doFinalization();
            virtual bool runOnLoop(Loop* L, LPPassManager &LPM);
            virtual void getAnalysisUsage(AnalysisUsage &AU) const;
            virtual bool doInitialization(Loop* L, LPPassManager &LPM);

            void InstrumentLoopIterate (LoopInfo* LI, Loop* loop, uint64_t loopid);
            void InstrumentLoopEntry   (LoopInfo* LI, Loop* loop, uint64_t loopid);
            void InstrumentLoopExits   (LoopInfo* LI, Loop* loop, uint64_t loopid);
    };
}

