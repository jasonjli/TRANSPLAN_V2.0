/* 
 * File:   transitionstarttimepropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:34 PM
 */

#ifndef TRANSITIONSTARTTIMEPROPAGATOR_H
#define	TRANSITIONSTARTTIMEPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{
	class TStartTimePropagator: public Propagator
	{
		private:
			PROP_STATUS propagateStartMax(int maxEndVal, CSPIntervalVar& endVar, int maxActVal, int actIndex)
			{
				IMPLY_EXCL_ON_FAILURE(endVar.lq(maxEndVal), actIndex);
				IMPLY_EXCL_ON_FAILURE(manager.a_start(currentState, actIndex).lq(maxActVal), actIndex);

				IntSet ubChildren = manager.getTransUBChildren(currentState, tIndex, isSVT).dom();

				IntSet::iterator itr = ubChildren.begin();
				while (itr != ubChildren.end())
				{
					currentState->activateInferator(manager.getUBInferatorIndex(this->isSVT, *itr));
				}

				return TRANSPLAN::FIX;
			}

			PROP_STATUS propagateStartMin(int minEndVal, CSPIntervalVar& endVar, int minActVal, int actIndex)
			{
				IMPLY_EXCL_ON_FAILURE(endVar.gq(minEndVal), actIndex);
				IMPLY_EXCL_ON_FAILURE(manager.a_start(currentState, actIndex).gq(minActVal), actIndex);

				return TRANSPLAN::FIX;
			}

		public:
			int tIndex;
			bool isSVT;

			TStartTimePropagator(int tIndex, bool isSVT, int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager), tIndex(tIndex), isSVT(isSVT)
			{

			}

			TStartTimePropagator(const TStartTimePropagator& prop) :
				Propagator(prop), tIndex(prop.tIndex), isSVT(prop.isSVT)
			{

			}

			/*
			 * Propagator StartTimeChanged(T):
			 1) End[T] = Start[T] + Dur[T]
			 3) Start[A] = Start[T] - offset(T)
			 2) Call inference 1/3, 2/4: : if upper bound of the start time changed then for each transition in the UB_Children set, call the SVT_UB or RT_UB inference
			 */
			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;

				int actIndex = Transplan::getTransActIndex(tIndex, isSVT);
				CSPIntervalVar& start = manager.getTransStart(currentState, tIndex, isSVT);
				CSPIntervalVar& end = manager.getTransEnd(currentState, tIndex, isSVT);
				int offSet = Transplan::getTransOffset(tIndex, isSVT);
				int duration = Transplan::getTransDuration(tIndex, isSVT);

				IntPair startTime = start.dom();

				if (start.isMaxUpdate)
				{
					RETURN_IF_FAIL( propagateStartMax(start.max()+duration, end, start.max()- offSet, actIndex));
					start.isMaxUpdate = false;
				}

				if (start.isMinUpdate)
				{
					RETURN_IF_FAIL(propagateStartMin(start.min()+duration, end, start.min()- offSet, actIndex));
					start.isMinUpdate = false;
				}

				return TRANSPLAN::FIX;
			}

	};
}

#endif	/* TRANSITIONSTARTTIMEPROPAGATOR_H */

