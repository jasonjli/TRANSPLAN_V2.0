#ifndef RESOURCETRANS_H
#define RESOURCETRANS_H

#include "transplan.h"
#include "constants.h"
//#include "resource.h"

namespace TRANSPLAN
{
    class Transplan;
    class Resource;

    class RTrans
    {
    public:
        int index; //gloabl index of the transiton
        int duration; //duration
        int r_index; //corresponding resource index
        int a_index; //corresponding action index
        int offset;
        int setup_state;
        int mode;

        TRANS_TYPE type; //type of transition
        int req; //if resource trans then resource requirement

        IntSet poss_pred; //possible supporters
        IntSet poss_succ; //possible followers

        IntSet mutex_trans; //mutex transitions

        bool is_dummy;

        /*!
          Constructor
         */
        RTrans(int index, int r_index, int a_index, int req, TRANS_TYPE type, int dur, int offset, int setup_state, int mode)
        : index(index)
        , r_index(r_index)
        , a_index(a_index)
        , req(req)
        , type(type)
        , duration(dur)
        , offset(offset)
        , setup_state(setup_state)
        , mode(mode)
        , is_dummy(false)
        {
            //
            assert(setup_state >= 0);
        }

        /*!
          Copy constructor
         */
        RTrans(const RTrans& trans)
        : index(trans.index)
        , duration(trans.duration)
        , offset(trans.offset)
        , setup_state(trans.setup_state)
        , mode(trans.mode)
        , r_index(trans.r_index)
        , a_index(trans.a_index)
        , type(trans.type)
        , req(trans.req)
        , poss_pred(trans.poss_pred)
        , poss_succ(trans.poss_succ)
        , mutex_trans(trans.mutex_trans)
        , is_dummy(trans.is_dummy)
        {
            //
        }

        /*!
          = operator overloading
         */
        RTrans& operator=(const RTrans& trans)
        {
            this->index = trans.index;
            this->duration = trans.duration;
            this->offset = trans.offset;
            this->setup_state = trans.setup_state;
            this->mode = trans.mode;
            this->r_index = trans.r_index;
            this->a_index = trans.a_index;
            this->type = trans.type;
            this->req = trans.req;
            this->poss_pred = trans.poss_pred;
            this->poss_succ = trans.poss_succ;
            this->mutex_trans = trans.mutex_trans;
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
