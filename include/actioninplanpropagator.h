/* 
 * File:   actioninplanchangedpropagator.h
 * Author: debdeep
 *
 * Created on 20 November 2014, 6:33 PM
 */

#ifndef ACTIONINPLANCHANGEDPROPAGATOR_H
#define	ACTIONINPLANCHANGEDPROPAGATOR_H

#include "propagator.h"
#include "transplan.h"

namespace TRANSPLAN
{

	class ActionInplanPropagator: public Propagator
	{
		private:

			PROP_STATUS propagateSVTransitions(bool assigned)
			{
				IntVector svTrans = Transplan::activities[actionIndex].sv_trans;

				for (int i = 0; i < svTrans.size(); i++)
				{
					if (assigned)
					{
						IMPLY_EXCL_ON_FAILURE(manager.svt_inplan(currentState, svTrans[i]).eqTrue(), actionIndex);
					}
					else
					{
						IMPLY_EXCL_ON_FAILURE(manager.svt_inplan(currentState, svTrans[i]).eqFalse(), actionIndex);
					}
				}

				return TRANSPLAN::FIX;
			}

			PROP_STATUS propagateRTransitions(bool assigned)
			{
				IntVector rTrans = Transplan::activities[actionIndex].r_trans;
				for (int i = 0; i < rTrans.size(); i++)
				{
					if (assigned)
					{
						IMPLY_EXCL_ON_FAILURE(manager.svt_inplan(currentState, rTrans[i]).eqTrue(), actionIndex);
					}
					else
					{
						IMPLY_EXCL_ON_FAILURE(manager.svt_inplan(currentState, rTrans[i]).eqFalse(), actionIndex);
					}
				}

				return TRANSPLAN::FIX;
			}

			PROP_STATUS propagateActionStartTimes()
			{
				IntPair startTime = manager.a_start(currentState, actionIndex).dom();

				IntPairSet targetActions;
				manager.a_dist(currentState).getOutgoingEdgesToIndexSet(actionIndex, targetActions);

				for (IntPairSet::iterator itr = targetActions.begin(); itr != targetActions.end(); itr++)
				{
					int toActIndex = (*itr).first;
					int lb_start_dist = (*itr).second;

					int proposedStartTime = startTime.first + lb_start_dist;
					IMPLY_EXCL_ON_FAILURE(manager.a_start(currentState, toActIndex).gq(proposedStartTime), toActIndex);

					proposedStartTime = startTime.second + lb_start_dist;
					IMPLY_EXCL_ON_FAILURE(manager.a_start(currentState, toActIndex).lq(proposedStartTime), toActIndex);
				}

				return TRANSPLAN::FIX;
			}

		public:
			int actionIndex;

			ActionInplanPropagator(int actionIndex, int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager), actionIndex(actionIndex)
			{

			}

			ActionInplanPropagator(const ActionInplanPropagator& prop) :
				Propagator(prop), actionIndex(prop.actionIndex)
			{

			}

			/*
			 * Propagator ActionInplanChanged(A):
			 1) for all related Transitions T: inplan[T] = inplan[A]
			 2. if A is included in the plan, then
			 a) for all actions, Dis(A,A') is defined: start[A'] >= start[A] + lb(Dis(A,A'))and start[A'] <= start[A].UB + lb(Dis(A,A'))
			 b) for each Pair A',A'': such that Dis(A',A) and Dis(A,A'') is defined, Dis(A',A'') >= Dis(A',A) + Dis(A, A'')
			 */
			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;
				bool assigned = manager.a_inplan(state, actionIndex).isTrue();

				RETURN_IF_FAIL(propagateSVTransitions(assigned));
				RETURN_IF_FAIL(propagateRTransitions(assigned));

				if (assigned)
				{
					RETURN_IF_FAIL(propagateActionStartTimes());
				}

				return TRANSPLAN::FIX;
			}
	};
}

#endif	/* ACTIONINPLANCHANGEDPROPAGATOR_H */

