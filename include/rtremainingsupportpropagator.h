/* 
 * File:   rtremainingsupportchangedpropagator.h
 * Author: debdeep
 *
 * Created on 20 November 2014, 6:31 PM
 */

#ifndef RTREMAININGSUPPORTCHANGEDPROPAGATOR_H
#define	RTREMAININGSUPPORTCHANGEDPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{   
    class RTRemainingSupportPropagator: public Propagator
    {
    public:
        int rtIndex;
        
        RTRemainingSupportPropagator(int rtIndex, int propagatorIndex, VariablePropagatorManager& manager)
        :Propagator(propagatorIndex, manager)
        ,rtIndex(rtIndex)
        {
            
        }
        
        RTRemainingSupportPropagator(const RTRemainingSupportPropagator& prop )
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

#endif	/* RTREMAININGSUPPORTCHANGEDPROPAGATOR_H */

