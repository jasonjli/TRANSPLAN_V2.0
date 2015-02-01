/* 
 * File:   rtremainingdemandchangedpropagator.h
 * Author: debdeep
 *
 * Created on 20 November 2014, 6:31 PM
 */

#ifndef RTREMAININGDEMANDPROPAGATOR_H
#define	RTREMAININGDEMANDPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{

	class RTRemainingDemandPropagator: public Propagator
	{
		public:
			int rtIndex;

			RTRemainingDemandPropagator(int rtIndex, int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager), rtIndex(rtIndex)
			{

			}

			RTRemainingDemandPropagator(const RTRemainingDemandPropagator& prop) :
				Propagator(prop), rtIndex(prop.rtIndex)
			{

			}

			/*
			 * Propagator RemainingDemandChanged(T):
			 1) for all T' in PossibleSupporter(T)|: Support[T',T] <= RemainingDemand(T)
			 */
			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;
				int remDemand = manager.getRemDemand(currentState, rtIndex);
				int actIndex = Transplan::getTransActIndex(rtIndex, false);

				IntSet possSuppSet = manager.poss_pred(currentState, rtIndex).domain;

				IntSet::iterator itr = possSuppSet.begin();
				while (itr != possSuppSet.end())
				{
					int transIndex = *itr;

					//post support max constraint
					IMPLY_EXCL_ON_FAILURE( manager.rt_supp(currentState, transIndex, rtIndex).lq( remDemand ), actIndex);

					itr++;
				}

				return TRANSPLAN::FIX;
			}

	};
}

#endif	/* RTREMAININGDEMANDPROPAGATOR_H */

