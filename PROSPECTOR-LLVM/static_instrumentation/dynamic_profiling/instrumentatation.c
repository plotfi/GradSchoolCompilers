// Author: Puyan Lotfi <puyan@gatech.edu>

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "statistics.h"

#undef LOOPSTACK_ENABLED
#define LOOPSTACK_ENABLED 0
#if LOOPSTACK_ENABLED
void PushLoopStack(uint64_t loopid);
void PopLoopStack(uint64_t loopid, uint64_t destLoopId);
#define PUSHLOOPSTACK(loopid) \
    PushLoopStack(loopid)
#define POPLOOPSTACK(loopid, destLoopId) \
    PopLoopStack(loopid, destLoopId)
#else
#define PUSHLOOPSTACK(loopid)            do { } while (0)
#define POPLOOPSTACK(loopid, destLoopId) do { } while (0)
#endif

extern int loopstack_size;

volatile uint64_t loadCount = 0;
volatile uint64_t storeCount = 0;

void OnLoopEntry(uint64_t loopid)
{
    ProvideLoopStats(loopid, ENTRY);
    PUSHLOOPSTACK(loopid);
}

void OnLoopIterate(uint64_t loopid)
{
    ProvideLoopStats(loopid, ITER);
}

void OnLoopExit(uint64_t loopid, uint64_t destLoopId)
{
    ProvideLoopStats(loopid, EXIT);
    POPLOOPSTACK(loopid, destLoopId);
}

void OnFunctionEntry(uint64_t functionid)
{
    ProvideFunctionStats(functionid, ENTRY);
}

void OnFunctionExit(uint64_t functionid)
{
    ProvideFunctionStats(functionid, EXIT);
}

void OnMemoryLoad(uint64_t loadAddress)
{
    loadCount++;
    // printf("Memory Load: %llu\n", loadAddress);
}

void OnMemoryStore(uint64_t storeAddress)
{
    storeCount++;
    // printf("Memory Store: %llu\n", storeAddress);
}

