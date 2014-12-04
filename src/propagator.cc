#include "propagator.h"

namespace TRANSPLAN
{
    //constructor

    Propagator::Propagator(int p_idx)
    : p_idx(p_idx)
    {
        //
    }

    //copy constructor

    Propagator::Propagator(const Propagator& pr)
    : p_idx(pr.p_idx)
    {
    }

    //= operator overloading

    Propagator& Propagator::operator=(const Propagator& pr)
    {
        this->prop_index = pr.prop_index;
        return *this;
    }

    //propagation

    PROP_STATUS Propagate::propagate()
    {
        return TRANSPLAN::FIX;
    }


}
