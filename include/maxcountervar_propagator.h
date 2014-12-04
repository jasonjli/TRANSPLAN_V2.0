#ifndef MAXCOUNTERVAR_PROPAGATOR_H
#define MAXCOUNTERVAR_PROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{

    class MaxCounterPropagator : public Propagator
    {
    public:
        //constructor
        MaxCounterPropagator(int idx, int v_idx);

        //copy constructor
        MaxCounterPropagator(const MaxCounterPropagator& pr);

        //= operator overloading
        MaxCounterPropagator& operator=(const MaxCounterPropagator& pr);

        //propagation
        PROP_STATUS propagate();
    };
}
#endif
