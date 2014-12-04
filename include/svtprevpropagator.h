/* 
 * File:   svtprevchangedpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:17 PM
 */

#ifndef SVTPREVCHANGEDPROPAGATOR_H
#define	SVTPREVCHANGEDPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{
    class SVTPrevPropagator: public Propagator
    {
    public:
        int svtIndex;
        
        SVTPrevPropagator(int sctIndex, int propagatorIndex, VariablePropagatorManager& manager)
        :Propagator(propagatorIndex, manager)
        ,svtIndex(svtIndex)
        {
            
        }
        
        SVTPrevPropagator(const SVTPrevPropagator& prop )
        :Propagator(prop)
        ,svtIndex(prop.svtIndex)
        {
            
        }
        
        PROP_STATUS propagate(SearchState* state)
        {
        	this->currentState = state;
            return TRANSPLAN::FIX;
        }
        
    };
}

#endif	/* SVTPREVCHANGEDPROPAGATOR_H */

