///
/// Author: Puyan Lotfi <puyan@gatech.edu>
///

#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "utility.h"
#include "common.h"
#include "statistics.h"
#include "llvm/Module.h"
#include "llvm/Metadata.h"
#include "llvm/LLVMContext.h"
#include "MemoryAnalyzer.h"
#include "llvm/LLVMContext.h"
#include "llvm/Instruction.h"
#include "llvm/Instructions.h"
#include "llvm/DerivedTypes.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Analysis/DebugInfo.h"
#include "llvm/Support/raw_ostream.h"

using namespace std;
using namespace llvm;

char MemoryAnalyzer::ID = 0;
RegisterPass<MemoryAnalyzer> RegisterMemoryAnalyzer("mipp", "Memory Instrumentation Pass for Prospector");

bool MemoryAnalyzer::runOnFunction(Function &F)
{
    ///
    /// This is to avoid infinite recursion.
    ///
    if (F.getNameStr() == "OnFunctionEntry") return false;
    if (F.getNameStr() == "OnFunctionExit")  return false;
    if (F.getNameStr() == "PrintLoopStack")  return false;
    if (F.getNameStr() == "OnLoopIterate")   return false;
    if (F.getNameStr() == "OnLoopEntry")     return false;
    if (F.getNameStr() == "OnLoopExit")      return false;

    ///
    /// Instrumenting Memory Load and Stores.
    ///
    InstrumentMemoryAccesses(F);

    return true;
}


///
/// Instrumentation Functions:
///

void MemoryAnalyzer::InstrumentMemoryAccesses(Function &F)
{
    for (Function::iterator bbiter = F.begin();
         bbiter != F.end();
         bbiter++)
    {
        for (BasicBlock::iterator institer = bbiter->begin();
             institer != bbiter->end();
             institer++)
        {
            Instruction* inst = institer;

            if (isa<LoadInst>(inst))
            {
                InstrumentMemoryLoad(inst);
            }
            else if (isa<StoreInst>(inst))
            {
                InstrumentMemoryStore(inst);
            }
            /*
            else if (isa<AllocaInst>(inst))
            {
                AllocaInst* allocaInst = dyn_cast<AllocaInst>(inst);
                cerr << "Alloca Instruction" << allocaInst->getOpcodeName() << endl;
            }
            else if (isa<GetElementPtrInst>(inst))
            {
                GetElementPtrInst* gepInst = dyn_cast<GetElementPtrInst>(inst);
                cerr << "GetElementPtrInst Instruction" << gepInst->getOpcodeName() << endl;
            }
            */
            else
            {
                // not a memory operation
            }
        }
    }
}

void MemoryAnalyzer::InstrumentMemoryLoad(Instruction* inst)
{
    if (isa<LoadInst>(inst))
    if (LoadInst* loadInst     = dyn_cast<LoadInst>(inst))
    if (GetElementPtrInst* GEP = dyn_cast<GetElementPtrInst>(loadInst->getPointerOperand()))
    {
        // Cast the GetElementPtrInst address to a simple 64 bit Integer. Same as & operator in C.
        CastInst* memoryLoadAddr = new PtrToIntInst(GEP, IntegerType::get(inst->getContext(), 64), "", inst);
        // Setup parameters to pass to function call.
        vvector parameters; parameters.push_back(memoryLoadAddr);
        // Setup function argument types including the return argument.
        vector<const Type*> argumentTypes; argumentTypes.push_back(IntegerType::get(getGlobalContext(), 64));
        FunctionType* returnType = FunctionType::get(Type::getVoidTy(getGlobalContext()), argumentTypes, false);

        // Instrument the OnMemory Function.
        InsertFunctionCall("OnMemoryLoad", returnType, parameters, inst->getParent(), false, inst);


        ///
        /// Constructing Metadata.
        ///
        //
        // BasicBlock* basicBlock   = inst->getParent();
        // uint64_t    parentfuncid = GetFunctionID(basicBlock->getParent());

        uint64_t    loadid       = GetLoadID(loadInst);
        unsigned    srcLine      = 0;
        unsigned    srcColumn    = 0;
        string      srcFile      = "???";
        string      srcDir       = "???";

        if (MDNode* N = inst->getMetadata("dbg"))
        {
            DILocation  Loc(N);
            srcLine   = Loc.getLineNumber();
            srcColumn = Loc.getColumnNumber();
            srcFile   = Loc.getFilename().str();
            srcDir    = Loc.getDirectory().str();
        }

        ///
        /// Writing Loop Metadata.
        ///
        ofstream mdata;
        mdata.open(MDATAFILE, ios_base::app);
        mdata  << MDATALOAD
            << "," << loadid
            << "," << GetInternedString(srcDir)
            << "," << GetInternedString(srcFile)
            << "," << srcLine
            << "," << srcColumn
            // << "," << GetInternedString(basicBlock->getNameStr())
            // << "," << parentloopid
            // << "," << parentfuncid
            << endl;

        mdata.close();
    }
}

void MemoryAnalyzer::InstrumentMemoryStore(Instruction* inst)
{
    if (isa<StoreInst>(inst))
    if (StoreInst* storeInst   = dyn_cast<StoreInst>(inst))
    if (GetElementPtrInst* GEP = dyn_cast<GetElementPtrInst>(storeInst->getPointerOperand()))
    {
        // Cast the GetElementPtrInst address to a simple 64 bit Integer. Same as & operator in C.
        CastInst* memoryLoadAddr = new PtrToIntInst(GEP, IntegerType::get(inst->getContext(), 64), "", inst);
        // Setup parameters to pass to function call.
        vvector parameters; parameters.push_back(memoryLoadAddr);
        // Setup function argument types including the return argument.
        vector<const Type*> argumentTypes; argumentTypes.push_back(IntegerType::get(getGlobalContext(), 64));
        FunctionType* returnType = FunctionType::get(Type::getVoidTy(getGlobalContext()), argumentTypes, false);

        // Instrument the OnMemory Function.
        InsertFunctionCall("OnMemoryStore", returnType, parameters, inst->getParent(), false, inst);

        ///
        /// Constructing Metadata.
        ///
        //
        // BasicBlock* basicBlock   = inst->getParent();
        // uint64_t    parentfuncid = GetFunctionID(basicBlock->getParent());

        uint64_t    storeid       = GetStoreID(storeInst);
        unsigned    srcLine      = 0;
        unsigned    srcColumn    = 0;
        string      srcFile      = "???";
        string      srcDir       = "???";

        if (MDNode* N = inst->getMetadata("dbg"))
        {
            DILocation  Loc(N);
            srcLine   = Loc.getLineNumber();
            srcColumn = Loc.getColumnNumber();
            srcFile   = Loc.getFilename().str();
            srcDir    = Loc.getDirectory().str();
        }

        ///
        /// Writing Loop Metadata.
        ///
        ofstream mdata;
        mdata.open(MDATAFILE, ios_base::app);
        mdata  << MDATASTORE
            << "," << storeid
            << "," << GetInternedString(srcDir)
            << "," << GetInternedString(srcFile)
            << "," << srcLine
            << "," << srcColumn
            // << "," << GetInternedString(basicBlock->getNameStr())
            // << "," << parentloopid
            // << "," << parentfuncid
            << endl;

        mdata.close();

    }
}

