#ifndef STATEVARIABLESTATE_H
#define STATEVARIABLESTATE_H

#include "statevariable.h"
#include "structures.h"

namespace TRANSPLAN
{
    class StateVariable;

    /*!
    Representation for state variables
     */
    class StateVariableState
    {
    public:
        StateVariable* statevar;
        std::string name; //name of the state variable
        int index; //integer index
        int achieve_before; // after this time no transition can achieve this state
        int achieve_after; // transitions can only achieve this state after this time
        int change_before; // after this time no transition can change this state to other state
        int change_after; // transitions can change this state to other state after this time
        int min_persist; // once achieved, the state can't be changed at least this period of time
        int max_persist; // once achived, the state must be changed within this period of time
        int min_change; // minimum number of time this state can be changed to other state
        int max_change;
        int min_achieve; // minimum number of time this state can be achieved from other state
        int max_achieve;
        int stability_offset; // this is the minimum time that must be elapsed after achievement of the state, before any transition can use it (any PREV or EFFT)

        bool is_goal_state;
        bool is_non_goal_state;
        bool is_init_state;

        IntVector incoming_trans; //incoming transitions 
        IntVector outgoing_trans; //outgoing transitions
        IntVector prevail_trans; //prevail transitions

        /*
          Constructor
         */
        StateVariableState(StateVariable* var, std::string name, int index)
        : statevar(var)
        , name(name)
        , index(index)
        , achieve_before(TRANSPLAN::DEFAULT_MAX_TIME)
        , achieve_after(TRANSPLAN::DEFAULT_MIN_TIME)
        , change_before(TRANSPLAN::DEFAULT_MAX_TIME)
        , change_after(TRANSPLAN::DEFAULT_MIN_TIME)
        , min_persist(TRANSPLAN::DEFAULT_MIN_TIME)
        , max_persist(TRANSPLAN::DEFAULT_MAX_TIME)
        , min_change(TRANSPLAN::DEFAULT_MIN)
        , max_change(TRANSPLAN::DEFAULT_MAX)
        , min_achieve(TRANSPLAN::DEFAULT_MIN)
        , max_achieve(TRANSPLAN::DEFAULT_MAX)
        , stability_offset(TRANSPLAN::DEFAULT_MIN_TIME)
        , is_goal_state(false)
        , is_non_goal_state(false)
        , is_init_state(false)
        {

        }

        /*
          Copy constructor
         */
        StateVariableState(const StateVariableState& sv)
        : statevar(sv.statevar)
        , name(sv.name)
        , index(sv.index)
        , achieve_before(sv.achieve_before)
        , achieve_after(sv.achieve_after)
        , change_before(sv.change_before)
        , change_after(sv.change_after)
        , min_persist(sv.min_persist)
        , max_persist(sv.max_persist)
        , min_change(sv.min_change)
        , max_change(sv.max_change)
        , min_achieve(sv.min_achieve)
        , max_achieve(sv.max_achieve)
        , stability_offset(sv.stability_offset)
        , is_goal_state(sv.is_goal_state)
        , is_non_goal_state(sv.is_non_goal_state)
        , is_init_state(sv.is_init_state)
        , incoming_trans(sv.incoming_trans)
        , outgoing_trans(sv.outgoing_trans)
        , prevail_trans(sv.prevail_trans)
        {
            //
        }

        /*
          = operator overloading
         */
        StateVariableState& operator=(const StateVariableState& sv)
        {
            this->statevar = sv.statevar;
            this->name = sv.name;
            this->index = sv.index;
            this->achieve_before = sv.achieve_before;
            this->achieve_after = sv.achieve_before;
            this->change_before = sv.change_before;
            this->change_after = sv.change_after;
            this->min_persist = sv.min_persist;
            this->max_persist = sv.max_persist;
            this->min_change = sv.min_change;
            this->max_change = sv.max_change;
            this->min_achieve = sv.min_achieve;
            this->max_achieve = sv.max_achieve;
            this->stability_offset = sv.stability_offset;

            this->is_goal_state = sv.is_goal_state;
            this->is_non_goal_state = sv.is_non_goal_state;
            this->is_init_state = sv.is_init_state;

            this->incoming_trans = sv.incoming_trans;
            this->outgoing_trans = sv.outgoing_trans;
            this->prevail_trans = sv.prevail_trans;

            return *this;
        }

    };
}
#endif
