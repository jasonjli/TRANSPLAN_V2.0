/* 
 * File:   transitionendtimechangedpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:34 PM
 */

#ifndef TRANSITIONENDTIMECHANGEDPROPAGATOR_H
#define	TRANSITIONENDTIMECHANGEDPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{    
    class TEndTimePropagator: public Propagator
    {
    public:
        int tIndex;
        bool isSVT;
        
        TEndTimePropagator(int tIndex, bool isSVT, int propagatorIndex, VariablePropagatorManager& manager)
        :Propagator(propagatorIndex, manager)
        ,tIndex(tIndex)
        ,isSVT(isSVT)
        {
            
        }
        
        TEndTimePropagator(const TEndTimePropagator& prop )
        :Propagator(prop)
        ,tIndex(prop.tIndex)
        ,isSVT(prop.isSVT)
        {
            
        }
        
        PROP_STATUS propagate(SearchState* state)
        {
        	this->currentState = state;
        	///if isMaxUpdated
        	///if isMinUpdated
            return TRANSPLAN::FIX;
        }
        
    };
}

#endif	/* TRANSITIONENDTIMECHANGEDPROPAGATOR_H */

