/* 
 * File:   rtsupplierchangedpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:32 PM
 */

#ifndef RTSUPPLIERPROPAGATOR_H
#define	RTSUPPLIERPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{
	class RTSupplierPropagator: public Propagator
	{
		public:
			int rtIndex;

			RTSupplierPropagator(int rtIndex, int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager), rtIndex(rtIndex)
			{

			}

			RTSupplierPropagator(const RTSupplierPropagator& prop) :
				Propagator(prop), rtIndex(prop.rtIndex)
			{

			}

			/*
			 * Propagator SupplierChanged(T): if T' is added
			 1) post T'->T
			 3) update remaining demand : Remdemand <= RemainingDemand - lb(Support[T',T])
			 2) Remove T' from PossibleSupplier(T)
			 */
			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;

				int actIndex = Transplan::getTransActIndex(rtIndex, false);

				while (!manager.suppliers(currentState, rtIndex).addedElements.empty())
				{
					// get added supplier T'
					int addedTrans = manager.suppliers(currentState, rtIndex).addedElements.top();

					///  supp(T',T)>= 1
					IMPLY_EXCL_ON_FAILURE( manager.rt_supp(currentState, addedTrans, rtIndex).gq(1), actIndex);

					///post T'->T
					RETURN_IF_FAIL( this->popagateTransitionPrecedence(addedTrans, rtIndex, false));

					int remDemand = manager.getRemDemand(currentState, rtIndex);
					int lbSupp = manager.getRTSupportMin(currentState, addedTrans, rtIndex);

					int newRemDemand = remDemand - lbSupp;

					/// update remaining demand
					IMPLY_EXCL_ON_FAILURE(manager.rem_demand(currentState, rtIndex).lq( newRemDemand), actIndex);

					//// remove T' from possible supplier set
					IMPLY_EXCL_ON_FAILURE(manager.poss_pred(currentState, rtIndex).remove( addedTrans), actIndex);

					/// removed added elements from the stack
					manager.suppliers(currentState, rtIndex).addedElements.pop();
				}

				///call RT_LB inferator
				currentState->activateInferator(manager.getLBInferatorIndex(false, rtIndex));

				return TRANSPLAN::FIX;
			}

	};
}

#endif	/* RTSUPPLIERPROPAGATOR_H */

