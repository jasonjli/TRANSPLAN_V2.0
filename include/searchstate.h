/* 
 * File:   searchState.h
 * Author: debdeep
 *
 * Created on November 11, 2014, 7:28 AM
 */

#ifndef SEARCHSTATE_H
#define	SEARCHSTATE_H

#include "constants.h"

namespace TRANSPLAN
{
    class Search;

    class SearchState
    {
    private:

        CSPIntVector           integerVariables;
        CSPIntervalVector      intervalVariables;
        CSPBooleanVector       booleanVariables;
        CSPMaxCounterVector    maxCounterVariables;
        CSPAuxSetVector        auxSetVariables;
        CSPAuxIntVector        auxIntVariables;
        CSPAuxSetMonoIncVector auxSetMonoIncVariables;
        CSPAuxSetMonoDecVector auxSetMonoDecVariables;
        CSPGraphVector         graphVariables;

        BoolVector             activePropagators;
        BoolVector             activeInferators;

        IntStack               propagators;
        IntStack               inferators;
        Search& search;

        int  popPropagatorIndex();
        int  popInferatorIndex();

    public:
        int stateID;

        ~SearchState();

        SearchState(Search& search);

        SearchState(const SearchState& state);

        SearchState& operator=(const SearchState& state);

        int newCSPIntVar(IntSet dom);
        int newCSPIntervalVar(IntPair range);
        int newCSPBoolVar();
        int newCSPAuxMonoDecSetVar(IntSet set);
        int newCSPAuxMonoIncSetVar(IntSet set);
        int newCSPAuxSetVar(IntSet set);
        int newCSPAuxIntVar(int init_val);
        int newCSPMaxCounterVar(int upperBound, IntSet set);
        int newCSPGraphVar(int numberOfNodes);

        void initializePropagators(int n_propagation);
        void initializeInferators(int n_inferator);

        void activatePropagator(int propagatorIndex);
        void activateInferator(int inferatorIndex);


        CSPIntVar&           getCSPIntVar(int varIndex);
        CSPIntervalVar&      getCSPIntervalVar(int varIndex);
        CSPBoolVar&          getCSPBoolVar(int varIndex);
        CSPAuxIntVar&        getCSPAuxIntVar(int varIndex);
        CSPAuxSetVar&        getCSPAuxSetVar(int varIndex);
        CSPAuxSetMonoDecVar& getCSPAuxSetMonoDecVar(int varIndex);
        CSPAuxSetMonoIncVar& getCSPAuxSetMonoIncVar(int varIndex);
        CSPMaxCounterVar&    getCSPMaxCounterVar(int varIndex);
        CSPGraphVar&         getCSPGraphVar(int varIndex);

        bool propagate();


    };
}

#endif	/* SEARCHSTATE_H */

