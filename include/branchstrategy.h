#ifndef BRANCH_H_
#define BRANCH_H_

#include<vector>
#include<map>
#include<set>
#include "constants.h"
#include "variablepropagatormanager.h"

namespace TRANSPLAN
{

    class Transplan;
    class VariablePropagatorManager;
    class SearchState;

    class BranchStrategy
    {
    public:

        BranchStrategy()
        {
        }

        virtual BRANCH_TYPE getNextDecision(SearchState* state, IntPair& curr_ass)
        {
        }
        /*
        {
          IntPairt ass;
          ass.first = get_next_var();
          ass.second = get_next_val(ass.first);

          return ass;
        }
         */
        /*
        int get_earliest_end_time();
        int get_earilest_end_cost();
        int get_earliest_end_level();

        int get_latest_end_time();
        int get_latest_end_cost();
        int get_latest_end_level();

        int get_earliest_start_time_val_index(int var_index);   //break ties with earliest end_time
        int get_earliest_end_time_val_index(int var_index);     //break ties with earliest start_time
        int get_earliest_start_cost_val_index(int var_index);   //break ties with earliest end_cost
        int get_earliest_end_cost_val_index(int var_index);     //break ties with earliest start_cost
        int get_earliest_start_level_val_index(int var_index);  //break ties with earliest end_level
        int get_earliest_end_level_val_index(int var_index);    //break ties with earliest start_level

        int get_latest_start_time_val_index(int var_index);     //break ties with latest end_time
        int get_latest_end_time_val_index(int var_index);       //break ties with latest start_time
        int get_latest_start_cost_val_index(int var_index);     //break ties with latest end_cost
        int get_latest_end_cost_val_index(int var_index);       //break ties with latest start_cost
        int get_latest_start_level_val_index(int var_index);    //break ties with latest end_level
        int get_latest_end_level_val_index(int var_index);      //break ties with latest start_level
         */

    };
}

#endif
