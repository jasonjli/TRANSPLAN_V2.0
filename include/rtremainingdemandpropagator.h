/* 
 * File:   rtremainingdemandchangedpropagator.h
 * Author: debdeep
 *
 * Created on 20 November 2014, 6:31 PM
 */

#ifndef RTREMAININGDEMANDCHANGEDPROPAGATOR_H
#define	RTREMAININGDEMANDCHANGEDPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{
       
    class RTRemainingDemandPropagator: public Propagator
    {
    public:
        int rtIndex;
        
        RTRemainingDemandPropagator(int rtIndex, int propagatorIndex, VariablePropagatorManager& manager)
        :Propagator(propagatorIndex, manager)
        ,rtIndex(rtIndex)
        {
            
        }
        
        RTRemainingDemandPropagator(const RTRemainingDemandPropagator& prop )
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

#endif	/* RTREMAININGDEMANDCHANGEDPROPAGATOR_H */

