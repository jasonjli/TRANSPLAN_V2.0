/* 
 * File:   rtcustomerchangedpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:32 PM
 */

#ifndef RTCUSTOMERPROPAGATOR_H
#define	RTCUSTOMERPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{
	class RTCustomerPropagator: public Propagator
	{
		public:
			int rtIndex;

			RTCustomerPropagator(int rtIndex, int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager), rtIndex(rtIndex)
			{

			}

			RTCustomerPropagator(const RTCustomerPropagator& prop) :
				Propagator(prop), rtIndex(prop.rtIndex)
			{

			}

			/*
			 * Propagator CustomerChanged(T): if T' is added
			 1) post T->T'
			 3) update remaining support: RemainingSupport <= RemainingSupport - lb(Support[T,T'])
			 2) Remove T' from PossibleCustomer(T)
			 */
			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;

				int actIndex = Transplan::getTransActIndex(rtIndex, false);

				while (!manager.customers(currentState, rtIndex).addedElements.empty())
				{
					// get added customer T'
					int addedTrans = manager.customers(currentState, rtIndex).addedElements.top();

					///  supp(T,T')>= 1
					IMPLY_EXCL_ON_FAILURE( manager.rt_supp(currentState, rtIndex, addedTrans).gq(1), actIndex);

					///post T->T'
					RETURN_IF_FAIL( this->popagateTransitionPrecedence(rtIndex, addedTrans, false));

					int remSupport = manager.getRemSupport(currentState, rtIndex);
					int lbSupp = manager.getRTSupportMin(currentState, rtIndex, addedTrans);

					int newRemSupport = remSupport - lbSupp;

					/// update remaining support
					IMPLY_EXCL_ON_FAILURE(manager.rem_supp(currentState, rtIndex).lq( newRemSupport), actIndex);

					//// remove T' from possible customer set
					IMPLY_EXCL_ON_FAILURE(manager.poss_succ(currentState, rtIndex).remove( addedTrans), actIndex);

					/// removed added elements from the stack
					manager.customers(currentState, rtIndex).addedElements.pop();
				}

				/// RT_UB inference for T
				currentState->activateInferator(manager.getUBInferatorIndex(false, rtIndex));

				return TRANSPLAN::FIX;
			}

	};
}

#endif	/* RTCUSTOMERPROPAGATOR_H */

