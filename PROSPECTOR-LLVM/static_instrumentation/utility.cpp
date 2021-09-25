// Author: Puyan Lotfi <puyan@gatech.edu>

#include <sstream>
#include "utility.h"
#include "llvm/Module.h"
#include "llvm/BasicBlock.h"
#include "llvm/CallingConv.h"
#include "llvm/LLVMContext.h"
#include "llvm/Instructions.h"

/**
 * functionName: Name of the function to call.
 * functionType: Type for in and out parameters of function.
 * parameters:   The parameters to pass to the function.
 * basicblock:   The basicblock to insert the into.
 * atBBEntry:    If true the insert the callinst at the begining
 *               of the basic block (after all preceding PHI nodes),
 *               otherwise insert at the end of the basicblock
 *               (before the branch instruction).
 */
void InsertFunctionCall(string functionName, FunctionType* functionType, vvector parameters, BasicBlock* basicblock,
                        bool atBBEntry, Instruction* beforeInst)
{
    assert(NULL != basicblock);
    assert(NULL != functionType);

    Constant* function = basicblock->getParent()->getParent()->getOrInsertFunction(functionName, functionType);

    if (NULL == beforeInst)
    {
        if (atBBEntry)
        {
            for (BasicBlock::InstListType::iterator iter =  basicblock->getInstList().begin();
                 iter != basicblock->getInstList().end(); ++iter)
            {
                if (Instruction::PHI != iter->getOpcode())
                {
                    beforeInst = iter; break;
                }
            }
        }
        else
        {
            beforeInst = &basicblock->getInstList().back();
        }
    }

    assert(beforeInst && "Can not insert instrumentation code before a NULL instruction.");

    CallInst::Create(function, parameters.begin(), parameters.end(), "", beforeInst);
}

/**
 * Given a loop, check to see that it has a preheader
 * and if it does not then insert one.
 */
BasicBlock* FixupLoopEntry(Loop* loop)
{
    BasicBlock* preheader = loop->getLoopPreheader();

    if (!preheader)
    {
        assert(false && "Preheader fixup is disabled. LLVM-GCC should be inserting a default preheader");

        BasicBlock* header = loop->getHeader();

        preheader = BasicBlock::Create(header->getContext(),
                    (header->getNameStr() + "_pre").c_str(),
                    header->getParent(), 0);

        BranchInst::Create(header, preheader);

        for (pred_iterator pred_iter = pred_begin(header);
             pred_iter != pred_end(header);
             pred_iter++)
        {
            // Part of a backedge, skip
            if (preheader == *pred_iter) continue;
            if (loop->contains(*pred_iter)) continue;

            BranchInst* predBrInst = cast<BranchInst>((*pred_iter)->getTerminator());

            // Point all pred branches to the fake Preheader
            for (unsigned i = 0; i < predBrInst->getNumSuccessors(); i++)
            {
                if (predBrInst->getSuccessor(i) == header)
                    predBrInst->setSuccessor(i, preheader);
            }
        }
    }

    return preheader;
}

ConstantInt* GetStaticLLVMInt64(uint64_t num)
{
    stringstream sstr; sstr << num;
    ConstantInt* constint64 = ConstantInt::get(getGlobalContext(), APInt(64, StringRef(sstr.str().c_str()), 10));
    return       constint64;
}

string GetFunctionName(Function* function)
{
    const FunctionType* functiontype = function->getFunctionType();

    string return_type = functiontype->getReturnType()->getDescription();
    string param_types = "";

    for (FunctionType::param_iterator iter = functiontype->param_begin();
         iter != functiontype->param_end();
         iter++)
    {
        Type* type = *iter;

        string typeDescription = type->getDescription();

startover: // XXX

        const char* typeDescription_cstr = typeDescription.c_str();

        for (unsigned int i = 0; i< typeDescription.size(); i++)
        {
            if (typeDescription_cstr[i] == ' ' ||
                    typeDescription_cstr[i] == ',')
            {
                typeDescription = typeDescription.erase(i, 1);
                goto startover;
            }
        }

        param_types = param_types + "_" + typeDescription;
    }

    string full_function_name = function->getNameStr();
    // string full_function_name = function->getNameStr() + "_" + return_type + param_types;

    return full_function_name;
}

string GetLoopName(Loop* loop)
{
    if (!loop) return "";

    BasicBlock* header = loop->getHeader();
    ostringstream loopname;
    loopname << header->getNameStr() << "_" << GetFunctionName(header->getParent());

    return loopname.str();
}

string GetBasicBlockName(BasicBlock* basicblock)
{
    if (!basicblock) return "";

    ostringstream basicblockname;
    basicblockname << basicblock->getNameStr() << "_" << GetFunctionName(basicblock->getParent());

    return basicblockname.str();
}

