// Author: Puyan Lotfi <puyan@gatech.edu>

#pragma once
#include <stdint.h>
#include "llvm/Function.h"
#include "llvm/Analysis/LoopPass.h"

using namespace llvm;
using namespace std;

struct StatisticFileDumper
{
    StatisticFileDumper();
    ~StatisticFileDumper();
};

uint64_t GetInternedString(string str);
uint64_t GetFunctionID(Function* function);
uint64_t GetStoreID(StoreInst* store);
uint64_t GetLoadID(LoadInst* load);
uint64_t GetLoopID(Loop* loop);

