/* 
 * File:   rtremainingsupportchangedpropagator.h
 * Author: debdeep
 *
 * Created on 20 November 2014, 6:31 PM
 */

#ifndef RTREMAININGSUPPORTPROPAGATOR_H
#define	RTREMAININGSUPPORTPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{
	class RTRemainingSupportPropagator: public Propagator
	{
		public:
			int rtIndex;

			RTRemainingSupportPropagator(int rtIndex, int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager), rtIndex(rtIndex)
			{

			}

			RTRemainingSupportPropagator(const RTRemainingSupportPropagator& prop) :
				Propagator(prop), rtIndex(prop.rtIndex)
			{

			}

			/*
			 * Propagator RemainingSupportChanged(T):
			 1) for all T' in PossibleCustomer(T): Support[T,T'] <= RemainingSupport(T)
			 */
			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;
				int remSupp = manager.getRemSupport(currentState, rtIndex);
				int actIndex = Transplan::getTransActIndex(rtIndex, false);

				IntSet possCustSet = manager.poss_succ(currentState, rtIndex).domain;

				IntSet::iterator itr = possCustSet.begin();
				while (itr != possCustSet.end())
				{
					int transIndex = *itr;

					//post support max constraint
					IMPLY_EXCL_ON_FAILURE( manager.rt_supp(currentState, rtIndex, transIndex).lq( remSupp ), actIndex);

					itr++;
				}

				return TRANSPLAN::FIX;
			}

	};
}

#endif	/* RTREMAININGSUPPORTPROPAGATOR_H */

