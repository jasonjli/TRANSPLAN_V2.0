#include "search.h"
#include "searchstate.h"
#include "cspintvar.h"
#include "cspintervalvar.h"
#include "cspboolvar.h"
#include "cspauxsetvar.h"
#include "cspauxintvar.h"
#include "cspmaxcountervar.h"
#include "cspauxsetmonotonicincvar.h"
#include "cspauxsetmonotonicdecvar.h"
#include "cspgraphvar.h"
#include "propagator.h"
#include "inferator.h"

namespace TRANSPLAN
{

	SearchState::SearchState(Search& search) :
		search(search)
	{
	}

	SearchState::SearchState(const SearchState& state) :
		search(state.search),
		integerVariables(state.integerVariables),
		intervalVariables(state.intervalVariables),
		booleanVariables(state.booleanVariables),
		maxCounterVariables(state.maxCounterVariables),
		auxSetVariables(state.auxSetVariables),
		auxIntVariables(state.auxIntVariables),
		auxSetMonoIncVariables(state.auxSetMonoIncVariables),
		auxSetMonoDecVariables(state.auxSetMonoDecVariables),
		graphVariables(state.graphVariables)
	{
		this->stateID = state.stateID;
	}

	SearchState::~SearchState()
	{
	}

	SearchState& SearchState::operator=(const SearchState& state)
	{
		this->stateID = state.stateID;
		this->search = state.search;
		this->integerVariables = state.integerVariables;
		this->intervalVariables = state.intervalVariables;
		this->booleanVariables = state.booleanVariables;
		this->maxCounterVariables = state.maxCounterVariables;
		this->auxSetVariables = state.auxSetVariables;
		this->auxIntVariables = state.auxIntVariables;
		this->auxSetMonoIncVariables = state.auxSetMonoIncVariables;
		this->auxSetMonoDecVariables = state.auxSetMonoDecVariables;
		this->graphVariables = state.graphVariables;

		return *this;
	}

	int SearchState::newCSPIntVar(IntSet dom)
	{
		integerVariables.push_back(CSPIntVar(search.integerVariableCount, dom, this));
		return search.integerVariableCount++;
	}

	int SearchState::newCSPIntervalVar(IntPair range)
	{
		intervalVariables.push_back(CSPIntervalVar(search.intervalVariableCount, range, this));
		return search.intervalVariableCount++;

	}

	int SearchState::newCSPBoolVar()
	{
		booleanVariables.push_back(CSPBoolVar(search.boolVariableCount, this));
		return search.boolVariableCount++;
	}

	int SearchState::newCSPAuxMonoDecSetVar(IntSet set)
	{
		auxSetMonoDecVariables.push_back(CSPAuxSetMonoDecVar(search.auxSetMonoDecVariableCount, set, this));
		return search.auxSetMonoDecVariableCount++;

	}

	int SearchState::newCSPAuxMonoIncSetVar(IntSet set)
	{
		auxSetMonoIncVariables.push_back(CSPAuxSetMonoIncVar(search.auxSetMonoIncVariableCount, set, this));
		return search.auxSetMonoIncVariableCount++;

	}

	int SearchState::newCSPAuxSetVar(IntSet set)
	{
		auxSetVariables.push_back(CSPAuxSetVar(search.auxSetVariableCount, set, this));
		return search.auxSetVariableCount++;

	}

	int SearchState::newCSPAuxIntVar(int init_val)
	{
		auxIntVariables.push_back(CSPAuxIntVar(search.auxIntVariableCount, init_val, this));
		return search.auxIntVariableCount++;
	}

	int SearchState::newCSPMaxCounterVar(int upperBound, IntSet set)
	{
		maxCounterVariables.push_back(CSPMaxCounterVar(search.maxCounterVariableCount, upperBound, set, this));
		return search.maxCounterVariableCount++;
	}

	int SearchState::newCSPGraphVar(int numberOfNodes)
	{
		graphVariables.push_back(CSPGraphVar(search.graphVariableCount, numberOfNodes, this));
		return search.graphVariableCount++;
	}

	CSPIntVar& SearchState::getCSPIntVar(int varIndex)
	{
		return integerVariables[varIndex];
	}

	CSPIntervalVar& SearchState::getCSPIntervalVar(int varIndex)
	{
		return intervalVariables[varIndex];
	}

	CSPBoolVar& SearchState::getCSPBoolVar(int varIndex)
	{
		return booleanVariables[varIndex];
	}

	CSPAuxIntVar& SearchState::getCSPAuxIntVar(int varIndex)
	{
		return auxIntVariables[varIndex];
	}

	CSPAuxSetVar& SearchState::getCSPAuxSetVar(int varIndex)
	{
		return auxSetVariables[varIndex];
	}

	CSPAuxSetMonoDecVar& SearchState::getCSPAuxSetMonoDecVar(int varIndex)
	{
		return auxSetMonoDecVariables[varIndex];
	}

	CSPAuxSetMonoIncVar& SearchState::getCSPAuxSetMonoIncVar(int varIndex)
	{
		return auxSetMonoIncVariables[varIndex];
	}

	CSPMaxCounterVar& SearchState::getCSPMaxCounterVar(int varIndex)
	{
		return maxCounterVariables[varIndex];
	}

	CSPGraphVar& SearchState::getCSPGraphVar(int varIndex)
	{
		return graphVariables[varIndex];
	}

	bool SearchState::propagate()
	{
		while (!this->propagators.empty() || !this->inferators.empty())
		{
			while (!this->propagators.empty())
			{
				int propIndex = popPropagatorIndex();

				if (search.getPropagator(propIndex)->propagate(this) == TRANSPLAN::FAIL)
				{
					return false;
				}

			}

			while (!this->inferators.empty())
			{
				int inferIndex = popInferatorIndex();

				if (search.getInferator(inferIndex)->infer(this) == TRANSPLAN::FAIL)
				{
					return false;
				}

			}
		}

		return true;
	}

	int SearchState::popInferatorIndex()
	{
		int inferIndex = this->inferators.top();
		this->inferators.pop();
		activeInferators[inferIndex] = false;

		return inferIndex;
	}

	int SearchState::popPropagatorIndex()
	{
		int propIndex = this->propagators.top();
		this->propagators.pop();
		activePropagators[propIndex] = false;

		return propIndex;
	}


	void SearchState::initializeInferators(int n_prop)
	{
		this->activePropagators = BoolVector(n_prop, false);
	}

	void SearchState::initializePropagators(int n_infer)
	{
		this->activeInferators = BoolVector(n_infer, false);
	}

	void SearchState::activatePropagator(int propagatorIndex)
	{
		if (!this->activePropagators[propagatorIndex])
		{
			this->activePropagators[propagatorIndex] = true;
			this->propagators.push(propagatorIndex);
		}
	}

	void SearchState::activateInferator(int inferatorIndex)
	{
		if (!this->activeInferators[inferatorIndex])
		{
			this->activeInferators[inferatorIndex] = true;
			this->inferators.push(inferatorIndex);
		}
	}

}
