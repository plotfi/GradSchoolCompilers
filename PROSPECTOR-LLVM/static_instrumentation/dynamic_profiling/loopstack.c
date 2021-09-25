#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include "statistics.h"

#define MAX_LOOP_STACK_SIZE 1024

static uint64_t g_loopStack[MAX_LOOP_STACK_SIZE];
static unsigned g_loopStackPosition = 0;

struct ProgramStatistics;

#define CheckLoopStack()                                                         \
do {                                                                           \
    if (g_loopStackPosition < 0 || g_loopStackPosition >= MAX_LOOP_STACK_SIZE) \
        printf("%s:%d LOOP STACK INCONSISTENCY: positon = %d\n",               \
               __FILE__, __LINE__, g_loopStackPosition);                       \
} while (0)

void DisplayLoopStack()
{
    int i = 0;
    for (; i <= g_loopStackPosition; i++)
        printf("[%lld]->", g_loopStack[i]);
    printf("//\n");
}

void PushLoopStack(uint64_t loopid)
{
    // printf("pushing loops %lld\n", loopid);
    g_loopStackPosition++;
    CheckLoopStack();
    g_loopStack[g_loopStackPosition] = loopid;
}

void PopLoopStack(uint64_t loopid, uint64_t destLoopId)
{
    while (g_loopStackPosition > 0                    &&
          (g_loopStack[g_loopStackPosition] == loopid  ||
           g_loopStack[g_loopStackPosition] != destLoopId))
    {
        // printf("poping loops %lld\n", loopid);

        uint64_t currloopid = g_loopStack[g_loopStackPosition];

        if (GetParentFunctionID(currloopid) !=
            GetParentFunctionID(loopid)) break;


        g_loopStackPosition--;


        if (g_loopStackPosition <= 0)
        {
            printf("LOOP STACK: just hit bottom.\n");
            break;
        }

        CheckLoopStack();


        if (g_loopStack[g_loopStackPosition] != destLoopId) break;
    }
}

