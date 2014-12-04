#include <sstream>
#include <time.h>

#include "search.h"
#include "searchstate.h"
#include "transplan.h"
#include "defaultBranchStrategy.h"
#include "variablepropagatormanager.h"

namespace TRANSPLAN
{

	Search::Search(BranchStrategy* strategy) :
		stateCount(0)
	{
		///create initial state;
		this->pushState(new SearchState(*this));
		this->initialState = states.top();
		this->strategy = strategy;
		this->varManager = new VariablePropagatorManager(*this);
	}

	Search::~Search()
	{
		//std::cout << "Search is deleted\n";
		delete this->varManager;
	}

	/**
	 Initialize Search
	 */
	void Search::initialize()
	{
		this->varManager->clearCSP();
		this->varManager->createCSP();
		//////////////////////////////
		this->initialState->initializeInferators(this->varManager->propagatorCount);
		this->initialState->initializePropagators(this->varManager->inferatorCount);
		////////////////////////////////////////////////////////////////////////////
		//// post initial decision
	}

	CSP_SOL Search::search()
	{
		while (!this->states.empty())
		{
			SearchState* st = this->popState();
			if (st->propagate())
			{
				if (branch(st) == TRANSPLAN::BR_NONE)
				{
					return TRANSPLAN::SOLVED;
				}
			}

			delete st;
		}
		return TRANSPLAN::NO_SOLUTION;
	}

	void Search::printPlan()
	{
		for (int i = 0; i < Transplan::n_activities; ++i)
		{
			/*
			 if ( VariableManager::a_inplan(i).max() )
			 {

			 std::cout << Transplan::activities[i].name << "["<< VariableManager::a_start(i).min()<<","
			 << VariableManager::a_start(i).max()<<"]\n";


			 }
			 */
		}
	}

	int Search::createStateIndex()
	{
		return Search::stateCount++;
	}

	void Search::pushState(SearchState* state)
	{
		states.push(state);
		state->stateID = stateCount++;
	}

	SearchState* Search::popState()
	{
		SearchState* top = this->states.top();
		this->states.pop();

		return top;
	}

	bool Search::validatePlan()
	{
		return true;
	}

	BRANCH_TYPE Search::branch(SearchState* st)
	{
		IntPair decision;
		TRANSPLAN::BRANCH_TYPE dec = strategy->getNextDecision(st, decision);
		if (dec != TRANSPLAN::BR_NONE)
		{
			SearchState* rightState = createNewSearchState(st);
			////assing var != val into rightState;
			pushState( rightState);

			SearchState* leftState = createNewSearchState(st);
			///assign var = val into leftState;
			pushState(leftState);
		}
		return dec;
	}

	Propagator* Search::getPropagator(int propIndex)
	{
		return this->varManager->propagators[propIndex];
	}

	Inferator* Search::getInferator(int inferIndex)
	{
		return this->varManager->inferators[inferIndex];
	}

	SearchState* Search::createNewSearchState(SearchState* clonethis)
	{
		SearchState* state = new SearchState(*clonethis);
		state->initializeInferators(this->varManager->propagatorCount);
		state->initializePropagators(this->varManager->inferatorCount);

		return state;
	}

}
