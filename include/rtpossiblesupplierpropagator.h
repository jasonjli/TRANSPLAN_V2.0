/* 
 * File:   rtpossiblesupplierchangedpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:33 PM
 */

#ifndef RTPOSSIBLESUPPLIERCHANGEDPROPAGATOR_H
#define	RTPOSSIBLESUPPLIERCHANGEDPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{
	class RTPossibleSupplierPropagator: public Propagator
	{
		public:
			int rtIndex;

			RTPossibleSupplierPropagator(int rtIndex, int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager), rtIndex(rtIndex)
			{

			}

			RTPossibleSupplierPropagator(const RTPossibleSupplierPropagator& prop) :
				Propagator(prop), rtIndex(prop.rtIndex)
			{

			}

			/*
			 * Propagator PossibleSupplierChanged(T): T' is deleted
			 1) if T' is in RT_LB_Set for T, then call RT_LB inference for T
			 */
			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;
				while (!manager.poss_pred(currentState, rtIndex).deletedElements.empty())
				{
					int deletedTransIndex = manager.poss_pred(currentState, rtIndex).deletedElements.top();
					IntSet lbSet = manager.rt_LB(currentState, rtIndex).domain;
					if (lbSet.count(deletedTransIndex) > 0)
					{
						currentState->activateInferator(manager.getLBInferatorIndex(false, rtIndex));
					}

					manager.poss_pred(currentState, rtIndex).deletedElements.pop();
				}
				return TRANSPLAN::FIX;
			}

	};
}

#endif	/* RTPOSSIBLESUPPLIERCHANGEDPROPAGATOR_H */

