/* 
 * File:   transitionmustnotafterchangedpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:21 PM
 */

#ifndef TRANSITIONMUSTNOTAFTERPROPAGATOR_H
#define	TRANSITIONMUSTNOTAFTERPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{   
    class TMustNotAfterPropagator: public Propagator
    {
    public:
        int tIndex;
        bool isSVT;
        
        TMustNotAfterPropagator(int tIndex, bool isSVT, int propagatorIndex, VariablePropagatorManager& manager)
        :Propagator(propagatorIndex, manager)
        ,tIndex(tIndex)
        ,isSVT(isSVT)
        {
            
        }
        
        TMustNotAfterPropagator(const TMustNotAfterPropagator& prop )
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

#endif	/* TRANSITIONMUSTNOTAFTERCHANGEDPROPAGATOR_H */

