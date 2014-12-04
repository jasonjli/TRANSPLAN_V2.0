/* 
 * File:   transitionstarttimechangedpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:34 PM
 */

#ifndef TRANSITIONSTARTTIMEPROPAGATOR_H
#define	TRANSITIONSTARTTIMEPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{   
    class TStartTimePropagator: public Propagator
    {
    public:
        int tIndex;
        bool isSVT;
        
        TStartTimePropagator(int tIndex, bool isSVT, int propagatorIndex, VariablePropagatorManager& manager)
        :Propagator(propagatorIndex, manager)
        ,tIndex(tIndex)
        ,isSVT(isSVT)
        {
            
        }
        
        TStartTimePropagator(const TStartTimePropagator& prop )
        :Propagator(prop)
        ,tIndex(prop.tIndex)
        ,isSVT(prop.isSVT)
        {
            
        }
        
        PROP_STATUS propagate(SearchState* state)
        {
        	this->currentState = state;
        	//if isMaxUpdated
        	//if isMinUpdated
            return TRANSPLAN::FIX;
        }
        
    };
}

#endif	/* TRANSITIONSTARTTIMECHANGEDPROPAGATOR_H */

