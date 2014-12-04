/* 
 * File:   actiondistancechangedpropagator.h
 * Author: debdeep
 *
 * Created on 20 November 2014, 6:32 PM
 */

#ifndef ACTIONDISTANCEPROPAGATOR_H
#define	ACTIONDISTANCEPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{
    class ActionDistancePropagator: public Propagator
    {
    public:
        
        ActionDistancePropagator(int propagatorIndex, VariablePropagatorManager& manager)
        :Propagator(propagatorIndex, manager)
        {
            
        }
        
        ActionDistancePropagator(const ActionDistancePropagator& prop )
        :Propagator(prop)
        {
            
        }
        
        PROP_STATUS propagate(SearchState* state)
        {
        	///for each pair (A1,A2)
        	///propagate start times between action distance A2.Start >= A1.Start + dist
        	///propagate transitive closure of distances, a-->c, and c-->d, then update a--->d
            return TRANSPLAN::FIX;
        }
        
    };
}

#endif	/* ACTIONDISTANCECHANGEDPROPAGATOR_H */

