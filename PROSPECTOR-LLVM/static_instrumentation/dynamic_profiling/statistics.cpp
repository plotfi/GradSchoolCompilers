#include "statistics.h"
#include "../common.h"

vector <string>         g_internedStrings(1);

vector <CodeStatistic*> g_loopStatistics(1);
vector <CodeStatistic*> g_loadStatistics(1);
vector <CodeStatistic*> g_storeStatistics(1);
vector <CodeStatistic*> g_functionStatistics(1);

extern volatile uint64_t loadCount;
extern volatile uint64_t storeCount;

struct OnProgramStartAndEnd
{
    // OnProgramStart
    OnProgramStartAndEnd()
    {
        LoadCSVMetadata();

        cout << "DONE READING CSV..." << endl;
    }

    // OnProgramEnd
    ~OnProgramStartAndEnd()
    {
        cout << endl;
        cout << endl;
        DumpLoops();
        cout << endl;
        DumpFunctions();
        cout << endl;
        DumpMemoryAccesses();

        cout << "Load Count:  " << loadCount << endl;
        cout << "Store Count: " << storeCount << endl;
    }
};

OnProgramStartAndEnd onProgramStartAndEnd;


void DumpMemoryAccesses()
{
    ofstream results;
    results.open("memory_results.csv");

    results
        << "Memory Access Statistics, \n"
        << "id, location \n";

    for (int i = 1; i < g_loadStatistics.size(); i++)
    {
        CodeStatistic* stat = g_loadStatistics[i];

        if (stat && stat->kind == LOAD_STAT)
        {
            results
                << "load, "
                << stat->u.LoadStat.m_id                          << ", "
                //<< interns[stat->u.LoopStat.m_dir   ]           << ""
                << g_internedStrings[stat->u.LoopStat.m_file  ]   << ":"
                << stat->u.LoadStat.m_line                        << ":"
                << stat->u.LoadStat.m_column                      << ", "
                << "\n";
        }
    }

    for (int i = 1; i < g_storeStatistics.size(); i++)
    {
        CodeStatistic* stat = g_storeStatistics[i];

        if (stat && stat->kind == STORE_STAT)
        {
            results
                << "store, "
                << stat->u.StoreStat.m_id                        << ", "
                //<< interns[stat->u.StoreStat.m_dir   ]         << ""
                << g_internedStrings[stat->u.StoreStat.m_file  ] << ":"
                << stat->u.StoreStat.m_line                      << ":"
                << stat->u.StoreStat.m_column                    << ", "
                << "\n";
        }
    }

    results.close();

}

void DumpLoops()
{
    ofstream results;
    results.open("loop_results.csv");


    results
    << "Loop Statistics, \n"
    << "id, location, header, parent id, function name, weight, iterations, entries, exits\n";

    for (int i = 1; i < g_loopStatistics.size(); i++)
    {
        CodeStatistic* stat = g_loopStatistics[i];

        if (stat && stat->kind == LOOP_STAT)
        {
            results
            << stat->u.LoopStat.m_id        << ", "
            //<< interns[stat->u.LoopStat.m_dir   ]           << ""
            << g_internedStrings[stat->u.LoopStat.m_file  ]   << ":"
            << stat->u.LoopStat.m_line                        << ":"
            << stat->u.LoopStat.m_column                      << ", "
            << g_internedStrings[stat->u.LoopStat.m_bbname]   << ", "
            << stat->u.LoopStat.m_parentloop                  << ", "
            << g_internedStrings[
               g_functionStatistics[
               stat->u.LoopStat.m_parentfunction]->
               u.FunctionStat.m_name]                         << ", "
            << stat->u.LoopStat.m_weight                      << ", "
            << stat->u.LoopStat.m_tripCount                   << ", "
            << stat->u.LoopStat.m_entryCount                  << ", "
            << stat->u.LoopStat.m_exitCount                   << ", "
            << "\n";
        }
    }

    results.close();
}

