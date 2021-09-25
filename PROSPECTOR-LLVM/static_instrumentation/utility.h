// Author: Puyan Lotfi <puyan@gatech.edu>

#pragma once
#include <list>
#include <vector>
#include <string>
#include <exception>
#include "llvm/Function.h"
#include "llvm/Instruction.h"
#include "llvm/BasicBlock.h"
#include "llvm/GlobalVariable.h"
#include "llvm/Analysis/LoopInfo.h"

using namespace std;
using namespace llvm;

typedef vector<Value*>   vvector;
typedef list<BasicBlock*> bblist;

string       GetLoopName       (Loop*         loop);
string       GetFunctionName   (Function*     function);
string       GetBasicBlockName (BasicBlock*   basicblock);

ConstantInt* GetStaticLLVMInt64(uint64_t      num);

BasicBlock*  FixupLoopEntry    (Loop*         loop);

void         InsertFunctionCall(string        functionName,
                                FunctionType* functionType,
                                vvector       parameters,
                                BasicBlock*   basicblock,
                                bool          atBBEntry = true,
                                Instruction* beforeInst = NULL);
