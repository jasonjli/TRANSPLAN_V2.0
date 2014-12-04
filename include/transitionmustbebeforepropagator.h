/* 
 * File:   transitionmustbebeforechangedpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:17 PM
 */

#ifndef TRANSITIONMUSTBEBEFORECHANGEDPROPAGATOR_H
#define	TRANSITIONMUSTBEBEFORECHANGEDPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{   
    class TMustBeBeforePropagator: public Propagator
    {
    public:
        int tIndex;
        bool isSVT;
        
        TMustBeBeforePropagator(int tIndex, bool isSVT, int propagatorIndex, VariablePropagatorManager& manager)
        :Propagator(propagatorIndex, manager)
        ,tIndex(tIndex)
        ,isSVT(isSVT)
        {
            
        }
        
        TMustBeBeforePropagator(const TMustBeBeforePropagator& prop )
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

#endif	/* TRANSITIONMUSTBEBEFORECHANGEDPROPAGATOR_H */

