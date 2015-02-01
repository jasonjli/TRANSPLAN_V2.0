/* 
 * File:   rtsupportchangedpropagator.h
 * Author: debdeep
 *
 * Created on 20 November 2014, 6:32 PM
 */

#ifndef RTSUPPORTCHANGEDPROPAGATOR_H
#define	RTSUPPORTCHANGEDPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{
	class RTSupportPropagator: public Propagator
	{
		public:
			int fromRTIndex;
			int toRTIndex;

			RTSupportPropagator(int fromRTIndex, int toRTIndex, int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager), fromRTIndex(fromRTIndex), toRTIndex(toRTIndex)
			{

			}

			RTSupportPropagator(const RTSupportPropagator& prop) :
				Propagator(prop), fromRTIndex(prop.fromRTIndex), toRTIndex(prop.toRTIndex)
			{

			}

			/**
			 * Propagator SupportChanged(T,T'):
			 1) If LB(Support(T,T')) > 0 then
			 a) make Inplan[T]=Inplan[T']=true
			 b) Add T' into Customer(T)
			 c) Add T into Supplier(T')
			 d) update FFS(T') >= min(lb(Support[T,T']), FFS(T))

			 2) If UB(Support(T,T')) = 0 then
			 a) Remove T from PossibleSupplier(T')
			 b) Remove T' from PossibleCustomer(T)
			 */
			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;

				CSPIntervalVar& supp = manager.rt_supp(currentState, fromRTIndex, toRTIndex);
				int fromActIndex = Transplan::getTransActIndex(fromRTIndex, false);
				int toActIndex = Transplan::getTransActIndex(toRTIndex, false);

				if (supp.min() > 0 and supp.isMinUpdate )
				{

					IMPLY_EXCL_ON_FAILURE( manager.getTransInplan(currentState, fromRTIndex, false).eqTrue(), fromActIndex);

					IMPLY_EXCL_ON_FAILURE( manager.getTransInplan(currentState, toRTIndex, false).eqTrue(), toActIndex);

					manager.customers(currentState, fromRTIndex).add(toRTIndex);
					manager.suppliers(currentState, toRTIndex).add(fromRTIndex);

					int lbSupport = supp.min();
					int FFSFrom  = manager.getFFS(currentState, fromRTIndex);

					int effectiveFFS = std::min(lbSupport, FFSFrom);

					IMPLY_EXCL_ON_FAILURE( manager.FSS(currentState, toRTIndex).gq( effectiveFFS), toActIndex);

					supp.isMinUpdate = false;
				}

				if (supp.min() == 0 && supp.max() == 0)
				{
					IMPLY_EXCL_ON_FAILURE(manager.poss_pred(currentState, toRTIndex).remove(fromRTIndex), toActIndex);
					IMPLY_EXCL_ON_FAILURE(manager.poss_succ(currentState, fromRTIndex).remove(toRTIndex), toActIndex);
				}

				if( supp.isMaxUpdate )
				{
					IntSet lbSet = manager.getTransLB(currentState, toRTIndex, false).domain;
					if( lbSet.count(fromRTIndex) > 0 )
					{
						currentState->activateInferator( manager.getLBInferatorIndex(false, toRTIndex));
					}

					IntSet ubSet = manager.getTransUB(currentState, fromRTIndex, false).domain;
					if( ubSet.count(toRTIndex) > 0 )
					{
						currentState->activateInferator( manager.getUBInferatorIndex(false, fromRTIndex));
					}

					supp.isMaxUpdate = false;
				}
				return TRANSPLAN::FIX;
			}

	};
}

#endif	/* RTSUPPORTCHANGEDPROPAGATOR_H */

