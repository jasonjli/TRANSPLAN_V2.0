#ifndef STATEVARIABLE_H
#define STATEVARIABLE_H

#include "statevariablestate.h"
#include "structures.h"

namespace TRANSPLAN
{

    /*!
    Representation for state variables
     */
    class StateVariable
    {
    public:
        std::string name; //name of the state variable
        int index; //integer index
        std::vector<StateVariableState> states;
        std::map<std::string, int> domain; //domain of the state variable

        IntVector transitions; //all transitions related to the state variable
        IntVector acts; //actions on the state variable
        IntIntMap act_trans_map; //action to transition relation
        IntIntMap trans_act_map; //transition to action map

        int domain_size; //domain size
        int init_state_index; //index of 
        int init_trans_index; //initial transition index
        int goal_state_index; //goal value //if -1 then no goal defined
        int goal_trans_index; //goal transition index

        int setup_matrix_id; //setup time and cost matrix id 
        int init_setup_state; //initial set up state
        int goal_setup_state; //goal set up state

        /*
          Constructor
         */
        StateVariable(std::string name, int index)
        : name(name)
        , index(index)
        , domain_size(0)
        , init_state_index(-1)
        , init_trans_index(-1)
        , goal_trans_index(-1)
        , goal_state_index(-1)
        , setup_matrix_id(-1)
        , init_setup_state(0)
        , goal_setup_state(0)
        {
            std::string goal = "G";
            std::string init = "S";

            add_domain_value(init);
            add_domain_value(goal);
        }

        /*
          Copy constructor
         */
        StateVariable(const StateVariable& sv)
        : name(sv.name)
        , index(sv.index)
        , domain(sv.domain)
        , states(sv.states)
        , domain_size(sv.domain_size)
        , transitions(sv.transitions) //all transitions related to the state variable
        , acts(sv.acts)
        , act_trans_map(sv.act_trans_map)
        , trans_act_map(sv.trans_act_map)
        , init_state_index(sv.init_state_index)
        , goal_state_index(sv.goal_state_index)
        , init_trans_index(sv.init_trans_index)
        , goal_trans_index(sv.goal_trans_index)
        , setup_matrix_id(sv.setup_matrix_id)
        , init_setup_state(sv.init_setup_state)
        , goal_setup_state(sv.goal_setup_state)
        {
            //
        }

        /*
          = operator overloading
         */
        StateVariable& operator=(const StateVariable& sv)
        {
            this->name = sv.name;
            this->index = sv.index;
            this->domain = sv.domain;
            this->states = sv.states;
            this->domain_size = sv.domain_size;
            this->transitions = sv.transitions; //all transitions related to the state variable
            this->acts = sv.acts;
            this->act_trans_map = sv.act_trans_map;
            this->trans_act_map = sv.trans_act_map;
            this->init_state_index = sv.init_state_index;
            this->goal_state_index = sv.goal_state_index;
            this->init_trans_index = sv.init_trans_index;
            this->goal_trans_index = sv.goal_trans_index;
            this->setup_matrix_id = sv.setup_matrix_id;
            this->init_setup_state = sv.init_setup_state;
            this->goal_setup_state = sv.goal_setup_state;

            return *this;
        }

        /**
           Set initial state
         */
        void set_init_state(std::string i_state)
        {
            init_state_index = domain[i_state];
        }

        /**
           Set goal state
         */
        void set_goal_state(std::string g_state)
        {
            goal_state_index = domain[g_state];
            states[goal_state_index].is_goal_state = true;
        }

        bool add_domain_value(std::string state)
        {
            NameIndexPair val(state, dom_size());

            return add_domain_value(val);

        }

        /*
          Add domain value. If its duplicate then returns false, otherwise true
         */
        bool add_domain_value(NameIndexPair val)
        {
            if (this->domain.insert(val).second)
            {
                domain_size++;
                StateVariableState state(this, val.first, domain[val.first]);
                states.push_back(state);
                return true;
            }

            return false;
        }

        void add_non_goal_state(std::string state)
        {
            states[ domain[state] ].is_non_goal_state = true;
        }

        bool is_non_goal_state(std::string state)
        {
            return states[ domain[state] ].is_non_goal_state;
        }

        void setMaxPersist(std::string state, int val)
        {
            states[ domain[state] ].max_persist = val;
        }

        int getMaxPersist(int state_idx)
        {
            return states[ state_idx ].max_persist;
        }

        void setMinPersist(std::string state, int val)
        {
            states[ domain[state] ].min_persist = val;
        }

        int getMinPersist(int state_idx)
        {
            return states[ state_idx ].min_persist;
        }

        void setStabilityOffset(std::string state, int val)
        {
            states[ domain[state] ].stability_offset = val;
        }

        int getStabilityOffset(int state_idx)
        {
            return states[ state_idx ].stability_offset;
        }

        std::string& getStateName(int state_idx)
        {
            return states[ state_idx ].name;
        }

        int getStateIndex(std::string state)
        {
            return domain[ state ];
        }

        void addIncomingTransition(int state_idx, int trans_idx)
        {
            states[state_idx].incoming_trans.push_back(trans_idx);
        }

        void addOutgoingTransition(int state_idx, int trans_idx)
        {
            states[state_idx].outgoing_trans.push_back(trans_idx);
        }

        void addPrevailTransition(int state_idx, int trans_idx)
        {
            states[state_idx].prevail_trans.push_back(trans_idx);
        }

        IntVector getIncomingTransitions(int state_idx)
        {
            return states[ state_idx ].incoming_trans;
        }

        IntVector getOutgoingTransitions(int state_idx)
        {
            return states[ state_idx ].outgoing_trans;
        }

        IntVector getPrevailTransitions(int state_idx)
        {
            return states[ state_idx ].prevail_trans;
        }

        /*
          Returns domain size of the state variable
         */
        int dom_size()
        {
            return domain.size();
        }

        /*
          Print a state variable with its domain
         */
        void print()
        {
            std::cout << name << ":" << index << std::endl;
            std::cout << "domain : {";
            std::map<std::string, int>::const_iterator itr = domain.begin();
            for (; itr != domain.end(); ++itr)
            {
                std::cout << itr->first << "(" << itr->second << ")" << " ";
            }
            std::cout << "}\n";
            /*
            std::cout << "Constraints :\n";
            for (int i = 0; i < constraints.size(); ++i)
              constraints[i].print();      
             */
        }

    };
}
#endif
