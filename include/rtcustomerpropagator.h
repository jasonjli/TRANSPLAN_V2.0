/* 
 * File:   rtcustomerchangedpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:32 PM
 */

#ifndef RTCUSTOMERCHANGEDPROPAGATOR_H
#define	RTCUSTOMERCHANGEDPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{ 
    class RTCustomerPropagator: public Propagator
    {
    public:
        int rtIndex;
        
        RTCustomerPropagator(int rtIndex, int propagatorIndex, VariablePropagatorManager& manager)
        :Propagator(propagatorIndex, manager)
        ,rtIndex(rtIndex)
        {
            
        }
        
        RTCustomerPropagator(const RTCustomerPropagator& prop )
        :Propagator(prop)
        ,rtIndex(prop.rtIndex)
        {
            
        }
        
        PROP_STATUS propagate(SearchState* state)
        {
            return TRANSPLAN::FIX;
        }
        
    };
}

#endif	/* RTCUSTOMERCHANGEDPROPAGATOR_H */

