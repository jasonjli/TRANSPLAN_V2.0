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
        
        RTSupportPropagator(int fromRTIndex, int toRTIndex, int propagatorIndex, VariablePropagatorManager& manager)
        :Propagator(propagatorIndex, manager)
        ,fromRTIndex(fromRTIndex)
        ,toRTIndex(toRTIndex)
        {
            
        }
        
        RTSupportPropagator(const RTSupportPropagator& prop )
        :Propagator(prop)
        ,fromRTIndex(prop.fromRTIndex)
        ,toRTIndex(prop.toRTIndex)
        {
            
        }
        
        PROP_STATUS propagate(SearchState* state)
        {
        	this->currentState = state;
            return TRANSPLAN::FIX;
        }
        
    };
}

#endif	/* RTSUPPORTCHANGEDPROPAGATOR_H */

