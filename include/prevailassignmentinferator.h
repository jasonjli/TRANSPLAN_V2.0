/* 
 * File:   prevailassignmentinferator.h
 * Author: debdeep
 *
 * Created on 22 November 2014, 9:59 AM
 */

#ifndef PREVAILASSIGNMENTINFERATOR_H
#define	PREVAILASSIGNMENTINFERATOR_H

#include "inferator.h"

namespace TRANSPLAN
{

class PrevailAssignmentInferator: public Inferator
{
public:
    int svtIndex;
    
    PrevailAssignmentInferator(int inferatorIndex, int svtIndex, VariablePropagatorManager& manager)
    :Inferator(inferatorIndex, manager)
    ,svtIndex(svtIndex)
    {
        
    }
    
    PrevailAssignmentInferator(const PrevailAssignmentInferator& infer)
    :Inferator(infer)
    ,svtIndex(svtIndex)
    {
        
    }
    
    PROP_STATUS infer(SearchState* state)
    {
        return TRANSPLAN::FIX;
    }
};
}


#endif	/* PREVAILASSIGNMENTINFERATOR_H */

