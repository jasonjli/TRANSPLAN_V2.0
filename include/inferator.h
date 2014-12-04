#ifndef INFERATOR_H
#define INFERATOR_H

#include <iostream>
#include <cstdlib>
#include "constants.h"
#include <ctime>

#include "cspintvar.h"
#include "cspintervalvar.h"
#include "cspboolvar.h"
#include "cspauxsetvar.h"
#include "cspauxsetmonotonicincvar.h"
#include "cspauxsetmonotonicdecvar.h"
#include "cspauxintvar.h"
#include "cspmaxcountervar.h"
#include "cspgraphvar.h"

#include "variablepropagatormanager.h"

namespace TRANSPLAN
{

    class SearchState;
    class VariablePropagatorManager;

    class Inferator
    {
        
    public:
        SearchState* currentState;
        int inferIndex;
        VariablePropagatorManager& manager;

        Inferator(int inferIndex, VariablePropagatorManager& manager)
        : inferIndex(inferIndex)
        , manager(manager)
        {
            //do nothing
        }

        Inferator(const Inferator& infer)
        : inferIndex(infer.inferIndex)
        , manager(infer.manager)
        {
            //do nothing
        }

        virtual PROP_STATUS infer(SearchState* state)=0;

    };
}
#endif
