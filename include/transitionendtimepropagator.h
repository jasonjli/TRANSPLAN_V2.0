/* 
 * File:   transitionendtimepropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:34 PM
 */

#ifndef TRANSITIONENDTIMEPROPAGATOR_H
#define	TRANSITIONENDTIMEPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{
	class TEndTimePropagator: public Propagator
	{
		private:

			PROP_STATUS propagateEndMax(int maxStartVal, CSPIntervalVar& startVar, int actIndex)
			{
				IMPLY_EXCL_ON_FAILURE(startVar.lq(maxStartVal), actIndex);

				return TRANSPLAN::FIX;
			}

			PROP_STATUS propagateEndMin(int minStartVal, CSPIntervalVar& startVar, int actIndex)
			{
				IMPLY_EXCL_ON_FAILURE(startVar.gq(minStartVal), actIndex);

				IntSet lbChildren = manager.getTransLBChildren(currentState, tIndex, isSVT).dom();

				IntSet::iterator itr = lbChildren.begin();
				while (itr != lbChildren.end())
				{
					currentState->activateInferator(manager.getLBInferatorIndex(this->isSVT, *itr));
				}
				return TRANSPLAN::FIX;
			}
		public:
			int tIndex;
			bool isSVT;

			TEndTimePropagator(int tIndex, bool isSVT, int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager), tIndex(tIndex), isSVT(isSVT)
			{

			}

			TEndTimePropagator(const TEndTimePropagator& prop) :
				Propagator(prop), tIndex(prop.tIndex), isSVT(prop.isSVT)
			{

			}

			/*
			 * Propagator EndTimeChanged(T):
			 1) Start[T] = End[T] - Dur[T]
			 2) Call inference 1/3, 2/4: if lower bound of the end time changed then for each transition in the LB_Children set, call the SVT_LB and RT_LB inference
			 */
			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;

				int actIndex = Transplan::getTransActIndex(tIndex, isSVT);
				CSPIntervalVar& start = manager.getTransStart(currentState, tIndex, isSVT);
				CSPIntervalVar& end = manager.getTransEnd(currentState, tIndex, isSVT);
				int duration = Transplan::getTransDuration(tIndex, isSVT);

				IntPair endTime = end.dom();

				if (end.isMaxUpdate)
				{
					RETURN_IF_FAIL( propagateEndMax(end.max()- duration, start, actIndex) );
					end.isMaxUpdate = false;
				}

				if (end.isMinUpdate)
				{
					RETURN_IF_FAIL( propagateEndMin(end.min() - duration, start, actIndex) );
					end.isMinUpdate = false;
				}
				return TRANSPLAN::FIX;
			}

	};
}

#endif	/* TRANSITIONENDTIMEPROPAGATOR_H */

