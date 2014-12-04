#ifndef TRANSITION_H
#define TRANSITION_H

#include "transplan.h"
#include "constants.h"
//#include "resource.h"

namespace TRANSPLAN
{
    class Transplan;
    class Resource;

    class Transition
    {
    public:
        int index; //gloabl index of the transiton
        int sv_index; //state variable index
        int r_index; //corresponding resource index
        int a_index; //corresponding action index

        TRANS_TYPE type; //type of transition
        int req; //if resource trans then resource requirement
        int need; //if sv prevail trans then state index of the state variable
        int pre; //if sv effect trans, then pair of state from
        int post; //if sv effect trans, then pair of state to

        bool dummy_start; //is dummy start
        bool dummy_end; //is dummy end


        IntSet poss_pred; //possible supporters
        IntSet poss_succ; //possible followers

        /*!
          Constructor
         */
        Transition(int index)
        : index(index)
        {
            //
        }

        /*!
          Copy constructor
         */
        Transition(const Transition& trans)
        : index(trans.index)
        , sv_index(trans.sv_index)
        , r_index(trans.r_index)
        , a_index(trans.a_index)
        , type(trans.type)
        , req(trans.req)
        , need(trans.need)
        , pre(trans.pre)
        , post(trans.post)
        , poss_pred(trans.poss_pred)
        , poss_succ(trans.poss_succ)
        {
            //
        }

        /*!
          = operator overloading
         */
        Transition& operator=(const Transition& trans)
        {
            this->index = trans.index;
            this->sv_index = trans.sv_index;
            this->r_index = trans.r_index;
            this->a_index = trans.a_index;
            this->type = trans.type;
            this->req = trans.req;
            this->need = trans.need;
            this->pre = trans.pre;
            this->post = trans.post;

            this->poss_pred = trans.poss_pred;
            this->poss_succ = trans.poss_succ;
            return *this;
        }

        bool is_init(int dummy_start_act_id)
        {
            return a_index == dummy_start_act_id;
        }

        bool is_goal(int dummy_end_act_id)
        {
            return a_index == dummy_end_act_id;
        }

    };
}
#endif