void DumpFunctions()
{
    ofstream results;
    results.open("function_results.csv");

   results
   << "Function Statistics, \n"
   << "id, location, name, weight, entries, exits\n";

    for (int i = 1; i < g_functionStatistics.size(); i++)
    {
        CodeStatistic* stat = g_functionStatistics[i];

        if (stat && stat->kind == FUNCTION_STAT)
        {
            results
            << stat->u.FunctionStat.m_id  << ", "
            //<< interns[stat->u.FunctionStat.m_dir ]         << ""
            << g_internedStrings[stat->u.FunctionStat.m_file] << ":"
            << stat->u.FunctionStat.m_line                    << ":"
            << stat->u.FunctionStat.m_column                  << ", "
            << g_internedStrings[stat->u.FunctionStat.m_name] << ", "
            << stat->u.FunctionStat.m_weight                  << ", "
            << stat->u.FunctionStat.m_entryCount              << ", "
            << stat->u.FunctionStat.m_exitCount
            << "\n";
        }
    }

    results.close();
}

void DumpInterns()
{
    for (int i = 1; i < g_internedStrings.size() + 1; i++)
        cout << i << ":" << g_internedStrings[i] << endl;
}

void LoadCSVMetadata()
{
    ifstream mdata;
    mdata.open(MDATAFILE);

    string newline;

    g_functionStatistics[0] = NULL;
    g_loopStatistics[0]     = NULL;
    g_loadStatistics[0]     = NULL;
    g_storeStatistics[0]    = NULL;
    g_internedStrings[0]    = "";

    while (std::getline(mdata, newline))
    {
        stringstream sstr; sstr << newline;
        string cell;
        map<int, string> commaSepStrs;

        for (int i = 0; std::getline(sstr, cell, ','); i++)
        {
            commaSepStrs.insert(pair<int, string>(i, cell));
        }

        if (commaSepStrs[0] == MDATALOOP)
        {
            CodeStatistic* stat = new CodeStatistic();
            stat->kind = LOOP_STAT;
            int i = 0; i++;
            stat->u.LoopStat.m_id             = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.LoopStat.m_dir            = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.LoopStat.m_file           = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.LoopStat.m_line           = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.LoopStat.m_column         = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.LoopStat.m_bbname         = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.LoopStat.m_parentloop     = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.LoopStat.m_parentfunction = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.LoopStat.m_weight         = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            if (i > commaSepStrs.size()) assert(false && "Malformed comma separated values");

            assert(!errno && "Malformed csv");

            uint64_t id = stat->u.LoopStat.m_id;

            if (g_loopStatistics.size() < id + 1)
                g_loopStatistics.resize(id + 1);

            g_loopStatistics[id] = stat;
        }
        else if (commaSepStrs[0] == MDATAFUNC)
        {
            CodeStatistic* stat = new CodeStatistic();
            stat->kind = FUNCTION_STAT;
            int i = 0; i++;
            stat->u.FunctionStat.m_id     = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.FunctionStat.m_dir    = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.FunctionStat.m_file   = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.FunctionStat.m_line   = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.FunctionStat.m_column = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.FunctionStat.m_name   = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.FunctionStat.m_weight = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            if (i > commaSepStrs.size()) assert(false && "Malformed comma separated values");

            if (errno)
                cerr << newline << endl;

            assert(!errno && "Malformed csv");

            uint64_t id = stat->u.FunctionStat.m_id;

            if (g_functionStatistics.size() < id + 1)
                g_functionStatistics.resize(id + 1);

            g_functionStatistics[id] = stat;
        }
        else if (commaSepStrs[0] == MDATALOAD)
        {
            CodeStatistic* stat = new CodeStatistic();
            stat->kind = LOAD_STAT;
            int i = 0; i++;
            stat->u.LoadStat.m_id      = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.LoadStat.m_dir     = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.LoadStat.m_file    = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.LoadStat.m_line    = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.LoadStat.m_column  = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            if (i > commaSepStrs.size()) assert(false && "Malformed comma separated values");

            assert(!errno && "Malformed csv");

            uint64_t id = stat->u.LoadStat.m_id;

            if (g_loadStatistics.size() < id + 1)
                g_loadStatistics.resize(id + 1);

            g_loadStatistics[id] = stat;
        }
        else if (commaSepStrs[0] == MDATASTORE)
        {
            CodeStatistic* stat = new CodeStatistic();
            stat->kind = STORE_STAT;
            int i = 0; i++;
            stat->u.StoreStat.m_id      = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.StoreStat.m_dir     = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.StoreStat.m_file    = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.StoreStat.m_line    = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            stat->u.StoreStat.m_column  = strtoll(commaSepStrs[i].c_str(), (char **)NULL, 10); i++;
            if (i > commaSepStrs.size()) assert(false && "Malformed comma separated values");

            assert(!errno && "Malformed csv");

            uint64_t id = stat->u.StoreStat.m_id;

            if (g_storeStatistics.size() < id + 1)
                g_storeStatistics.resize(id + 1);

            g_storeStatistics[id] = stat;
        }
        else if (commaSepStrs[0] == MDATAINTR && commaSepStrs.size() == 3)
        {
            uint64_t id  = strtoll(commaSepStrs[1].c_str(), (char **)NULL, 10);
            string   str =         commaSepStrs[2];

            if (g_internedStrings.size() < id + 1)
                g_internedStrings.resize(id + 1);

            g_internedStrings[id] = str;
        }
        else
        {
            assert(false && "Malformed comma separated values");
        }

    }
}

