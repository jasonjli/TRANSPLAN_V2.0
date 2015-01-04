/* 
 * File:   transitionmustbeafterpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:18 PM
 */

#ifndef TRANSITIONMUSTBEAFTERPROPAGATOR_H
#define	TRANSITIONMUSTBEAFTERPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{
	class TMustBeAfterPropagator: public Propagator
	{
		public:
			int tIndex;
			bool isSVT;

			TMustBeAfterPropagator(int tIndex, bool isSVT, int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager), tIndex(tIndex), isSVT(isSVT)
			{

			}

			TMustBeAfterPropagator(const TMustBeAfterPropagator& prop) :
				Propagator(prop), tIndex(prop.tIndex), isSVT(prop.isSVT)
			{

			}

			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;

				CSPAuxSetMonoIncVar& mBefore = manager.getTransMustBefore(currentState, tIndex, isSVT);
				CSPAuxSetMonoIncVar& mAfter = manager.getTransMustAfter(currentState, tIndex, isSVT);
				int actIndex = Transplan::getTransActIndex(tIndex, isSVT);

				while (!mAfter.addedElements.empty())
				{
					int addedTrans = mAfter.addedElements.top();

					/// if added Transition is active then recalculate UB
					if (manager.isTransIncluded(currentState, addedTrans, isSVT))
					{
						currentState->activateInferator(manager.getLBInferatorIndex(isSVT, tIndex));
					}

					/// if the transition in active then propagate precedence constraint between added element and the set of transition in mustBeBefore set
					if (manager.isTransIncluded(currentState, tIndex, isSVT))
					{
						IntSet::iterator beforeItr = mBefore.dom().begin();
						while (beforeItr != mBefore.dom().end())
						{
							RETURN_IF_FAIL(popagateTransitionPrecedence( *beforeItr, addedTrans, isSVT));
						}
					}

					/*
					 * @TODO: MustBeInbetween propagation
					 */
					mAfter.addedElements.pop();
				}
				return TRANSPLAN::FIX;
			}

	};
}

#endif	/* TRANSITIONMUSTBEAFTERPROPAGATOR_H */

