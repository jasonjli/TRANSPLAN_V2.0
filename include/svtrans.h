#ifndef SVTRANS_H
#define SVTRANS_H

#include "transplan.h"
#include "constants.h"
//#include "resource.h"

namespace TRANSPLAN
{
    class Transplan;
    class Resource;

    class SVTrans
    {
    public:
        int index; //gloabl index of the transiton
        int duration; //duration of the transition
        int sv_index; //state variable index
        int a_index; //corresponding action index
        int offset; /// offset value from the start of the action
        int setup_state; /// setup state of the transition

        TRANS_TYPE type; //type of transition
        int need; //if sv prevail trans then state index of the state variable
        int pre; //if sv effect trans, then pair of state from
        int post; //if sv effect trans, then pair of state to

        IntSet poss_pred; //possible supporters
        IntSet poss_succ; //possible followers

        bool is_dummy;

        /*!
          Constructor
         */
        SVTrans(int index, int sv_index, int a_index, int from, int to, int dur, int offset, int setup_state)
        : index(index)
        , sv_index(sv_index)
        , a_index(a_index)
        , pre(from)
        , post(to)
        , duration(dur)
        , offset(offset)
        , setup_state(setup_state)
        , is_dummy(false)
        {
            //
            if (pre == post)
            {
                type = TRANSPLAN::PRV;
                need = pre;
            } else
            {
                type = TRANSPLAN::EFFT;
                need = -1;
            }
            assert(setup_state >= 0);
        }

        /*!
          Copy constructor
         */
        SVTrans(const SVTrans& trans)
        : index(trans.index)
        , duration(trans.duration)
        , offset(trans.offset)
        , sv_index(trans.sv_index)
        , a_index(trans.a_index)
        , type(trans.type)
        , need(trans.need)
        , pre(trans.pre)
        , post(trans.post)
        , poss_pred(trans.poss_pred)
        , poss_succ(trans.poss_succ)
        , setup_state(trans.setup_state)
        , is_dummy(trans.is_dummy)
        {
            //
        }

        /*!
          = operator overloading
         */
        SVTrans& operator=(const SVTrans& trans)
        {
            this->index = trans.index;
            this->sv_index = trans.sv_index;
            this->a_index = trans.a_index;
            this->duration = trans.duration;
            this->offset = trans.offset;
            this->type = trans.type;
            this->need = trans.need;
            this->pre = trans.pre;
            this->post = trans.post;
            this->poss_pred = trans.poss_pred;
            this->poss_succ = trans.poss_succ;
            this->setup_state = trans.setup_state;
            this->is_dummy = trans.is_dummy;
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