extern "C" double GetTimeSinceEpochAsDouble()
{
    struct timeval  tv;
    struct timezone tz;

    tz.tz_minuteswest = 0;
    tz.tz_dsttime     = 0;

    gettimeofday(&tv, &tz);

    return (((double)tv.tv_sec) + (((double)tv.tv_usec) / 1000000.0F));
}

extern "C" void ProvideFunctionStats(uint64_t functionid, FlowDirection direction)
{
    assert(functionid < g_functionStatistics.size());
    assert(NULL != g_functionStatistics[functionid]);
    assert(FUNCTION_STAT == g_functionStatistics[functionid]->kind);

    switch (direction)
    {
        case ENTRY:
        {
            g_functionStatistics[functionid]->u.FunctionStat.m_entryCount++;

            break;
        }
        case EXIT:
        {
            g_functionStatistics[functionid]->u.FunctionStat.m_exitCount++;

            break;
        }
        default:
            assert(false &&
                   "Provided function statistic must be  "
                   "either for a function entry, or exit.");
    }
}

/**
 * This function adds information about the runtime behavior of
 * loop exits, iterations, and entrances.
 *
 */
extern "C" void ProvideLoopStats(uint64_t loopid, FlowDirection direction)
{
    assert(loopid < g_loopStatistics.size());
    assert(NULL != g_loopStatistics[loopid]);
    assert(LOOP_STAT == g_loopStatistics[loopid]->kind);

    switch (direction)
    {
        case ENTRY:
        {
            g_loopStatistics[loopid]->u.LoopStat.m_entryCount++;
            g_loopStatistics[loopid]->u.LoopStat.m_isInside = 1;

            //g_loopStatistics[loopid]->u.LoopStat.m_entryTime = GetTimeSinceEpochAsDouble();
            break;
        }
        case ITER:
        {
            g_loopStatistics[loopid]->u.LoopStat.m_tripCount++;
            g_loopStatistics[loopid]->u.LoopStat.m_isInside = 1;
            break;
        }
        case EXIT:
        {
            if (g_loopStatistics[loopid]->u.LoopStat.m_isInside)
            {
                g_loopStatistics[loopid]->u.LoopStat.m_hasNormalExit = true;
                g_loopStatistics[loopid]->u.LoopStat.m_exitCount++;
            }

            g_loopStatistics[loopid]->u.LoopStat.m_isInside = 0;

            /*
            if (g_loopStatistics[loopid]->u.LoopStat.m_entryTime)
            {
                g_loopStatistics[loopid]->u.LoopStat.m_exitTime  = GetTimeSinceEpochAsDouble();

                g_loopStatistics[loopid]->u.LoopStat.m_duration +=
                g_loopStatistics[loopid]->u.LoopStat.m_exitTime -
                g_loopStatistics[loopid]->u.LoopStat.m_entryTime;

                g_loopStatistics[loopid]->u.LoopStat.m_exitTime  = 0;
                g_loopStatistics[loopid]->u.LoopStat.m_entryTime = 0;
            }
            */
            break;
        }
        default:
            assert(false &&
                   "Provided loop statistic must be either "
                   "for a loop entry, iterate, or exit.    ");
    }
}


uint64_t GetParentFunctionID(uint64_t loopid)
{
    assert(loopid     < g_loopStatistics.size());
    assert(NULL      != g_loopStatistics[loopid]);
    assert(LOOP_STAT == g_loopStatistics[loopid]->kind);

    return g_loopStatistics[loopid]->u.LoopStat.m_parentfunction;
}

