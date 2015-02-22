#ifndef ACTIVITY_H
#define ACTIVITY_H

#include "structures.h"
#include "constants.h"
#include <vector>
#include <cstdlib>

namespace TRANSPLAN
{

    class Activity
    {
    public:
        std::string name; //name of the action
        int index; //index of the action;
        int release_date; //can only start at this time
        int deadline; //all transitions must finish before this date
        IntSet mutex; //mutex set of action

        IntVector sv_trans; //all state variable transitions of this activity
        IntVector r_trans; //all resource transitions of this activity

        IntMatrix res_trans_map; //for each resource stores list of resource transitions' index
        IntMatrix sv_trans_map; //for each state variable stores list of transitions' index

        /*!
          Action constructor
         */
        Activity(std::string name,
                int index,
                int n_sv,
                int n_res,
                int release_d = TRANSPLAN::DEFAULT_RELEASE_DATE,
                int deadline_d = TRANSPLAN::DEFAULT_DEADLINE)
        : index(index)
        , name(name)
        , release_date(release_d)
        , deadline(deadline_d)
        {
            //
            //res_trans_map = IntVector();
            for (int i = 0; i < n_res; i++)
            {
                res_trans_map.push_back(IntVector());
            }

            // sv_trans_map = IntVector();
            for (int i = 0; i < n_sv; i++)
            {
                sv_trans_map.push_back(IntVector());
            }
        }

        /*!
          Copy constructor
         */
        Activity(const Activity& act)
        : index(act.index)
        , name(act.name)
        , sv_trans(act.sv_trans)
        , r_trans(act.r_trans)
        , release_date(act.release_date)
        , deadline(act.deadline)
        , res_trans_map(act.res_trans_map)
        , sv_trans_map(act.sv_trans_map)
        , mutex(act.mutex)
        {
            //

        }

        /*!
          = operator overloading
         */
        Activity& operator=(const Activity& act)
        {
            this->index = act.index;
            this->name = act.name;
            this->sv_trans = act.sv_trans;
            this->r_trans = act.r_trans;
            this->release_date = act.release_date;
            this->deadline = act.deadline;
            this->res_trans_map = act.res_trans_map;
            this->sv_trans_map = act.sv_trans_map;
            this->mutex = act.mutex;

            return *this;
        }

        IntVector getTransitionsOnDomainObject(int objIndex, bool isSV )
        {
        	if( isSV )
        	{
        		return sv_trans_map[objIndex];
        	}
        	else
        	{
        		return res_trans_map[objIndex];
        	}
        }



    };

}
#endif
