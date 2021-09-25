#include <map>
#include <cassert>
#include <stdint.h>
#include <iostream>

using namespace std;

struct IntervalMetadata
{
    IntervalMetadata(): m_valid(false), m_rstweight(0), m_li_cached_number(0) { }
    bool    m_valid;
    int64_t m_rstweight; // right subtree
    int64_t m_li_cached_number;
};

class IntervalNode
{
    private:

        IntervalNode() { }

    public:

        enum kind { value_t, interval_t };

        IntervalNode(
            int64_t value):
            m_kind(value_t),
            m_value(value),
            m_start(0),
            m_end(0)   { }

        IntervalNode(
            int64_t start,
            int64_t end):
            m_kind(interval_t),
            m_value(0),
            m_start(start),
            m_end(end) { }


        bool operator ==(const IntervalNode& rhs) const
        {
            assert(m_kind == rhs.m_kind);

            if (m_kind == value_t)
            {
                return m_value == rhs.m_value;
            }
            else if (m_kind == interval_t)
            {
                return m_start == rhs.m_start &&
                       m_end   == rhs.m_end;
            }
            else
            {
                return false;
            }
        }


        bool IsAdjacent(const IntervalNode& rhs) const
        {
            assert(m_kind     == interval_t);
            assert(rhs.m_kind == interval_t);

            assert(rhs.m_start <= rhs.m_end);
            assert(m_start     <= m_end);

            return m_end == rhs.m_start - 1;
        }


        kind    m_kind;
        int64_t m_value;
        int64_t m_start;
        int64_t m_end;
};

struct IntervalNodeComparitor
{
    bool operator() (const IntervalNode& lhs, const IntervalNode& rhs) const
    {
        if (lhs.m_kind == IntervalNode::value_t &&
            rhs.m_kind == IntervalNode::interval_t)
        {
            assert(rhs.m_start <= rhs.m_end);
            return lhs.m_value < rhs.m_start;
        }
        else
        if (lhs.m_kind == IntervalNode::interval_t &&
            rhs.m_kind == IntervalNode::value_t)
        {
            assert(lhs.m_start <= lhs.m_end);
            return lhs.m_end   < rhs.m_value;
        }
        else
        if (lhs.m_kind == IntervalNode::interval_t &&
            rhs.m_kind == IntervalNode::interval_t)
        {
            assert(rhs.m_start <= rhs.m_end);
            assert(lhs.m_start <= lhs.m_end);

            /*
            if (!(lhs.m_end < rhs.m_start ||
                   rhs.m_end < lhs.m_start ||
                   (lhs.m_start == rhs.m_start && lhs.m_end != rhs.m_end)  ||
                   (lhs.m_end == rhs.m_end && lhs.m_start != rhs.m_start)  ||
                   lhs == rhs))
            {
                cout << "lhs start: " << lhs.m_start << endl;
                cout << "rhs start: " << rhs.m_start << endl;
                cout << "lhs end: " << lhs.m_end << endl;
                cout << "rhs end: " << rhs.m_end << endl;

            }
            */

            assert(lhs.m_end < rhs.m_start ||
                   rhs.m_end < lhs.m_start ||
                   (lhs.m_start == rhs.m_start && lhs.m_end != rhs.m_end)  ||
                   (lhs.m_end == rhs.m_end && lhs.m_start != rhs.m_start)  ||
                   (lhs.m_end == rhs.m_start && lhs.m_start != rhs.m_end)  ||
                   (rhs.m_end == lhs.m_start && rhs.m_start != lhs.m_end)  ||
                   (lhs.m_end == lhs.m_start)  ||
                   (rhs.m_end == rhs.m_start)  ||
                   lhs == rhs);

            if (lhs.m_start == rhs.m_start && lhs.m_end != rhs.m_end)
                return lhs.m_end < rhs.m_end;


            return lhs.m_end < rhs.m_start;
        }
        else
        {
            cout << lhs.m_kind << " " << lhs.m_value << " " << lhs.m_start << " " << lhs.m_end << endl;
            cout << rhs.m_kind << " " << rhs.m_value << " " << rhs.m_start << " " << rhs.m_end << endl;
            assert(false &&
                   "< operator on intervalnode must compare two different kinds.");
        }
    }
};

class IntervalTree
{
    private:

        typedef map<IntervalNode, IntervalMetadata, IntervalNodeComparitor> IntervalTreeImpl;
        typedef pair<IntervalNode, IntervalMetadata> IntervalTreeEntry;

    public:

        IntervalTree(): m_lowest_logical_accesstime(0) { }

        typedef IntervalTreeImpl::iterator iterator;

