/*
 * rtFSSpropagator.h
 *
 *  Created on: 21/02/2015
 *      Author: debdeep
 */

#ifndef RTFSSPROPAGATOR_H_
#define RTFSSPROPAGATOR_H_

#include "propagator.h"
namespace TRANSPLAN
{
	class RTFSSPropagator: public Propagator
	{
		public:
			int rtIndex;

			RTFSSPropagator(int rtIndex, int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager), rtIndex(rtIndex)
			{

			}

			RTFSSPropagator(const RTCustomerPropagator& prop) :
				Propagator(prop), rtIndex(prop.rtIndex)
			{

			}

			/*
			 Propagator FFSChanged(T)
			 1. For each Transition T' in Customer(T): update FFS(T') >= min(Support[T,T'], FFS(T))
			 2. Do mutex propagation 	T, T' is mutex, Capacity(R) - FFS(T) < req(T') ==> T-->T' : provided T,T' didn't have any precedence constraints
			 */
			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;
				IntSet dom = manager.customers(this->currentState, rtIndex).dom();
				int Tffs = manager.getFFS(this->currentState, rtIndex);

				IntSet::iterator itr = dom.begin();
				while( itr != dom.end() )
				{
					int toIndex = *itr;
					int supportMin = manager.getRTSupportMin(currentState, rtIndex, toIndex);
					int updatedFFS = std::min(supportMin, Tffs);

					IMPLY_EXCL_ON_FAILURE( manager.FFS(currentState, toIndex).gq( updatedFFS ), Transplan::getTransActIndex( rtIndex, false));

					itr++;
				}

				IntSet mutexset = Transplan::getTranMutexSet(rtIndex);
				itr = mutexset.begin();
				while( itr != mutexset.end())
				{
					int muIndex = *itr;
					if( !hasPrecendenceConstraint(rtIndex, muIndex, false) && !hasPrecendenceConstraint(muIndex, rtIndex, false))
					{
						int resIndex = Transplan::getResourceIndex(rtIndex);
						int cap = Transplan::resources[resIndex].cap;

						if( cap - Tffs < Transplan::r_trans[muIndex].req )
						{
							RETURN_IF_FAIL(popagateTransitionPrecedence(rtIndex, muIndex, false));
						}
					}
				}
				return TRANSPLAN::FIX;
			}

	};
}
#endif /* RTFSSPROPAGATOR_H_ */
