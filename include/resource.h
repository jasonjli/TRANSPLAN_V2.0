#ifndef RESOURCE_H
#define RESOURCE_H

#include "structures.h"

namespace TRANSPLAN
{

    class Resource
    {
    public:
        std::string name;
        int index; //integer index
        int cap; //capacity of resource
        int init_level; /// initial level of resource    |
        int goal_level_min; // goal level min (default 0 )   |-> only applies to Reservoir resources
        int goal_level_max; // goal level max ( default cap) |
        int dummy_s_index;
        int dummy_e_index;
        IntVector transitions; //all transitions related to the state variable
        IntVector acts; //actions that have transitions on the resource
        RES_TYPE type;
        IntIntMap act_trans_map;
        IntIntMap trans_act_map;
        IntVector borrow_trans;
        IntVector prod_trans;
        IntVector cons_trans;

        ///for reservoir resources
        int dummy_start_prod_idx;
        int dummy_start_cons_idx;

        int dummy_end_prod_idx;
        int dummy_end_cons_idx;

        int setup_matrix_id;
        int init_setup_state;
        int goal_setup_state;

        /*!
          Constructor
         */
        Resource(std::string name, int idx, int cap, RES_TYPE type)
        : name(name)
        , index(idx)
        , cap(cap)
        , type(type)
        , init_level(cap)
        , goal_level_min(0)
        , goal_level_max(cap)
        , dummy_s_index(-1)
        , dummy_e_index(-1)
        , dummy_start_prod_idx(-1)
        , dummy_start_cons_idx(-1)
        , dummy_end_prod_idx(-1)
        , dummy_end_cons_idx(-1)
        , setup_matrix_id(-1)
        , init_setup_state(0)
        , goal_setup_state(0)
        {
            //
        }

        /*!
          Copy Constructor
         */
        Resource(const Resource& res)
        : name(res.name)
        , index(res.index)
        , cap(res.cap)
        , init_level(res.init_level)
        , goal_level_max(res.goal_level_max)
        , goal_level_min(res.goal_level_min)
        , dummy_s_index(res.dummy_s_index)
        , dummy_e_index(res.dummy_e_index)
        , transitions(res.transitions)
        , acts(res.acts)
        , type(res.type)
        , act_trans_map(res.act_trans_map)
        , trans_act_map(res.trans_act_map)
        , borrow_trans(res.borrow_trans)
        , prod_trans(res.prod_trans)
        , cons_trans(res.cons_trans)
        , dummy_start_prod_idx(res.dummy_start_prod_idx)
        , dummy_start_cons_idx(res.dummy_start_cons_idx)
        , dummy_end_prod_idx(res.dummy_end_prod_idx)
        , dummy_end_cons_idx(res.dummy_end_cons_idx)
        , setup_matrix_id(res.setup_matrix_id)
        , init_setup_state(res.init_setup_state)
        , goal_setup_state(res.goal_setup_state)
        {
            //
        }

        /*!
          = Operator overloading
         */
        Resource& operator=(const Resource& res)
        {
            this->name = res.name;
            this->index = res.index;
            this->cap = res.cap;
            this->transitions = res.transitions;
            this->dummy_s_index = res.dummy_s_index;
            this->dummy_e_index = res.dummy_e_index;
            this->transitions = res.transitions;
            this->acts = res.acts;
            this->type = res.type;
            this->act_trans_map = res.act_trans_map;
            this->trans_act_map = res.trans_act_map;
            this->borrow_trans = res.borrow_trans;
            this->prod_trans = res.prod_trans;
            this->cons_trans = res.cons_trans;
            this->init_level = res.init_level;
            this->goal_level_min = res.goal_level_min;
            this->goal_level_max = res.goal_level_max;
            this->dummy_start_prod_idx = res.dummy_start_prod_idx;
            this->dummy_start_prod_idx = res.dummy_start_cons_idx;
            this->dummy_end_prod_idx = res.dummy_end_prod_idx;
            this->dummy_end_cons_idx = res.dummy_end_cons_idx;
            this->setup_matrix_id = res.setup_matrix_id;
            this->init_setup_state = res.init_setup_state;
            this->goal_setup_state = res.goal_setup_state;

            return *this;
        }

        bool is_init(int idx)
        {
            return (idx == this->dummy_s_index);
        }

        bool is_goal(int idx)
        {
            return (idx == this->dummy_e_index);
        }

        bool is_dummy_s_prod(int idx)
        {
            return (idx == this->dummy_start_prod_idx);
        }

        bool is_dummy_s_cons(int idx)
        {
            return (idx == this->dummy_start_cons_idx);
        }

        bool is_dummy_e_prod(int idx)
        {
            return (idx == this->dummy_end_prod_idx);
        }

        bool is_dummy_e_cons(int idx)
        {
            return (idx == this->dummy_end_cons_idx);
        }

    };
}
#endif
