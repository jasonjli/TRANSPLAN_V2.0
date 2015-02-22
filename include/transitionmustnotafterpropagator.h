/* 
 * File:   transitionmustnotafterchangedpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:21 PM
 */

#ifndef TRANSITIONMUSTNOTAFTERPROPAGATOR_H
#define	TRANSITIONMUSTNOTAFTERPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{
	class TMustNotAfterPropagator: public Propagator
	{
		public:
			int tIndex;
			bool isSVT;

			TMustNotAfterPropagator(int tIndex, bool isSVT, int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager), tIndex(tIndex), isSVT(isSVT)
			{

			}

			TMustNotAfterPropagator(const TMustNotAfterPropagator& prop) :
				Propagator(prop), tIndex(prop.tIndex), isSVT(prop.isSVT)
			{

			}

			/*
			 Propagator MustNOTAfterChanged(T): if T' is added
			 1) Next[T] <> T'
			 2) Support[T,T'] = 0,
			 3) if T,T' is mutex, then Post(T'-->T)
			 */
			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;

				CSPAuxSetMonoIncVar& mNotAfter = manager.getTransMustNOTAfter(currentState, tIndex, isSVT);
				int actIndex = Transplan::getTransActIndex(tIndex, isSVT);

				while (!mNotAfter.addedElements.empty())
				{
					int addedTrans = mNotAfter.addedElements.top();
					if (isSVT)
					{
						///delete from next domain
						IMPLY_EXCL_ON_FAILURE(manager.next(currentState, tIndex).nq( addedTrans), actIndex);
					}
					else
					{
						///set the possible support to 0
						IMPLY_EXCL_ON_FAILURE(manager.rt_supp(currentState, tIndex, addedTrans).eq(0), actIndex);

						if (Transplan::r_trans[tIndex].mutex_trans.count(addedTrans) > 0)
						{
							RETURN_IF_FAIL(popagateTransitionPrecedence(addedTrans, tIndex, isSVT));
						}
					}

					mNotAfter.addedElements.pop();
				}
				return TRANSPLAN::FIX;
			}

	};
}

#endif	/* TRANSITIONMUSTNOTAFTERCHANGEDPROPAGATOR_H */

