/* 
 * File:   actionstarttimechangedpropagator.h
 * Author: debdeep
 *
 * Created on 20 November 2014, 6:33 PM
 */

#ifndef ACTIONSTARTTIMEPROPAGATOR_H
#define	ACTIONSTARTTIMEPROPAGATOR_H

#include "propagator.h"
#include "transplan.h"

namespace TRANSPLAN
{

	class ActionStartTimePropagator: public Propagator
	{
		private:

			PROP_STATUS propagateTransStartTimes(int transIndex, IntPair startTime, bool isLB, bool isUB, bool isSVT)
			{
				int offset = Transplan::getTransOffset(transIndex, isSVT);

				if (isLB)
				{
					int newStart = startTime.first + offset;
					IMPLY_EXCL_ON_FAILURE(manager.svt_start(this->currentState, transIndex).gq(newStart), actionIndex);
				}

				if (isUB)
				{
					int newStart = startTime.second + offset;
					IMPLY_EXCL_ON_FAILURE(manager.svt_start(this->currentState, transIndex).lq(newStart), actionIndex);
				}

				return TRANSPLAN::FIX;
			}

			PROP_STATUS propagateSVTStartTimes(IntPair startTime, bool isLB, bool isUB)
			{
				IntVector svTrans = Transplan::activities[actionIndex].sv_trans;
				for (int i = 0; i < svTrans.size(); i++)
				{
					RETURN_IF_FAIL(propagateTransStartTimes(svTrans[i], startTime, isLB, isUB, true));
				}

			}

			PROP_STATUS propagateRTStratTimes(IntPair startTime, bool isLB, bool isUB)
			{
				IntVector rTrans = Transplan::activities[actionIndex].r_trans;
				for (int i = 0; i < rTrans.size(); i++)
				{
					RETURN_IF_FAIL(propagateTransStartTimes(rTrans[i], startTime, isLB, isUB, false));
				}

			}

			PROP_STATUS propagateActionStartTimes(IntPair startTime, bool isLB, bool isUB)
			{
				if (manager.isActIncluded(currentState, actionIndex))
				{
					if (isLB)
					{
						IntPairSet targetActions;
						manager.a_dist(currentState).getOutgoingEdgesToIndexSet(actionIndex, targetActions);
						for (IntPairSet::iterator itr = targetActions.begin(); itr != targetActions.end(); itr++)
						{
							int toActIndex = (*itr).first;
							int lb_start_dist = (*itr).second;

							int proposedStartTime = startTime.first + lb_start_dist;
							IMPLY_EXCL_ON_FAILURE(manager.a_start(currentState, toActIndex).gq(proposedStartTime), toActIndex);
						}
					}

					if (isUB)
					{
						IntPairSet targetActions;
						manager.a_dist(currentState).getIncomingEdgesFromIndexSet(actionIndex, targetActions);
						for (IntPairSet::iterator itr = targetActions.begin(); itr != targetActions.end(); itr++)
						{
							int fromActIndex = (*itr).first;
							int lb_start_dist = (*itr).second;

							int proposedStartTime = startTime.second - lb_start_dist;
							IMPLY_EXCL_ON_FAILURE(manager.a_start(currentState, fromActIndex).lq(proposedStartTime), fromActIndex);
						}
					}
				}

				return TRANSPLAN::FIX;
			}
		public:
			int actionIndex;

			ActionStartTimePropagator(int actionIndex, int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager), actionIndex(actionIndex)
			{

			}

			ActionStartTimePropagator(const ActionStartTimePropagator& prop) :
				Propagator(prop), actionIndex(prop.actionIndex)
			{

			}

			/*
			 * Propagator ActionStartTimeChanged(A):
			 1) for all related Transitions T: start[T] >= start[A].LB + offset(T) and start[T] <= start[A].UB + offset
			 2) if A is inplan, for all actions, Dist(A,A') is defined: start[A'] >= start[A].LB + lb(Dis(A,A')) and start[A'] <= start[A].UB + lb(Dis(A,A')),
			 */
			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;
				CSPIntervalVar& start = manager.a_start(state, actionIndex);
				IntPair startTime = start.dom();//manager.a_start(state, actionIndex).domain;

				RETURN_IF_FAIL(propagateSVTStartTimes(startTime, start.isMinUpdate, start.isMaxUpdate));
				RETURN_IF_FAIL(propagateRTStratTimes(startTime, start.isMinUpdate, start.isMaxUpdate));
				RETURN_IF_FAIL(propagateActionStartTimes(startTime, start.isMinUpdate, start.isMaxUpdate));

				start.isMinUpdate = false;
				start.isMaxUpdate = false;

				return TRANSPLAN::FIX;
			}

	};
}

#endif	/* ACTIONSTARTTIMECHANGEDPROPAGATOR_H */

