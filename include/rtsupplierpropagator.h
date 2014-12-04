/* 
 * File:   rtsupplierchangedpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:32 PM
 */

#ifndef RTSUPPLIERCHANGEDPROPAGATOR_H
#define	RTSUPPLIERCHANGEDPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{
    class RTSupplierPropagator: public Propagator
    {
    public:
        int rtIndex;
        
        RTSupplierPropagator(int rtIndex, int propagatorIndex, VariablePropagatorManager& manager)
        :Propagator(propagatorIndex, manager)
        ,rtIndex(rtIndex)
        {
            
        }
        
        RTSupplierPropagator(const RTSupplierPropagator& prop )
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

#endif	/* RTSUPPLIERCHANGEDPROPAGATOR_H */

