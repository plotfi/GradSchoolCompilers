///
/// Author: Puyan Lotfi <puyan@gatech.edu>
///

#include <string>
#include <vector>
#include <cstring>
#include <sstream>
#include <fstream>
#include <stdint.h>
#include <stdlib.h>
#include <iostream>
#include "utility.h"
#include "common.h"
#include "llvm/Pass.h"
#include "statistics.h"
#include "llvm/Value.h"
#include "llvm/Module.h"
#include "LoopAnalyzer.h"
#include "llvm/Metadata.h"
#include "llvm/Function.h"
#include "llvm/LLVMContext.h"
#include "llvm/CallingConv.h"
#include "llvm/Instructions.h"
#include "llvm/DerivedTypes.h"
#include "llvm/Support/Casting.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/DebugInfo.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/PostDominators.h"

using namespace std;
using namespace llvm;

char LoopAnalyzer::ID = 0;
RegisterPass<LoopAnalyzer> RegisterLoopAnalyzer("lipp", "Loop Instrumentation Pass for Prospector");

bool LoopAnalyzer::runOnLoop(Loop* loop, LPPassManager &LPM)
{
    ///
    /// This is to avoid infinite recursion.
    ///
    if (loop->getHeader()->getParent()->getNameStr() == "OnFunctionEntry") return false;
    if (loop->getHeader()->getParent()->getNameStr() == "OnFunctionExit")  return false;
    if (loop->getHeader()->getParent()->getNameStr() == "PrintLoopStack")  return false;
    if (loop->getHeader()->getParent()->getNameStr() == "OnLoopIterate")   return false;
    if (loop->getHeader()->getParent()->getNameStr() == "OnLoopEntry")     return false;
    if (loop->getHeader()->getParent()->getNameStr() == "OnLoopExit")      return false;

    ///
    /// Instrumenting Loop 
    ///
    LoopInfo*    loopInfo     = getAnalysisIfAvailable<LoopInfo>();
    uint64_t     loopid       = GetLoopID(loop);
    InstrumentLoopExits   (loopInfo, loop, loopid);
    InstrumentLoopIterate (loopInfo, loop, loopid);
    InstrumentLoopEntry   (loopInfo, loop, loopid);

    ///
    /// Constructing Loop Metadata.
    ///
    BasicBlock* header       = loop->getHeader();
    uint64_t    parentloopid = GetLoopID(loop->getParentLoop());
    uint64_t    parentfuncid = GetFunctionID(header->getParent());
    unsigned    srcColumn    = 0;
    unsigned    srcLine      = 0;
    string      srcFile      = "???";
    string      srcDir       = "???";

    for (BasicBlock::iterator iter = header->begin();
         iter != header->end(); ++iter)
    {
        Instruction* inst = iter;

        if (MDNode* N = inst->getMetadata("dbg"))
        {
            DILocation  Loc(N);
            srcLine   = Loc.getLineNumber();
            srcColumn = Loc.getColumnNumber();
            srcFile   = Loc.getFilename().str();
            srcDir    = Loc.getDirectory().str();
            break;
        }
    }

    ///
    /// Calculating Loop Weight.
    ///
    uint64_t loopWeight = 0;

    for (Loop::block_iterator bbiter = loop->block_begin();
         bbiter != loop->block_end();
         bbiter++)
    {
        for (BasicBlock::iterator institer = (*bbiter)->begin();
             institer != (*bbiter)->end();
             institer++)
        {
            loopWeight++;
        }
    }




    ///
    /// Writing Loop Metadata.
    ///
    ofstream mdata;
    mdata.open(MDATAFILE, ios_base::app);
    mdata  << MDATALOOP
    << "," << loopid
    << "," << GetInternedString(srcDir)
    << "," << GetInternedString(srcFile)
    << "," << srcLine
    << "," << srcColumn
    << "," << GetInternedString(header->getNameStr())
    << "," << parentloopid
    << "," << parentfuncid
    << "," << loopWeight
    << endl;

    mdata.close();

    return true;
}

bool LoopAnalyzer::doInitialization(Loop* L, LPPassManager &LPM)
{
    LPM.initializeAnalysisInfo();
    return true;
}

bool LoopAnalyzer::doFinalization()
{
    return true;
}

void LoopAnalyzer::getAnalysisUsage(AnalysisUsage &AU) const
{
    AU.addRequired<LoopInfo>();
}

///
/// Instrumentation Functions:
///

void LoopAnalyzer::InstrumentLoopEntry(LoopInfo* loopInfo, Loop* loop, uint64_t loopid)
{
    assert(NULL != loopInfo);
    assert(NULL != loop);

    // Insert a preheader if none available.
    //BasicBlock* preheader = FixupLoopEntry(loop);
    BasicBlock* preheader = loop->getLoopPreheader();
    assert(preheader && "Loop simplify pass should add preheaders.");

    // Setup parameters to pass to function call.
    vvector parameters; parameters.push_back(GetStaticLLVMInt64(loopid));

    // Setup function argument types including the return argument.
    vector<const Type*> argumentTypes; argumentTypes.push_back(IntegerType::get(getGlobalContext(), 64));
    FunctionType* returnType = FunctionType::get(Type::getVoidTy(getGlobalContext()), argumentTypes, false);

    InsertFunctionCall("OnLoopEntry", returnType, parameters, preheader, true);
}

