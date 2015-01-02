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

			PROP_STATUS propagateSVTStartTimes(IntPair startTime, bool isLB, bool isUB)
			{
				IntVector svTrans = Transplan::activities[actionIndex].sv_trans;
				for (int i = 0; i < svTrans.size(); i++)
				{
					if (isLB)
					{
						int start_offset = startTime.first + Transplan::sv_trans[svTrans[i]].offset;
						IMPLY_EXCL_ON_FAILURE(manager.svt_start(this->currentState, svTrans[i]).gq(start_offset), actionIndex);
					}

					if (isUB)
					{
						int start_offset = startTime.second + Transplan::sv_trans[svTrans[i]].offset;
						IMPLY_EXCL_ON_FAILURE(manager.svt_start(this->currentState, svTrans[i]).lq(start_offset), actionIndex);
					}
				}

			}

			PROP_STATUS propagateRTStratTimes(IntPair startTime, bool isLB, bool isUB)
			{
				IntVector rTrans = Transplan::activities[actionIndex].r_trans;
				for (int i = 0; i < rTrans.size(); i++)
				{
					if (isLB)
					{
						int start_offset = startTime.first + Transplan::r_trans[rTrans[i]].offset;
						IMPLY_EXCL_ON_FAILURE(manager.rt_start(this->currentState, rTrans[i]).gq(start_offset), actionIndex);
					}

					if (isUB)
					{
						int start_offset = startTime.second + Transplan::r_trans[rTrans[i]].offset;
						IMPLY_EXCL_ON_FAILURE(manager.rt_start(this->currentState, rTrans[i]).lq(start_offset), actionIndex);
					}
				}

			}

			PROP_STATUS propagateActionStartTimes(IntPair startTime, bool isLB, bool isUB)
			{
				if (manager.isActIncluded(currentState, actionIndex))
				{
					IntPairSet targetActions;
					manager.a_dist(currentState).getOutgoingEdgesToIndexSet(actionIndex, targetActions);
					for (IntPairSet::iterator itr = targetActions.begin(); itr != targetActions.end(); itr++)
					{
						int toActIndex = (*itr).first;
						int lb_start_dist = (*itr).second;

						if (isLB)
						{
							int proposedStartTime = startTime.first + lb_start_dist;
							IMPLY_EXCL_ON_FAILURE(manager.a_start(currentState, toActIndex).gq(proposedStartTime), toActIndex);
						}

						if (isUB)
						{
							int proposedStartTime = startTime.second + lb_start_dist;
							IMPLY_EXCL_ON_FAILURE(manager.a_start(currentState, toActIndex).lq(proposedStartTime), toActIndex);
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

