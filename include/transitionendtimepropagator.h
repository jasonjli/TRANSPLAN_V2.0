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

				//// re-infer lower bounds
				IntSet lbChildren = manager.getTransLBChildren(currentState, tIndex, isSVT).dom();

				IntSet::iterator itr = lbChildren.begin();
				while (itr != lbChildren.end())
				{
					currentState->activateInferator(manager.getLBInferatorIndex(this->isSVT, *itr));
				}

				if( isSVT )
				{
					if( !manager.isSVTExcluded(currentState, tIndex) && !manager.next(currentState, tIndex).assigned() )
					{
						RETURN_IF_FAIL( inferAbsoluteTemporalConstraint());
					}
				}
				else if( !manager.isRTExcluded(currentState, tIndex) && manager.getRemSupport(currentState, tIndex) > 0 )
				{
					RETURN_IF_FAIL( inferAbsoluteTemporalConstraint());
				}
				return TRANSPLAN::FIX;
			}

			PROP_STATUS inferAbsoluteTemporalConstraint()
			{
				int endMin = manager.getTransEnd(currentState, tIndex, isSVT).min();

				IntSet poss_succ;
				if( isSVT )
				{
					poss_succ = manager.next(currentState, tIndex).dom();
				}
				else
					poss_succ = manager.poss_succ(currentState, tIndex).dom();

				IntSet::iterator itr = poss_succ.begin();
				while( itr != poss_succ.end() )
				{
					int startMax = manager.getTransStart(currentState, *itr, isSVT).max();
					if( endMin > startMax )
					{
						RETURN_IF_FAIL(popagateTransitionAntiPrecedence(tIndex, *itr, isSVT));
					}
					itr++;
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
					///
					end.isMinUpdate = false;
				}
				return TRANSPLAN::FIX;
			}

	};
}

#endif	/* TRANSITIONENDTIMEPROPAGATOR_H */

