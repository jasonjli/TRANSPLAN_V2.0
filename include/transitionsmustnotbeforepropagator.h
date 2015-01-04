/* 
 * File:   transitionsmustnotbeforechangedpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:20 PM
 */

#ifndef TRANSITIONSMUSTNOTBEFOREPROPAGATOR_H
#define	TRANSITIONSMUSTNOTBEFOREPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{
	class TMustNotBeforePropagator: public Propagator
	{
		public:
			int tIndex;
			bool isSVT;

			TMustNotBeforePropagator(int tIndex, bool isSVT, int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager), tIndex(tIndex), isSVT(isSVT)
			{

			}

			TMustNotBeforePropagator(const TMustNotBeforePropagator& prop) :
				Propagator(prop), tIndex(prop.tIndex), isSVT(prop.isSVT)
			{

			}

			/*
			 * Propagator MustNOTBeforeChanged(T): if T' is added
			 1) Prev[T] <> T'
			 2) Support[T'T] = 0
			 3) if T,T' is mutex, then Post(T-->T')
			 */
			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;
				CSPAuxSetMonoIncVar& mNotBefore = manager.getTransMustNOTBefore(currentState, tIndex, isSVT);
				int actIndex = Transplan::getTransActIndex(tIndex, isSVT);

				while (!mNotBefore.addedElements.empty())
				{
					int addedTrans = mNotBefore.addedElements.top();
					if (isSVT)
					{
						///delte from prev domain
						IMPLY_EXCL_ON_FAILURE(manager.prev(currentState, tIndex).nq( addedTrans), actIndex);
					}
					else
					{
						///set the possible support to 0
						IMPLY_EXCL_ON_FAILURE(manager.rt_supp(currentState, addedTrans, tIndex).eq(0), actIndex);

						if (Transplan::r_trans[tIndex].mutex_trans.count(addedTrans) > 0)
						{
							RETURN_IF_FAIL(popagateTransitionPrecedence(tIndex, addedTrans, isSVT));
						}
					}

					mNotBefore.addedElements.pop();
				}
				return TRANSPLAN::FIX;
			}

	};
}

#endif	/* TRANSITIONSMUSTNOTBEFORECHANGEDPROPAGATOR_H */

