
#include <map>
#include <cstdio>
#include <cassert>
#include <fstream>
#include <sstream>
#include <stdint.h>
#include "common.h"
#include "llvm/Function.h"
#include "llvm/BasicBlock.h"
#include "llvm/ADT/StringMap.h"
#include "llvm/Analysis/LoopPass.h"

#include <iostream>
#include <vector>

#include "statistics.h"

using namespace std;
using namespace llvm;

map<string   , uint64_t> g_internTable;

map<Function*,  uint64_t> g_functionids;
map<StoreInst*, uint64_t> g_storeids;
map<LoadInst*,  uint64_t> g_loadids;
map<Loop*,      uint64_t> g_loopids;

StatisticFileDumper statisticFileDumper;

///
/// This constructor runs at the start of the opt program.
///
StatisticFileDumper::StatisticFileDumper()
{
    remove(MDATAFILE);
}

///
/// This destructor runs at the end of the opt program.
///
StatisticFileDumper::~StatisticFileDumper()
{
    vector<string> internedValuesInorder(g_internTable.size() + 1);
    internedValuesInorder[0] = "INTERNS";

    for (map<string, uint64_t>::iterator iter = g_internTable.begin();
         iter != g_internTable.end();
         iter++)
    {
        internedValuesInorder[iter->second] = iter->first;
    }

    for (unsigned i = 1; i < internedValuesInorder.size(); i++)
    {
        ofstream mdata;
        mdata.open(MDATAFILE, ios_base::app);
        mdata  << MDATAINTR
        << "," << i
        << "," << internedValuesInorder[i]
        << endl;

        mdata.close();
    }
}

uint64_t GetInternedString(string str)
{
    assert(str != "" && "Not allowed to intern an empty string.");

    if (g_internTable.find(str) == g_internTable.end())
    {
        uint64_t interncount = g_internTable.size();
        g_internTable.insert(pair<string, uint64_t>(str, ++interncount));
    }

    return g_internTable[str];
}

uint64_t GetFunctionID(Function* function)
{
    if (!function) return 0;

    if (g_functionids.find(function) == g_functionids.end())
    {
        uint64_t functioncount = g_functionids.size();
        g_functionids.insert(pair<Function*, uint64_t>(function, ++functioncount));
    }

    return g_functionids[function];
}

uint64_t GetLoopID(Loop* loop)
{
    if (!loop) return 0;

    if (g_loopids.find(loop) == g_loopids.end())
    {
        uint64_t loopcount = g_loopids.size();
        g_loopids.insert(pair<Loop*, uint64_t>(loop, ++loopcount));
    }

    return g_loopids[loop];
}

uint64_t GetLoadID(LoadInst* load)
{
    if (!load) return 0;

    if (g_loadids.find(load) == g_loadids.end())
    {
        uint64_t loadcount = g_loadids.size();
        g_loadids.insert(pair<LoadInst*, uint64_t>(load, ++loadcount));
    }

    return g_loadids[load];
}

uint64_t GetStoreID(StoreInst* store)
{
    if (!store) return 0;

    if (g_storeids.find(store) == g_storeids.end())
    {
        uint64_t storecount = g_storeids.size();
        g_storeids.insert(pair<StoreInst*, uint64_t>(store, ++storecount));
    }

    return g_storeids[store];
}

string GetFunctionMetadata(Function* function)
{
    assert(NULL != function);

    assert(g_functionids.find(function) != g_functionids.end() && "Invalid function id.");

    stringstream sstr;    sstr << "( "
    << g_functionids[function] << ", "
    << function->getNameStr()  << " )\n";

    return sstr.str();
}

string GetLoopMetadata(Loop* loop)
{
    assert(NULL != loop);

    BasicBlock* header = loop->getHeader();
    Function*   parent = header->getParent();

    assert(g_loopids.find(loop)       != g_loopids.end()     && "Invalid loop."    );
    assert(g_functionids.find(parent) != g_functionids.end() && "Invalid function.");

    stringstream sstr; sstr << "( "
    << g_loopids[loop]      << ", "
    << header->getNameStr() << " )\n";

    return sstr.str();
}

