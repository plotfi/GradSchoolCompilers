///
/// Author: Puyan Lotfi <puyan@gatech.edu>
///

#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include "utility.h"
#include "common.h"
#include "statistics.h"
#include "llvm/Module.h"
#include "llvm/Metadata.h"
#include "llvm/LLVMContext.h"
#include "FunctionAnalyzer.h"
#include "llvm/LLVMContext.h"
#include "llvm/Instruction.h"
#include "llvm/Instructions.h"
#include "llvm/DerivedTypes.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Analysis/DebugInfo.h"
#include "llvm/Support/raw_ostream.h"

using namespace std;
using namespace llvm;

char FunctionAnalyzer::ID = 0;
RegisterPass<FunctionAnalyzer> RegisterFunctionAnalyzer("fipp", "Function Instrumentation Pass for Prospector");

bool FunctionAnalyzer::runOnFunction(Function &F)
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
    /// Instrumenting Function.
    ///
    uint64_t functionid = GetFunctionID(&F);
    InstrumentFunctionEntry(F, functionid);
    InstrumentFunctionExits(F, functionid);

    ///
    /// Constructing Function Metadata.
    ///
    unsigned srcLine   = 0;
    unsigned srcColumn = 0;
    string   srcFile   = "???";
    string   srcDir    = "???";

    if (NULL != F.begin())
    for (BasicBlock::iterator
         iter  = F.begin()->begin();
         iter != F.begin()->end();
         ++iter)
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
    /// Measuring the Function Weight.
    ///
    uint64_t weight = 0;

    /// Iterate over all function basicblocks.
    for (Function::iterator
         fiter  = F.begin();
         fiter != F.end();
         ++fiter)
    {
        /// Interate over all basicblock instructions.
        for (BasicBlock::iterator
             biter  = fiter->begin();
             biter != fiter->end();
             ++biter)
        {
            weight++;
        }
    }


    ///
    /// Writing Function Metadata.
    ///
    ofstream mdata;
    mdata.open(MDATAFILE, ios_base::app);
    mdata  << MDATAFUNC
    << "," << functionid
    << "," << GetInternedString(srcDir)
    << "," << GetInternedString(srcFile)
    << "," << srcLine
    << "," << srcColumn
    << "," << GetInternedString(F.getNameStr())
    << "," << weight
    << endl;
    mdata.close();

    return true;
}


///
/// Instrumentation Functions:
///

void FunctionAnalyzer::InstrumentFunctionEntry(Function &F, uint64_t functionid)
{
    // Setup parameters to pass to function call.
    vvector parameters; parameters.push_back(GetStaticLLVMInt64(functionid));

    // Setup function argument types including the return argument.
    vector<const Type*> argumentTypes; argumentTypes.push_back(IntegerType::get(getGlobalContext(), 64));
    FunctionType* returnType = FunctionType::get(Type::getVoidTy(getGlobalContext()), argumentTypes, false);

    InsertFunctionCall("OnFunctionEntry", returnType, parameters, &F.getEntryBlock(), true);
}

void FunctionAnalyzer::InstrumentFunctionExits(Function &F, uint64_t functionid)
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

            if (Instruction::Ret == inst->getOpcode())
            {
                vvector parameters; parameters.push_back(GetStaticLLVMInt64(functionid));

                // Setup function argument types including the return argument.
                vector<const Type*> argumentTypes; argumentTypes.push_back(IntegerType::get(getGlobalContext(), 64));
                FunctionType* returnType = FunctionType::get(Type::getVoidTy(getGlobalContext()), argumentTypes, false);

                InsertFunctionCall("OnFunctionExit", returnType, parameters, &F.getEntryBlock(), false, inst);
            }
        }
    }
}