void LoopAnalyzer::InstrumentLoopIterate(LoopInfo* loopInfo, Loop* loop, uint64_t loopid)
{
    assert(NULL != loopInfo);
    assert(NULL != loop);

    BasicBlock* header    = loop->getHeader();
    BasicBlock* preheader = loop->getLoopPreheader();


    BasicBlock* loopBody = NULL;

    for (succ_iterator iter = succ_begin(header);
         iter != succ_end(header);
         ++iter)
    {
        if (loop == loopInfo->getLoopFor(*iter))
        {
            loopBody = *iter;
        }
        else
        {
            vector<BasicBlock*> loopbbs = loop->getBlocks();

            for (unsigned i = 0; i < loopbbs.size(); i++)
            {
                if (loopbbs[i] != header &&
                    loopbbs[i] != preheader)
                {
                    loopBody = loopbbs[i];
                    break;
                }
            }
        }

        if (loopBody) break;
    }

    if (!loopBody)
    {
        cerr << "Problem Loop: " << loop->getHeader()->getNameStr() << endl;
        cerr << "Problem Loop: " << loop->getHeader()->getParent()->getNameStr() << endl;
    }

    assert("Loop header does not branch to a proper loop body" && loopBody);

    // Setup parameters to pass to function call.
    vvector parameters; parameters.push_back(GetStaticLLVMInt64(loopid));

    // Setup function argument types including the return argument.
    vector<const Type*> argumentTypes; argumentTypes.push_back(IntegerType::get(getGlobalContext(), 64));
    FunctionType* returnType = FunctionType::get(Type::getVoidTy(getGlobalContext()), argumentTypes, false);

    InsertFunctionCall("OnLoopIterate", returnType, parameters, loopBody, false);
}

void LoopAnalyzer::InstrumentLoopExits(LoopInfo* loopInfo, Loop* loop, uint64_t loopid)
{
    assert(NULL != loopInfo);
    assert(NULL != loop);

    /*
    for (Loop::block_iterator bbiter = loop->block_begin();
         bbiter != loop->block_end();
         bbiter++)
    {
        BasicBlock* bb = *bbiter;

        if (loop == loopInfo->getLoopFor(bb))
        {
            for (BasicBlock::iterator institer = bb->begin();
                 institer != bb->end();
                 institer++)
            {
                Instruction* inst = institer;

                if (isa<BranchInst>(inst))
                {
                    //Loop* destLoop = loopInfo->getLoopFor();
                    //uint64_t destLoopId = GetLoopID(destLoop);

                    // Setup parameters to pass to function call.
                    vvector parameters;
                    parameters.push_back(GetStaticLLVMInt64(loopid));
                    parameters.push_back(GetStaticLLVMInt64(loopid));

                    // Setup function argument types including the return argument.
                    vector<const Type*> argumentTypes;
                    argumentTypes.push_back(IntegerType::get(getGlobalContext(), 64)); // loopid
                    argumentTypes.push_back(IntegerType::get(getGlobalContext(), 64)); // destination loopid, or 0
                    FunctionType* returnType = FunctionType::get(Type::getVoidTy(getGlobalContext()), argumentTypes, false);

                    InsertFunctionCall("OnLoopExit", returnType, parameters, bb, false, inst);
                }
            }
        }
    }*/


    SmallVector<BasicBlock*, 8> exits(0); loop->getExitBlocks(exits);

    for (SmallVectorImpl<BasicBlock*>::iterator
         basicBlock  = exits.begin();
         basicBlock != exits.end();
         basicBlock ++)
    {
        Loop* destLoop = loopInfo->getLoopFor(*basicBlock);
        uint64_t destLoopId = GetLoopID(destLoop);

        // Setup parameters to pass to function call.
        vvector parameters;
        parameters.push_back(GetStaticLLVMInt64(loopid));
        parameters.push_back(GetStaticLLVMInt64(destLoopId));

        // Setup function argument types including the return argument.
        vector<const Type*> argumentTypes;
        argumentTypes.push_back(IntegerType::get(getGlobalContext(), 64)); // loopid
        argumentTypes.push_back(IntegerType::get(getGlobalContext(), 64)); // destination loopid, or 0
        FunctionType* returnType = FunctionType::get(Type::getVoidTy(getGlobalContext()), argumentTypes, false);

        InsertFunctionCall("OnLoopExit", returnType, parameters, *basicBlock, false);
    }

        /*
        Instruction* insertPoint = NULL;

        for (BasicBlock::iterator
             instruction  = basicBlock->begin();
             instruction != basicBlock->end();
             instruction ++)
        {
            if (isa<CallInst>(instruction))
            {
                if (CallInst* callInst = dyn_cast<CallInst>(instruction))
                {
                    if (string("OnLoopExit") ==
                            callInst->getCalledFunction()->getNameStr())
                    {
                        insertPoint = instruction;
                    }
                    else break;
                }
                else break;
            }
            else break;
        }

        if (insertPoint)
            InsertFunctionCall("OnLoopExit", returnType, parameters, *iter, true, insertPoint);
        else
            InsertFunctionCall("OnLoopExit", returnType, parameters, *iter, true);
       */
 }

