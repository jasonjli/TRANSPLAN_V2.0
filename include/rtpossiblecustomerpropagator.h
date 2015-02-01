/* 
 * File:   rtpossiblecustomerchangedpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:33 PM
 */

#ifndef RTPOSSIBLECUSTOMERCHANGEDPROPAGATOR_H
#define	RTPOSSIBLECUSTOMERCHANGEDPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{

	class RTPossibleCustomerPropagator: public Propagator
	{
		public:
			int rtIndex;

			RTPossibleCustomerPropagator(int rtIndex, int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager), rtIndex(rtIndex)
			{

			}

			RTPossibleCustomerPropagator(const RTPossibleCustomerPropagator& prop) :
				Propagator(prop), rtIndex(prop.rtIndex)
			{

			}

			/*
			 *Propagator PossibleCustomerChanged(T): T' is deleted
			 1) if T' is in RT_UB_Set for T, then call RT_UB inference for T
			 */
			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;

				while (!manager.poss_succ(currentState, rtIndex).deletedElements.empty())
				{
					int deletedTransIndex = manager.poss_succ(currentState, rtIndex).deletedElements.top();
					IntSet ubSet = manager.rt_UB(currentState, rtIndex).domain;
					if( ubSet.count( deletedTransIndex ) > 0 )
					{
						currentState->activateInferator( manager.getUBInferatorIndex(false, rtIndex));
					}

					manager.poss_succ(currentState, rtIndex).deletedElements.pop();
				}
				return TRANSPLAN::FIX;
			}

	};
}

#endif	/* RTPOSSIBLECUSTOMERCHANGEDPROPAGATOR_H */

