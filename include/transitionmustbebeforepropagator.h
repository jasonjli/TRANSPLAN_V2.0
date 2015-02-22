/* 
 * File:   transitionmustbebeforepropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:17 PM
 */

#ifndef TRANSITIONMUSTBEBEFOREPROPAGATOR_H
#define	TRANSITIONMUSTBEBEFOREPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{
	class TMustBeBeforePropagator: public Propagator
	{

		public:
			int tIndex;
			bool isSVT;

			TMustBeBeforePropagator(int tIndex, bool isSVT, int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager), tIndex(tIndex), isSVT(isSVT)
			{

			}

			TMustBeBeforePropagator(const TMustBeBeforePropagator& prop) :
				Propagator(prop), tIndex(prop.tIndex), isSVT(prop.isSVT)
			{

			}

			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;

				CSPAuxSetMonoIncVar& mBefore = manager.getTransMustBefore(currentState, tIndex, isSVT);
				CSPAuxSetMonoIncVar& mAfter = manager.getTransMustAfter(currentState, tIndex, isSVT);
				int actIndex = Transplan::getTransActIndex(tIndex, isSVT);

				while (!mBefore.addedElements.empty())
				{
					int addedTrans = mBefore.addedElements.top();

					/// if added Transition is active then recalculate LB
					if (manager.isTransIncluded(currentState, addedTrans, isSVT))
					{
						currentState->activateInferator(manager.getLBInferatorIndex(isSVT, tIndex));
					}

					/// if the transition is active then propagate precedence constraint between added element and the set of transition in mustBeAfter set
					if (manager.isTransIncluded(currentState, tIndex, isSVT))
					{
						IntSet::iterator afterItr = mAfter.dom().begin();
						while (afterItr != mAfter.dom().end())
						{
							RETURN_IF_FAIL(popagateTransitionPrecedence(addedTrans, *afterItr, isSVT));
						}
					}



					/*
					 * @TODO: MustBeInbetween propagation based in USB set
					 */
					mBefore.addedElements.pop();
				}

				return TRANSPLAN::FIX;
			}

	};
}

#endif	/* TRANSITIONMUSTBEBEFOREPROPAGATOR_H */

