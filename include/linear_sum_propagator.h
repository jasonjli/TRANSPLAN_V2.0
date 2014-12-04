#ifndef LINEAR_SUM_PROPAGATOR_H
#define LINEAR_SUM_PROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{

    class LinearSumPropagator : public Propagator
    {
    public:
        int A;
        int C;
        int B;
        IntPair b;
        NUM_OP op;

        //constructor
        LinearSumPropagator(int idx, int A, int C, IntPair b, NUM_OP op);

        LinearSumPropagator(int idx, int A, int C, int B, NUM_OP op);

        //copy constructor
        LinearSumPropagator(const LinearSumPropagator& pr);

        //= operator overloading
        LinearSumPropagator& operator=(const LinearSumPropagator& pr);

        void initialize_previous_values();
        //propagation
        PROP_STATUS propagate();
        /*
        //update a+b == c
        PROP_STATUS update_EQ();
        //update a+b <= c
        PROP_STATUS update_LQ();
        //update a+b >= c
        PROP_STATUS update_GQ();
         */

    };
}
#endif
