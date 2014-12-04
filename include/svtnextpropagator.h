/* 
 * File:   svtnextchangedpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:16 PM
 */

#ifndef SVTNEXTCHANGEDPROPAGATOR_H
#define	SVTNEXTCHANGEDPROPAGATOR_H

#include "propagator.h"

namespace TRANSPLAN
{  

class SVTNextPropagator : public Propagator
{
public:
    int svtIndex;
    
    SVTNextPropagator(int svtIndex, int propagatorIndex, VariablePropagatorManager& manager)
    :Propagator(propagatorIndex, manager)
    ,svtIndex(svtIndex)
    {
            
    }
        
    SVTNextPropagator(const SVTNextPropagator& prop )
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

#endif	/* SVTNEXTCHANGEDPROPAGATOR_H */

