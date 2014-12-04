/* 
 * File:   transitionmustbeafterpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:18 PM
 */

#ifndef TRANSITIONMUSTBEAFTERPROPAGATOR_H
#define	TRANSITIONMUSTBEAFTERPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{   
    class TMustBeAfterPropagator: public Propagator
    {
    public:
        int tIndex;
        bool isSVT;
        
        TMustBeAfterPropagator(int tIndex, bool isSVT, int propagatorIndex, VariablePropagatorManager& manager)
        :Propagator(propagatorIndex, manager)
        ,tIndex(tIndex)
        ,isSVT(isSVT)
        {
            
        }
        
        TMustBeAfterPropagator(const TMustBeAfterPropagator& prop )
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

#endif	/* TRANSITIONMUSTBEAFTERPROPAGATOR_H */

