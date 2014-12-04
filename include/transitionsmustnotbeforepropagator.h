/* 
 * File:   transitionsmustnotbeforechangedpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:20 PM
 */

#ifndef TRANSITIONSMUSTNOTBEFOREPROPAGATOR_H
#define	TRANSITIONSMUSTNOTBEFOREPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{    
    class TMustNotBeforePropagator: public Propagator
    {
    public:
        int tIndex;
        bool isSVT;
        
        TMustNotBeforePropagator(int tIndex, bool isSVT, int propagatorIndex, VariablePropagatorManager& manager)
        :Propagator(propagatorIndex, manager)
        ,tIndex(tIndex)
        ,isSVT(isSVT)
        {
            
        }
        
        TMustNotBeforePropagator(const TMustNotBeforePropagator& prop )
        :Propagator(prop)
        ,tIndex(prop.tIndex)
        ,isSVT(prop.isSVT)
        {
            
        }
        
        PROP_STATUS propagate(SearchState* state)
        {
        	this->currentState = state;
            return TRANSPLAN::FIX;
        }
        
    };
}

#endif	/* TRANSITIONSMUSTNOTBEFORECHANGEDPROPAGATOR_H */

