#ifndef DEFAULTBRANCHSTRATEGY_H
#define DEFAULTBRANCHSTRATEGY_H

#include "branchstrategy.h"
#include "utils.h"

namespace TRANSPLAN
{

    class DefaultStrategy : public BranchStrategy
    {
    public:

        /*
          Idea here is to choose a truck which has
          1. minimum number next options
          2. minimum est 
         */

        DefaultStrategy()
        : BranchStrategy()
        {
        }

        //// returns 0 when no branching possible, 1-when state variable branching and 2- when resource branching

        BRANCH_TYPE getNextDecision(SearchState* state, IntPair& curr_ass)
        {
            return TRANSPLAN::BR_NONE;

        }
    };
}
#endif
