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
        
        RTPossibleSupplierPropagator(int rtIndex, int propagatorIndex, VariablePropagatorManager& manager)
        :Propagator(propagatorIndex, manager)
        ,rtIndex(rtIndex)
        {
            
        }
        
        RTPossibleSupplierPropagator(const RTPossibleSupplierPropagator& prop )
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

#endif	/* RTPOSSIBLESUPPLIERCHANGEDPROPAGATOR_H */