        void insert(int64_t i, IntervalTreeImpl::iterator& currentWorkingSubtree)
        {
            auto cwst = currentWorkingSubtree;

            if (m_lowest_logical_accesstime <= i && currentWorkingSubtree->second.m_valid)
            {
                if (!(cwst->first.m_start <= i && i <= cwst->first.m_end))
                    m_lowest_logical_accesstime  = i;
            }

            auto inserted = m_rbtree.insert(cwst,
                    IntervalTreeEntry(
                        IntervalNode(i, i),
                        IntervalMetadata()));

            // if (!inserted.second) return;

            // Collapse adjacent intervals if possible.
            auto less  = inserted;
            auto equal = inserted;
            auto more  = inserted;
            less--;
            more++;

            if (less  != m_rbtree.end()  &&
                equal != m_rbtree.end()  &&
                more  != m_rbtree.end()  &&
                less->first.IsAdjacent(equal->first) &&
                equal->first.IsAdjacent(more->first))
            {
                int64_t less_start  = less->first.m_start;
                int64_t more_end    = more->first.m_end;

                m_rbtree.erase(equal);
                m_rbtree.erase(more);

                m_rbtree.insert(less,
                        IntervalTreeEntry(
                            IntervalNode(less_start, more_end),
                            IntervalMetadata()));

                m_rbtree.erase(less);
            }
            else
            if (less  != m_rbtree.end()  &&
                equal != m_rbtree.end()  &&
                less->first.IsAdjacent(equal->first))
            {
                int64_t less_start  = less->first.m_start;
                int64_t equal_end   = equal->first.m_end;

                m_rbtree.erase(equal);

                m_rbtree.insert(less,
                        IntervalTreeEntry(
                            IntervalNode(less_start, equal_end),
                            IntervalMetadata()));

                m_rbtree.erase(less);
            }
            else
            if (equal != m_rbtree.end()  &&
                more  != m_rbtree.end()  &&
                equal->first.IsAdjacent(more->first))
            {
                int64_t equal_start = equal->first.m_start;
                int64_t more_end    = more->first.m_end;

                m_rbtree.erase(more);

                m_rbtree.insert(equal,
                        IntervalTreeEntry(
                            IntervalNode(equal_start, more_end),
                            IntervalMetadata()));

                m_rbtree.erase(equal);
            }
        }

        int64_t GetHoleCount(int64_t i, IntervalTreeImpl::iterator& currentWorkingSubtree)
        {
            int64_t count = 0;

            currentWorkingSubtree = m_rbtree.lower_bound(IntervalNode(i));

            if (currentWorkingSubtree == m_rbtree.end()) return 0;

            if (m_lowest_logical_accesstime <= i && currentWorkingSubtree->second.m_valid)
            {
                // cout << "Using access time cache" << endl;
                return currentWorkingSubtree->second.m_rstweight;
            }

            auto iter = m_rbtree.end();

            // auto iter = m_rbtree.find(IntervalNode(m_lowest_logical_accesstime));
            // if (iter !=  m_rbtree.end()) count = iter->second.m_rstweight;
            // auto iter = m_rbtree.lower_bound(IntervalNode(m_lowest_logical_accesstime));
            // count = iter->second.m_rstweight;

            while (iter != currentWorkingSubtree)
            {
                iter--;

                // cout << count << endl;

                // cout << iter->first.m_start << " - " << iter->first.m_end   << endl;
                if (i <= iter->first.m_start)
                {
                    count += iter->first.m_end - iter->first.m_start + 1;
                    m_lowest_logical_accesstime = iter->first.m_start;
                    iter->second.m_valid = true;
                    iter->second.m_rstweight = count;
                    iter->second.m_li_cached_number = iter->first.m_start;
                }
                else if (iter->first.m_start < i && i <= iter->first.m_end)
                {
                    count += iter->first.m_end - i + 1;
                    m_lowest_logical_accesstime     = iter->first.m_start;
                    iter->second.m_valid            = true;
                    iter->second.m_rstweight        = count;
                    iter->second.m_li_cached_number = iter->first.m_start;
                }
            }

            return count;
        }

        void DumpTree()
        {
            for (auto iter = m_rbtree.begin(); iter != m_rbtree.end(); iter++)
                cout << iter->first.m_start << " - " << iter->first.m_end << endl;
        }

        size_t size() const { return m_rbtree.size(); }

        int64_t                    m_lowest_logical_accesstime;
        IntervalTreeImpl::iterator  m_lowest_cached_rst_iter;

        IntervalTreeImpl m_rbtree;
};

/*
int main()
{
    IntervalTree holes;
    holes.m_lowest_logical_accesstime = 0;

    while(true)
    {
    while(true)
    {

        for (auto iter = holes.m_rbtree.begin(); iter != holes.m_rbtree.end(); iter++)
        {
            cout << iter->first.m_start << " - " << iter->first.m_end   << endl;
        }

        int64_t i;
        cin >> i;

        if (i == 0) break;

        auto holes_position = holes.m_rbtree.begin();
        holes.insert(i, holes_position);

    }

    while(true)
    {
        int64_t i;
        cin >> i;


        if (i == 0) break;

        auto holes_position = holes.m_rbtree.begin();

        cout << "less than count: " << holes.GetHoleCount(i, holes_position) << endl;
    }

}

    return 0;
}
*/
