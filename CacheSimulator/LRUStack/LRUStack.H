/// Puyan Lotfi
/// puyan@gatech.edu
/// LRUStack implementation (naive).

/// Includes:

//#include <map>
#include <vector>
#include <utility>
#include <sstream>
#include <cassert>
#include <iostream>
#include <stdint.h>

#include "IntervalTree.H"

#ifdef _WIN32

#include <unordered_map>

#else

#include <tr1/unordered_map>

#endif

using namespace std;
using namespace std::tr1;



using namespace std;

#define DEBUG 0
#if DEBUG
#undef debug
#endif

#if DEBUG
#define MSG(msg) do { cout << msg; } while (0)
#else
#define MSG(msg) do {              } while (0)
#endif


class MemoryReference
{
    private:

        MemoryReference() { }

    public:

        MemoryReference(int64_t referenceBlockAddress, int64_t logicalTime):

            m_lastLogicalHibernateInterval(0),
            m_referenceBlockAddress(referenceBlockAddress),
            m_lastLogicalAccessTime(logicalTime),
            m_lastReuseDistance(0),
            m_accessCount(1),

            m_wasLastAccessDuringHibernate(false),
            m_isFirstCompulsoryAccess(true)
        {

        }

        virtual ~MemoryReference()
        {

        }

    public:

        int64_t m_lastLogicalHibernateInterval;
        int64_t m_referenceBlockAddress;
        int64_t m_lastLogicalAccessTime;
        int64_t m_lastReuseDistance;
        int64_t m_accessCount;

        bool    m_wasLastAccessDuringHibernate;
        bool    m_isFirstCompulsoryAccess;
};

class LRUStack
{
    public:
        LRUStack(): m_stackcount(0), m_infinity(0) { }
        virtual ~LRUStack() { }

        MemoryReference* SearchAndCount(int64_t referenceBlockAddress, int64_t currentLogicalHibernateInterval, bool isHibernating);

        void   Update(MemoryReference* memoryReference);
        void   rehash(size_t nbuckets);
        string DumpHistogram() const;



    private:
        std::tr1::unordered_map<int64_t, MemoryReference*> m_lrustack;
        std::tr1::unordered_map<int64_t, int64_t>          m_histogram;

        int64_t m_stackcount;
        int64_t m_infinity; // compulsory misses

        mutable IntervalTree m_holes;
};


void LRUStack::rehash(size_t nbuckets)
{
    m_lrustack.rehash(nbuckets);
    m_histogram.rehash(nbuckets);
}

MemoryReference* LRUStack::SearchAndCount(
        int64_t referenceBlockAddress,
        int64_t currentLogicalHibernateInterval,
        bool    isHibernating)
{
    assert(0 != referenceBlockAddress);

    MemoryReference* lruentry = NULL;
    auto lruiter = m_lrustack.find(referenceBlockAddress);

    if (lruiter == m_lrustack.end())
    {
        // This is a new reference not in the reuse "stack."

        lruentry = new MemoryReference(referenceBlockAddress, m_stackcount);

        // Initialize info about hibernate and sampling.
        lruentry->m_wasLastAccessDuringHibernate = isHibernating;
        lruentry->m_lastLogicalHibernateInterval = currentLogicalHibernateInterval;

        m_lrustack.insert(pair<int64_t, MemoryReference*>(referenceBlockAddress, lruentry));

        // Always sample new references.
        return lruentry;
    }
    else // This is a reuse from the last count
    {
        lruentry = lruiter->second;

        // Set whether we want to sample
        // for this references or not.
        bool doNotSample =
            isHibernating &&
            lruentry->m_wasLastAccessDuringHibernate &&
            lruentry->m_lastLogicalHibernateInterval ==
            currentLogicalHibernateInterval;

        // Keep track of this iterator to
        // make efficient inserts and lookups.
        IntervalTree::iterator currentWorkingSubtree;

        int64_t lastLogicalAccessTime   = lruentry->m_lastLogicalAccessTime;
        if (!doNotSample)
        {
            int64_t totalHolesForAccessTime =
                m_holes.GetHoleCount(
                        lruentry->m_lastLogicalAccessTime,
                        currentWorkingSubtree);

            // Calculate the new reuse distance
            // from last access time and holes.
            lruentry->m_lastReuseDistance =
                m_stackcount -
                lastLogicalAccessTime -
                totalHolesForAccessTime;

            // Update Interval Tree of Holes
            m_holes.insert(lastLogicalAccessTime, currentWorkingSubtree);
        }

        // Update fields for this references
        // regardless of if we are sampling.
        lruentry->m_accessCount++;
        lruentry->m_isFirstCompulsoryAccess      = false;
        lruentry->m_lastLogicalAccessTime        = m_stackcount;
        lruentry->m_wasLastAccessDuringHibernate = isHibernating;
        lruentry->m_lastLogicalHibernateInterval = currentLogicalHibernateInterval;

        // Sample or Not
        return doNotSample ? NULL : lruentry;
    }
}

void LRUStack::Update(MemoryReference* memoryReference)
{
    assert(NULL != memoryReference);
    assert(0 != memoryReference->m_referenceBlockAddress);

    if (memoryReference->m_isFirstCompulsoryAccess)
    {
        m_infinity++;
    }
    else
    {
        int64_t lastReuseDistance = memoryReference->m_lastReuseDistance;

        auto result = m_histogram.insert(pair<int64_t, int64_t>(lastReuseDistance, 1));

        if (!result.second)
        {
            result.first->second++;
        }
    }

    m_stackcount++;
}

string LRUStack::DumpHistogram() const
{
    stringstream sstr;

    cout << "Histogram Size: " << m_histogram.size() << endl;
    cout << "Reuse table Size: " << m_lrustack.size() << endl;
    cout << "Holes Size: " << m_holes.size() << endl;

    map<int64_t, int64_t> histogram;

    //    m_holes.DumpTree();
    /*
    cout << "Dumping memory hash table: " << endl;
    for (auto iter = m_lrustack.begin(); iter != m_lrustack.end(); iter++)
    {
    if (iter->second->linenum > 0)
    cout << "addr: " << iter->second->m_referenceBlockAddress << " linenum: " << iter->second->linenum << endl;
    }
    */

    for (auto iter  = m_histogram.begin();
              iter != m_histogram.end();
              iter ++)
    {
        histogram.insert(pair<int64_t, int64_t>(iter->first, iter->second));
    }

    for (auto iter  = histogram.begin();
              iter != histogram.end();
              iter ++)
    {
        sstr << iter->first << "," << iter->second << "\n";
    }

    sstr << "infinity" << "," <<  m_infinity << "\n";

    return sstr.str();
}

