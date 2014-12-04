#include "propagatorManager.h"

#include <ctime>
#include "configure.h"

namespace TRANSPLAN
{

    int PropagatorManager::propagator_count = 0;

    std::vector<Propagator* > PropagatorManager::propagators;

    BoolVector PropagatorManager::prop_in_stack;

    //constructor

    PropagatorManager::PropagatorManager()
    {
        std::cout << "Propagator Manager Created\n";

    }

    void PropagatorManager::addStateVarTransProp(int svt_index)
    {
        if (!SVTRANS(svt_index).is_goal(DUMMY_END))
        {
            int prop_idx = PropagatorManager::propagator_count++;
            VariableManager::next(svt_index).addPropagator(prop_idx); //CSPInteger
        }

        if (!SVTRANS(svt_index).is_init(DUMMY_START))
        {

            int prop_idx = PropagatorManager::propagator_count++;


            VariableManager::prev(svt_index).addPropagator(prop_idx);
            ; //CSPInteger
        }

        int prop_idx = PropagatorManager::propagator_count++;




        VariableManager::svt_start(svt_index).addPropagator(prop_idx); //CSPInterval
        VariableManager::svt_end(svt_index).addPropagator(prop_idx); //CSPInterval
        VariableManager::svt_inplan(svt_index).addPropagator(prop_idx);


    }

    void PropagatorManager::addResourceTransProp(int rt_index)
    {
        int prop_idx = PropagatorManager::propagator_count++;

        VariableManager::rem_demand(rt_index).addPropagator(prop_idx); //CSPInterval
        VariableManager::rem_supp(rt_index).addPropagator(prop_idx); //CSPInterval
        VariableManager::direct_inflow(rt_index).addPropagator(prop_idx); //CSPInterval

        VariableManager::poss_pred(rt_index).addPropagator(prop_idx); //CSPAuxMonoDecSet
        VariableManager::poss_succ(rt_index).addPropagator(prop_idx); //CSPAuxMonoDecSet
        //VariableManager::rt_supp( rt_index ).addPropagator( prop_idx );//CSPInterval
        VariableManager::rt_start(rt_index).addPropagator(prop_idx); //CSPInterval
        VariableManager::rt_end(rt_index).addPropagator(prop_idx); //CSPInterval

        VariableManager::rt_inplan(rt_index).addPropagator(prop_idx); //CSPBoolVar    

    }

    void PropagatorManager::addRTSupportProps()
    {
        //std::cout<<"======> "<< PropagatorManager::propagator_count << std::endl;
        for (int i = 0; i < Transplan::n_r_trans; ++i)
        {
            for (int j = 0; j < Transplan::n_r_trans; ++j)
            {
                if (VariableManager::rt_support_matrix[i][j] != -1)
                    //if (poss_succ.count( RTRANS(j).index)!=0)
                {
                    int prop_idx = PropagatorManager::propagator_count++;


                    VariableManager::rt_supp(i, j).addPropagator(prop_idx); //CSPAuxMonoIncSet 
                }
            }
        }
    }

    void PropagatorManager::addActProp(int act_index)
    {
        int prop_idx = PropagatorManager::propagator_count++;


        VariableManager::a_before(act_index).addPropagator(prop_idx); //CSPAuxMonoIncSet 
        VariableManager::a_after(act_index).addPropagator(prop_idx); //CSPAuxMonoIncSet
        VariableManager::a_not_before(act_index).addPropagator(prop_idx); //CSPAuxMonoIncSet
        VariableManager::a_not_after(act_index).addPropagator(prop_idx); //CSPAuxMonoIncSet

        VariableManager::a_start(act_index).addPropagator(prop_idx); //CSPInterval
        //VariableManager::a_inplan( act_index ).addPropagator( prop_idx );//CSPBoolVar    

        PropagatorManager::addActInplanProp(act_index);

    }

    void PropagatorManager::addActInplanProp(int act_index)
    {
        int prop_idx = PropagatorManager::propagator_count++;

        VariableManager::a_inplan(act_index).addPropagator(prop_idx); //CSPBoolVar    

    }



    //static 

    bool PropagatorManager::propagate_all()
    {
        return true;
    }

    //static 

    void PropagatorManager::attach(int prop_index)
    {

    }

}
