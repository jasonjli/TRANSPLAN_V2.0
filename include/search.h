#ifndef SEARCH_H_
#define SEARCH_H_


#include <vector>
#include <set>

#include "constants.h"

namespace TRANSPLAN
{
    class SearchState;
    class BranchStrategy;
    class VariablePropagatorManager;
    class Propagator;
    class Inferator;

    class Search
    {
    private:
        int                          stateCount;
        std::stack<SearchState*>     states;
        SearchState*                 initialState;
        BranchStrategy*              strategy;
        VariablePropagatorManager*   varManager;
        
        BRANCH_TYPE  branch(SearchState* state);
        int          createStateIndex();
        void         pushState(SearchState* state);
        SearchState* popState();
        SearchState* createNewSearchState(SearchState* clonethis);

    public:
        int totalVariableCount;
        int integerVariableCount;
        int intervalVariableCount;
        int boolVariableCount;
        int maxCounterVariableCount;
        int auxSetVariableCount;
        int auxSetMonoIncVariableCount;
        int auxSetMonoDecVariableCount;
        int auxIntVariableCount;
        int graphVariableCount;


        Search(BranchStrategy* strategy);
        ~Search();

        SearchState* getInitialState()
        {
            return this->initialState;
        }

        Propagator* getPropagator(int propIndex);
        Inferator* getInferator(int inferIndex);

        void    initialize();
        CSP_SOL search();
        bool    validatePlan();
        void    printStatistics();
        void    printPlan();
    };

}

#endif
