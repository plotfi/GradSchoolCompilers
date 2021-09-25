#ifndef _STATISTICS_H_
#define _STATISTICS_H_

#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>

#ifndef __cplusplus
typedef char bool;
#include <stdio.h>
#else
#include <map>
#include <list>
#include <cstdio>
#include <vector>
#include <string>
#include <cassert>
#include <fstream>
#include <sstream>
#include <iostream>
using namespace std;
#endif

typedef enum
{
    ENTRY = 0,
    EXIT  = 1,
    ITER  = 2
} FlowDirection;

typedef enum
{
    LOOP_STAT = 0,
    FUNCTION_STAT,
    LOAD_STAT,
    STORE_STAT
} CodeConstruct;

struct CodeStatistic
{
    CodeConstruct kind;

    union
    {
        struct
        {
            // Static Data
            uint64_t  m_id;
            uint64_t  m_dir;
            uint64_t  m_file;
            uint64_t  m_line;
            uint64_t  m_column;
            uint64_t  m_bbname;
            uint64_t  m_parentloop;
            uint64_t  m_parentfunction;

            // Dynamic Data
            uint64_t  m_weight;
            uint64_t  m_isInside;
            uint64_t  m_tripCount;
            uint64_t  m_exitCount;
            uint64_t  m_entryCount;

            // Dynamic Data
            double    m_duration;
            double    m_exitTime;
            double    m_entryTime;

            // Useless Fields
            bool      m_isNaturalLoop;
            bool      m_hasNormalExit;
            bool      m_staticAnalysis;
        } LoopStat;

        struct
        {
            // Static Data
            uint64_t  m_id;
            uint64_t  m_dir;
            uint64_t  m_file;
            uint64_t  m_line;
            uint64_t  m_column;
            uint64_t  m_name;

            // Dynamic Data
            unsigned  m_weight;
            unsigned  m_tripCount;
            unsigned  m_exitCount;
            unsigned  m_entryCount;

            // Dynamic Data
            double    m_duration;
            double    m_exitTime;
            double    m_entryTime;
        } FunctionStat;

        struct
        {
            uint64_t  m_id;
            uint64_t  m_dir;
            uint64_t  m_file;
            uint64_t  m_line;
            uint64_t  m_column;
        } LoadStat;

        struct
        {
            uint64_t  m_id;
            uint64_t  m_dir;
            uint64_t  m_file;
            uint64_t  m_line;
            uint64_t  m_column;
        } StoreStat;

    } u;
};

#ifndef __cplusplus
void     ProvideFunctionStats(uint64_t functionid, FlowDirection direction);
void     ProvideLoopStats(uint64_t loopid, FlowDirection direction);
void     DumpLoops();
void     DumpInterns();
void     DumpFunctions();
void     DumpMemoryAccesses();
void     LoadCSVMetadata();
double   GetTimeSinceEpochAsDouble();
uint64_t GetParentFunctionID(uint64_t loopid);
#else
extern "C" void     ProvideFunctionStats(uint64_t functionid, FlowDirection direction);
extern "C" void     ProvideLoopStats(uint64_t loopid, FlowDirection direction);
extern "C" void     DumpLoops();
extern "C" void     DumpInterns();
extern "C" void     DumpFunctions();
extern "C" void     DumpMemoryAccesses();
extern "C" void     LoadCSVMetadata();
extern "C" double   GetTimeSinceEpochAsDouble();
extern "C" uint64_t GetParentFunctionID(uint64_t loopid);
#endif

#endif // _STATISTICS_H_

