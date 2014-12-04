#ifndef PROPAGATORMANAGER_H
#define PROPAGATORMANAGER_H

#include <bitset>
#include <memory>
#include "variableManager.h"
#include "propagator.h"
#include "constants.h"

namespace TRANSPLAN
{

#define RETURN_IF_ES_FAIL(prop_event)		\
  {						\
    if (prop_event == TRANSPLAN::FAIL)		\
      {						\
	PropagatorManager::empty_stacks();	\
	return false;				\
      }						\
  }


#define PROP(idx)      PropagatorManager::propagators[idx]

    class Propagator;
    class VariableManager;
    class Search;

    class PropagatorManager
    {
    public:
        static int propagator_count;

        static std::vector<Propagator * > propagators;
        static BoolVector prop_in_stack;


        //constructor
        PropagatorManager();


        



        //destructor

        static inline void delete_propagators()
        {
            for (int i = 0; i < propagator_count; i++)
            {
                delete PROP(i);
            }

            propagators.clear();
            prop_in_stack.clear();
            propagator_count = 0;
        }

        static bool propagate_all();

        static void attach(int prop_idx);

        static void empty_stacks()
        {
            //sv_trans_stack.clear();
            //res_trans_stack.clear();
            //act_stack.clear();
        }

        static void print_support_matrix();


    };
}

#endif
