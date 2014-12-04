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
        
        RTPossibleCustomerPropagator(int rtIndex, int propagatorIndex, VariablePropagatorManager& manager)
        :Propagator(propagatorIndex, manager)
        ,rtIndex(rtIndex)
        {
            
        }
        
        RTPossibleCustomerPropagator(const RTPossibleCustomerPropagator& prop )
        :Propagator(prop)
        ,rtIndex(prop.rtIndex)
        {
            
        }
        
        PROP_STATUS propagate(SearchState* state)
        {
        	this->currentState = state;
            return TRANSPLAN::FIX;
        }
        
    };
}

#endif	/* RTPOSSIBLECUSTOMERCHANGEDPROPAGATOR_H */

