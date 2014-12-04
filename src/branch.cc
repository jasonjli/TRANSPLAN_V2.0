#include "branch.h"
#include "transplan.h"
#include "search.h"
#include <cmath>


namespace TRANSPLAN
{

    Branch::Branch(Search* search)
    : search(search)
    {
    }

    Branch::~Branch()
    {
        //do nothing
    }

    bool Branch::get_next_assignment(Triplet& assign)
    {
        //return get_next_assignment_rand(assign);
        //return get_next_assignment_H1(assign);
        //return get_next_assignment_H2(assign);
        return get_next_assignment_VRP_Simple(assign);

    }

    bool Branch::get_next_assignment_rand(Triplet& assign)
    {
        //std::cout << "Starting branching .....\n";
        bool trans;
        int var_index = TRANSPLAN::CSP_NO_NEXT_VAR;
        int val_index = -1;

        int suffix;
        int prefix;
        IntSet potential_act_set;

        int min_eet_res = INT_MAX;

        for (int i = 0; i < Transplan::n_resources; ++i)
        {
            //std::cout << "Resource "<< Transplan::resources[i].name << std::endl; 
            trans = false;
            int min_eet_next = INT_MAX;
            int min_active_est = INT_MAX;
            int min_active_index = -1;

            suffix = current_path_suffix_res(i).val();
            //std::cout <<"Suffix = " << suffix << std::endl;
            prefix = current_path_prefix_res(i).val();
            //std::cout <<"Prefix = " << prefix << std::endl;
            //IntSet next_front = next_frontier(suffix, trans).dom();
            //Utils::print_set(next_front);

            //std::cout << suffix << std::endl;
            if (!next(suffix, trans).assigned())
            {
                //std::cout <<"Suffix = " << suffix << std::endl;
                //std::cout <<"Prefix = " << prefix << std::endl;
                //IntSet next_front = next_frontier(suffix, trans).dom();
                IntSet next_dom = next(suffix, trans).dom();
                //Utils::print_set(next_dom);
                IntSet active_set;
                for (IntSet::iterator itr = next_dom.begin(); itr != next_dom.end(); ++itr)
                {
                    if (*itr != prefix && prev(*itr, trans).active())
                    {
                        //std::cout << "Inserting " << *itr << std::endl;
                        active_set.insert(*itr);
                        int est = start_time(*itr, trans).min();

                        if (min_active_est > est)
                        {
                            min_active_est = est;
                            min_active_index = *itr;
                        }
                    }
                }

                if (!active_set.empty())
                {
                    bool none_exists = true;

                    for (IntSet::iterator itr = next_dom.begin(); itr != next_dom.end(); ++itr)
                    {
                        int eet = end_time(*itr, trans).min();

                        if (active_set.count(*itr) == 0)
                        {
                            for (IntSet::iterator itr_in = active_set.begin(); itr_in != active_set.end(); ++itr_in)
                            {
                                int est = start_time(*itr_in, trans).min();
                                //std::cout << "Setup time " << *itr << " -> " << *itr_in << std::endl;
                                int finish = eet + get_setup_time(*itr, *itr_in, trans);
                                if (finish <= est)
                                {
                                    //store *itr,
                                    int act_index = RESREQ_ACT_INDEX(*itr);
                                    potential_act_set.insert(act_index);
                                    //std::cout << "RR " << *itr << " exists in between...\n";
                                    none_exists = false;
                                }
                            }
                        }
                    }

                    if (none_exists)
                    {
                        //return variable assignment
                        //suffix = min_active_index
                        int eet_res = end_time(suffix, trans).min();
                        if (min_eet_res > eet_res)
                        {
                            min_eet_res = eet_res;
                            var_index = suffix;
                            val_index = min_active_index;
                        } else if (min_eet_res == eet_res)
                        {
                            int est_next_res_val_min = start_time(val_index, trans).min();
                            int est_next_res = start_time(min_active_index, trans).min();

                            if (est_next_res_val_min > est_next_res)
                            {
                                var_index = suffix;
                                val_index = min_active_index;
                            }
                        }

                    }
                }
            }
        }

        if (var_index != TRANSPLAN::CSP_NO_NEXT_VAR)
        {
            assign.lhs = var_index;
            assign.rhs = val_index;
            assign.type = trans;

            return true;
        }

        //std::exit(0);
        //***************************************************************************************************************
        // **************************** For variables ********************************************************************



        int min_eet = INT_MAX;
        int min_val_lst = INT_MAX;
        int min_val_est = INT_MAX;
        int min_val_index = -1;
        int var_index_diff = -1;
        trans = true;

        IntVector active_svs;
        for (int i = 0; i < Transplan::n_variables; ++i)
        {
            suffix = current_path_suffix_sv(i).val();
            prefix = current_path_prefix_sv(i).val();

            if (!next(suffix, trans).assigned())
            {
                active_svs.push_back(suffix);
            }
        }

        if (!active_svs.empty())
        {
            int size = active_svs.size();
            var_index = active_svs[Utils::randint(size - 1)];
            IntVector next_dom_vec;
            IntSet next_dom = next(var_index, trans).dom();

            for (IntSet::iterator itr = next_dom.begin(); itr != next_dom.end(); ++itr)
            {
                next_dom_vec.push_back(*itr);
            }

            Utils::initrand();

            val_index = next_dom_vec[Utils::randint(next_dom_vec.size() - 1)];
        }

        if (var_index == TRANSPLAN::CSP_NO_NEXT_VAR)
        {
            return false;
        } else
        {
            //std::cout << "Selected var index = " << var_index << std::endl;
            assign.lhs = var_index;
            assign.rhs = val_index;
            assign.type = trans;
        }

        return true;

    }

    bool Branch::get_next_assignment_H1(Triplet& assign)
    {
        //std::cout << "Starting branching .....\n";
        bool trans;
        int var_index = TRANSPLAN::CSP_NO_NEXT_VAR;
        int val_index = -1;

        int suffix;
        int prefix;
        IntSet potential_act_set;

        for (int i = 0; i < Transplan::n_resources; ++i)
        {
            //std::cout << "Resource "<< Transplan::resources[i].name << std::endl; 
            trans = false;
            int min_eet_next = INT_MAX;
            int min_active_est = INT_MAX;
            int min_active_index = -1;

            suffix = current_path_suffix_res(i).val();
            //std::cout <<"Suffix = " << suffix << std::endl;
            prefix = current_path_prefix_res(i).val();
            //std::cout <<"Prefix = " << prefix << std::endl;


            std::cout << "Res " << i << std::endl;

            IntSet next_front = next_frontier(suffix, trans).dom();
            std::cout << "\tNext frontier size = " << next_front.size() << std::endl;
            if (next(suffix, trans).assigned())
            {
                //std::cout << "\tSuffix assigned to = " << next(suffix, trans).val() << std::endl;
                if (next(suffix, trans).val() != -1)
                    std::exit(0);
            }

            IntSet prev_front = previous_frontier(prefix, trans).dom();
            //std::cout << "\tPrev frontier size = " << prev_front.size() <<" [" <<prev(prefix, trans).assigned() 
            //<< "]"<< std::endl;
            if (prev(prefix, trans).assigned())
            {
                //std::cout << "\tPrefix assigned to = " << prev(prefix, trans).val() << std::endl;
                if (prev(prefix, trans).val() != -1)
                    std::exit(0);
            }


            //if (prev_front.size() == 0)
            //std::exit(0);



            //Utils::print_set(next_front);

            //std::cout << suffix << std::endl;
            if (!next(suffix, trans).assigned())
            {
                //std::cout <<"Suffix = " << suffix << std::endl;
                //std::cout <<"Prefix = " << prefix << std::endl;
                //IntSet next_front = next_frontier(suffix, trans).dom();
                IntSet next_dom = next(suffix, trans).dom();
                //Utils::print_set(next_front);
                IntSet active_set;
                for (IntSet::iterator itr = next_dom.begin(); itr != next_dom.end(); ++itr)
                {
                    if (*itr != prefix && prev(*itr, trans).active())
                    {
                        //std::cout << "Inserting " << *itr << std::endl;
                        active_set.insert(*itr);
                        int est = start_time(*itr, trans).min();

                        if (min_active_est > est)
                        {
                            min_active_est = est;
                            min_active_index = *itr;
                        }
                    }
                }

                if (!active_set.empty())
                {
                    bool none_exists = true;

                    for (IntSet::iterator itr = next_dom.begin(); itr != next_dom.end(); ++itr)
                    {
                        int eet = end_time(*itr, trans).min();

                        if (active_set.count(*itr) == 0)
                        {
                            for (IntSet::iterator itr_in = active_set.begin(); itr_in != active_set.end(); ++itr_in)
                            {
                                int est = start_time(*itr_in, trans).min();
                                //std::cout << "Setup time " << *itr << " -> " << *itr_in << std::endl;
                                int finish = eet + get_setup_time(*itr, *itr_in, trans);
                                if (finish <= est)
                                {
                                    //store *itr,
                                    int act_index = RESREQ_ACT_INDEX(*itr);
                                    potential_act_set.insert(act_index);
                                    //std::cout << "RR " << *itr << " exists in between...\n";
                                    none_exists = false;
                                }
                            }
                        }
                    }

                    if (none_exists)
                    {
                        //return variable assignment
                        //suffix = min_active_index
                        var_index = suffix;
                        val_index = min_active_index;

                        assign.lhs = var_index;
                        assign.rhs = val_index;
                        assign.type = trans;

                        return true;
                    }
                }
            }
        }

        //std::exit(0);
        //***************************************************************************************************************
        //**************************** For variables ********************************************************************

        std::cout << "Potential Act Set = " << potential_act_set.size() << std::endl;

        int min_eet = INT_MAX;
        int min_val_lst = INT_MAX;
        int min_val_est = INT_MAX;
        int min_val_index = -1;
        int var_index_diff = -1;
        trans = true;

        for (int i = 0; i < Transplan::n_variables; ++i)
        {
            suffix = current_path_suffix_sv(i).val();
            prefix = current_path_prefix_sv(i).val();

            if (!next(suffix, trans).assigned())
            {
                int sv_index = TRANS_VAR_INDEX(suffix);
                std::string name = Transplan::state_variables[sv_index].name;
                int local_min_val_lst = INT_MAX;
                int local_min_val_est = INT_MAX;
                int local_min_val_lst_index = -1;
                int local_min_val_est_index = -1;

                //get the min_val_lft and min_val_index
                IntSet next_dom = next(suffix, trans).dom();

                for (IntSet::iterator itr = next_dom.begin(); itr != next_dom.end(); ++itr)
                {
                    int lst = start_time(*itr, trans).max();
                    int est = start_time(*itr, trans).min();

                    if (local_min_val_lst > lst)
                    {
                        local_min_val_lst = lst;
                        local_min_val_lst_index = *itr;
                    }

                    if (local_min_val_est > est)
                    {
                        local_min_val_est = est;
                        local_min_val_est_index = *itr;
                    }
                }
                //est of sv
                int eet = end_time(suffix, trans).min();

                //calculate diff to goal
                //std::cout << "Current Path prefix = " << prefix << std::endl;
                int pr_est = start_time(prefix, trans).max(); //min();
                int diff = pr_est - eet;
                //std::cout << "Difference " << "(" << suffix << ")" <<" = " << diff << std::endl;



                //std::cout << "compare " << min_eet << " > " << eet  << " for var_index " 
                //  << suffix << "(" << name << ")" << std::endl ;

                if (min_eet > eet)
                {
                    min_eet = eet;
                    min_val_lst = local_min_val_lst;
                    min_val_est = local_min_val_est;
                    var_index = suffix;
                    val_index = local_min_val_est_index;
                    var_index_diff = diff;
                } else if (min_eet == eet)
                {

                    if (var_index_diff < diff)
                    {
                        min_val_lst = local_min_val_lst;
                        min_val_est = local_min_val_est;
                        var_index = suffix;
                        val_index = local_min_val_est_index;
                        var_index_diff = diff;
                    } else if (var_index_diff == diff)
                    {

                        //std::cout << "Here...\n";
                        if (min_val_est > local_min_val_est)
                        {
                            min_val_lst = local_min_val_lst;
                            min_val_est = local_min_val_est;
                            var_index = suffix;
                            val_index = local_min_val_est_index;

                        }
                        /*
                        else if (min_val_est  == local_min_val_est)
                        {
                          if (min_val_lst > local_min_val_lst)
                          {
                            min_val_lst = local_min_val_lst;
                            //min_val_est = local_min_val_est;
                            var_index = suffix;
                            val_index = local_min_val_est_index;
                          }
                        }
                         */
                    }
                }
            }
        }


        if (var_index == TRANSPLAN::CSP_NO_NEXT_VAR)
        {
            return false;
        } else
        {
            //std::cout << "Selected var index = " << var_index << std::endl;
            assign.lhs = var_index;
            assign.rhs = val_index;
            assign.type = trans;
        }

        return true;

    }

    bool Branch::get_next_assignment_H2(Triplet& assign)
    {
        //std::cout << "Starting branching .....\n";
        bool trans;
        int var_index = TRANSPLAN::CSP_NO_NEXT_VAR;
        int val_index = -1;

        int suffix;
        int prefix;
        IntSet potential_act_set;

        for (int i = 0; i < Transplan::n_resources; ++i)
        {
            //std::cout << "Resource "<< Transplan::resources[i].name << std::endl; 
            trans = false;
            int min_eet_next = INT_MAX;
            int min_active_est = INT_MAX;
            int min_active_index = -1;

            suffix = current_path_suffix_res(i).val();
            //std::cout <<"Suffix = " << suffix << std::endl;
            prefix = current_path_prefix_res(i).val();
            //std::cout <<"Prefix = " << prefix << std::endl;
            //IntSet next_front = next_frontier(suffix, trans).dom();
            //Utils::print_set(next_front);

            //std::cout << suffix << std::endl;
            if (!next(suffix, trans).assigned())
            {
                //std::cout <<"Suffix = " << suffix << std::endl;
                //std::cout <<"Prefix = " << prefix << std::endl;
                //IntSet next_front = next_frontier(suffix, trans).dom();
                IntSet next_dom = next(suffix, trans).dom();
                //Utils::print_set(next_dom);
                IntSet active_set;
                for (IntSet::iterator itr = next_dom.begin(); itr != next_dom.end(); ++itr)
                {
                    if (*itr != prefix && prev(*itr, trans).active())
                    {
                        //std::cout << "Inserting " << *itr << std::endl;
                        active_set.insert(*itr);
                        int est = start_time(*itr, trans).min();

                        if (min_active_est > est)
                        {
                            min_active_est = est;
                            min_active_index = *itr;
                        }
                    }
                }

                if (!active_set.empty())
                {
                    bool none_exists = true;

                    for (IntSet::iterator itr = next_dom.begin(); itr != next_dom.end(); ++itr)
                    {
                        int eet = end_time(*itr, trans).min();

                        if (active_set.count(*itr) == 0)
                        {
                            for (IntSet::iterator itr_in = active_set.begin(); itr_in != active_set.end(); ++itr_in)
                            {
                                int est = start_time(*itr_in, trans).min();
                                //std::cout << "Setup time " << *itr << " -> " << *itr_in << std::endl;
                                int finish = eet + get_setup_time(*itr, *itr_in, trans);
                                if (finish <= est)
                                {
                                    //store *itr,
                                    int act_index = RESREQ_ACT_INDEX(*itr);
                                    potential_act_set.insert(act_index);
                                    //std::cout << "RR " << *itr << " exists in between...\n";
                                    none_exists = false;
                                }
                            }
                        }
                    }

                    if (none_exists)
                    {
                        //return variable assignment
                        //suffix = min_active_index
                        var_index = suffix;
                        val_index = min_active_index;

                        assign.lhs = var_index;
                        assign.rhs = val_index;
                        assign.type = trans;

                        return true;
                    }
                }
            }
        }

        //std::exit(0);
        //***************************************************************************************************************
        //**************************** For variables ********************************************************************

        //int max_dom_size = -INT_MAX;
        int min_dom_size = INT_MAX;
        int min_eet = INT_MAX;
        int min_val_est = INT_MAX;
        int min_val_index = -1;

        trans = true;

        for (int i = 0; i < Transplan::n_variables; ++i)
        {
            suffix = current_path_suffix_sv(i).val();
            prefix = current_path_prefix_sv(i).val();

            if (!next(suffix, trans).assigned())
            {
                //int sv_index = TRANS_VAR_INDEX(suffix);
                //std::string name = Transplan::state_variables[sv_index].name;

                //get the min_val_lft and min_val_index
                int next_dom_size = next(suffix, trans).size();
                int eet = end_time(suffix, trans).min();

                if (min_eet > eet)
                {
                    min_eet = eet;
                    var_index = suffix;
                    //max_dom_size = next_dom_size;
                    min_dom_size = next_dom_size;
                } else if (min_eet == eet)
                {
                    //if (max_dom_size < next_dom_size)
                    if (min_dom_size > next_dom_size)
                    {
                        //max_dom_size = next_dom_size;
                        min_dom_size = next_dom_size;
                        var_index = suffix;
                    }
                }
            }
        }


        if (var_index == TRANSPLAN::CSP_NO_NEXT_VAR)
        {
            return false;
        } else
        {
            int min_est = INT_MAX;

            IntSet next_dom = next(var_index, trans).dom();
            for (IntSet::iterator itr = next_dom.begin(); itr != next_dom.end(); ++itr)
            {
                int est = start_time(*itr, trans).min();

                if (min_est > est)
                {
                    min_est = est;
                    val_index = *itr;
                }
            }

            //std::cout << "Selected var index = " << var_index << std::endl;
        }

        assign.lhs = var_index;
        assign.rhs = val_index;
        assign.type = trans;

        return true;

    }

    bool Branch::get_next_assignment_VRP_Simple(Triplet& assign)
    {

        ///check for binary
        /*
        bool trans;

        for (int i = 0; i < Transplan::n_variables; ++i)
        {
          trans = true;
          int suffix = current_path_suffix_sv(i).val();
      
          if (!next(suffix, trans).assigned())
          {
            //std::cout << "State Variable " << Transplan::state_variables[i].name << " has domain \n";
            //std::cout << next(suffix).dom().size() << std::endl;
            if (next(suffix, trans).dom().size() == 2)
            {
              assign.lhs = suffix;
              assign.rhs = *next(suffix, trans).dom().end();
              assign.type = trans;
            }
	
          }
        }
         */

        //std::cout << "Starting branching .....\n";
        int var_index = TRANSPLAN::CSP_NO_NEXT_VAR;
        int max_space = INT_MAX;
        int suffix;
        int prefix;
        int last;
        bool trans = false;
        int selected_res = -1;

        for (int i = 0; i < Transplan::n_resources; ++i)
        {
            //std::cout << "Atl_next of resource " << i << std::endl;
            //Utils::print_set(atl_next);
            suffix = current_path_suffix_res(i).val();
            std::cout << "Next_dom_size of suffix for truck " << i << " = " << next(suffix, trans).size() << std::endl;
            prefix = current_path_prefix_res(i).val();
            int last = Transplan::resources[i].goal_requirement;

            if (!next(suffix, trans).assigned())
            {
                //std::cout << "Suffix = " << suffix << std::endl;
                //std::cout << "Prefix = " << prefix << std::endl;

                int remaining = r_start_level(prefix).max() - r_start_level(prefix).min();
                std::cout << "Res " << i << "remaining = " << remaining << std::endl;
                if (remaining < max_space)
                {
                    max_space = remaining;
                    var_index = suffix;
                    selected_res = i;
                } else if (remaining == max_space)
                {
                    if (next(suffix, trans).size() < next(var_index, trans).size())
                    {
                        max_space = remaining;
                        var_index = suffix;
                        selected_res = i;
                    }
                }

            }
        }

        //std::cout << "Selected Truck = " << selected_res << std::endl;    
        if (var_index == TRANSPLAN::CSP_NO_NEXT_VAR)
        {
            return false;
        } else
        {
            int val_index = TRANSPLAN::CSP_NO_NEXT_VAL;
            IntSet next_dom = next(var_index, trans).dom();
            //Utils::print_set(next_dom);
            int val_end_time = INT_MAX;
            int val_load = -INT_MAX;

            int res_index = RESREQ_RES_INDEX(var_index);
            int prefix = current_path_prefix_res(res_index).val();
            for (IntSet::iterator itr = next_dom.begin(); itr != next_dom.end(); ++itr)
            {
                if (*itr != prefix)
                {
                    int end_min = end_time(*itr, trans).min();
                    int end_max = end_time(*itr, trans).max();
                    int slack = end_max - end_min;
                    int load = r_requirement(*itr).min();

                    int start_min = start_time(*itr, trans).min();
                    int start_max = start_time(*itr, trans).max();
                    if (start_min < val_end_time)
                    {
                        val_end_time = start_min; //end_min;//end_max + slack;
                        val_index = *itr;
                        val_load = load;
                    } else if (start_min == val_end_time)
                    {
                        if (val_load < load)
                        {
                            val_end_time = start_min; //end_min;//end_max + slack;
                            val_index = *itr;
                            val_load = load;
                        }
                        //std::exit(0);
                    }
                }
            }


            assign.lhs = var_index;
            assign.rhs = val_index;
            assign.type = trans;
        }

        return true;

    }

    int Branch::get_next_assign(IntPair& var_type)
    {
        /*
        for(int i = 0; i < Transplan::n_variables; ++i)
        {
          int trans = true;

          int suffix  = current_path_suffix_sv(i).val();
          //std::cout << suffix << std::endl;
          if (!next(suffix, trans).assigned())
          {
            int t_index = *(next(suffix, trans).dom().begin());
            int act_index = TRANS_ACT_INDEX(t_index);
            Action* a = &Transplan::actions[act_index];
            int rel_rr = a->res_requirements.size()-1;

            std::cout << "Suffix index for "<< Transplan::state_variables[i].name << " is " 
                      << Transplan::actions[TRANS_ACT_INDEX(suffix)].name <<"("<<rel_rr<<")"<< std::endl;

          }
        }

        for(int i = 0; i < Transplan::n_resources; ++i)
        {
          int trans = false;

          int suffix  = current_path_suffix_res(i).val();
          //std::cout << suffix << std::endl;
          if (!next(suffix, trans).assigned())
          {
            //IntSet next_front = next_frontier(suffix, trans).dom();
            //Utils::print_set(next_front);
            std::cout << "Suffix index for "<< Transplan::resources[i].name << " is " 
                      << Transplan::actions[RESREQ_ACT_INDEX(suffix)].name << std::endl;
          }
        }

        std::exit(0);
         */
        return get_next_var_TT(var_type);
    }

    void Branch::get_next_var(IntPair& var_type)
    {
        /*
        //For Machine Scheduling
        int var_index = TRANSPLAN::CSP_NO_NEXT_VAR;
        int var_end_time = INT_MAX;
        int suffix;
        bool trans = false;

        for(int i = 0; i < Transplan::n_resources; ++i)
        {
          if ( atl_next_res(i).dom().size() > 0)
          {
            IntSet atl_next = atl_next_res(i).dom();
            //std::cout << "Atl_next of resource " << i << std::endl;
            //Utils::print_set(atl_next);
            suffix  = current_path_suffix_res(i).val();
            if (suffix != current_path_prefix_res(i).val())
            {
              if ( end_time(suffix, trans).min() < var_end_time)
              {
                var_end_time = end_time(suffix, trans).min();
                var_index = suffix;
              }
            }
          }
        }
        var_type.first  = var_index;
        var_type.second = trans;
        //return var_index;
         */

        /*
        int var_index = TRANSPLAN::CSP_NO_NEXT_VAR;
        int res_index = -1;
        int res_remain_cost = INT_MAX;
        int suffix;
        int binary_count = 0;
        int ternary_count = 0;

    
        for (int i = 0; i < Transplan::n_variables; ++i)
        {
          int suffix = current_path_suffix_sv(i).val();
          if (!next(suffix).assigned())
          {
            //std::cout << "State Variable " << Transplan::state_variables[i].name << " has domain \n";
            //std::cout << next(suffix).dom().size() << std::endl;
            if (next(suffix).dom().size() == 2)
              binary_count++;
            else if (next(suffix).dom().size() == 3)
              ternary_count++;
          }
        }

        //std::cout << "Binary Package = " << binary_count << std::endl;
        //std::cout << "Ternary Package = " << ternary_count << std::endl;

        for(int i = 0; i < Transplan::n_resources; ++i)
        {
          int teardown = Transplan::resources[i].teardown_requirement;

          if (!prev(teardown).assigned())
          {
            int rem = t_end_cost(teardown).max() - t_end_cost(teardown).min();
            if ( rem < res_remain_cost)
            {
              res_remain_cost = rem;
              res_index = i;
            }
          }
        }
    
        if (res_index != -1)
        {
          var_index = current_path_suffix_res(res_index).val();
        }
    
        return var_index;
         */
        get_next_var_VRP_h2(var_type);
        //get_next_var_MS(var_type);

    }

    int Branch::get_next_val(IntPair& var_type)
    {
        /*
        //For Machine Scheduling Problem
    
        int val_index = TRANSPLAN::CSP_NO_NEXT_VAL;
        //	current_delta().level_remaining_values = t_next(current_csp_var).dom();

        IntSet next_dom =  next(var_type.first, var_type.second).dom();
        //Utils::print_set(next_dom);
        int val_start_time = INT_MAX;

        int teardown_index = Transplan::resources[RESREQ_RES_INDEX(var_type.first)].goal_requirement;
        for (IntSet::iterator itr = next_dom.begin(); itr != next_dom.end(); ++itr)
        {
          if (*itr != teardown_index)
          {
            if (start_time(*itr, var_type.second).min() < val_start_time)
            {
              val_start_time = start_time(*itr, var_type.second).min();
              val_index = *itr;
            }
          }
        }

        return val_index;
         */
        /*
        int val_index = TRANSPLAN::CSP_NO_NEXT_VAL;
        IntSet next_dom =  next(var_index).dom();
        //Utils::print_set(next_dom);
        int val_end_time = INT_MAX;

        int teardown_index = Transplan::resources[TRANS_VAR_INDEX(var_index)].teardown_requirement;
        for (IntSet::iterator itr = next_dom.begin(); itr != next_dom.end(); ++itr)
        {
          if (*itr != teardown_index)
          {
            if (t_end_time(*itr).min() < val_end_time)
            {
              val_end_time = t_end_time(*itr).min();
              val_index = *itr;
            }
          }
        }

        return val_index;
         */
        return get_next_val_VRP_h2(var_type);
        //return get_next_val_MS(var_type);
    }

    void Branch::get_next_var_VRP_h2(IntPair& var_type)
    {
        int var_index = TRANSPLAN::CSP_NO_NEXT_VAR;
        int max_space = -INT_MAX;
        int suffix;
        int last;
        bool trans = false;

        for (int i = 0; i < Transplan::n_resources; ++i)
        {
            //std::cout << "Atl_next of resource " << i << std::endl;
            //Utils::print_set(atl_next);
            suffix = current_path_suffix_res(i).val();
            last = Transplan::resources[i].goal_requirement;

            if (!next(suffix, trans).assigned())
            {
                int remaining = r_end_level(last).max() - r_end_level(last).min();
                if (remaining > max_space)
                {
                    max_space = remaining;
                    var_index = suffix;
                } else if (remaining == max_space)
                {
                    if (next(suffix, trans).size() < next(var_index, trans).size())
                    {
                        max_space = remaining;
                        var_index = suffix;
                    }
                }
            }
        }

        var_type.first = var_index;
        var_type.second = trans;

    }

    void Branch::get_next_var_MS(IntPair& var_type)
    {
        int var_index = TRANSPLAN::CSP_NO_NEXT_VAR;
        int min_eet = INT_MAX;
        int suffix;
        int last;
        bool trans = true;

        for (int i = 0; i < Transplan::n_variables; ++i)
        {
            //std::cout << "Atl_next of resource " << i << std::endl;
            //Utils::print_set(atl_next);
            suffix = current_path_suffix_sv(i).val();
            last = Transplan::state_variables[i].goal_trans_index;

            if (!next(suffix, trans).assigned())
            {
                int eet = end_time(suffix, trans).min();
                if (min_eet > eet)
                {
                    min_eet = eet;
                    var_index = suffix;
                }
            }
        }

        var_type.first = var_index;
        var_type.second = trans;

    }

    int Branch::get_next_var_TT(IntPair& var_type)
    {
        int var_index = TRANSPLAN::CSP_NO_NEXT_VAR;
        int val_index = -1;
        int max_rr = -INT_MAX;
        int min_choices = INT_MAX;
        int suffix;
        int prefix;
        bool trans;
        bool found = false;
        for (int i = 0; i < Transplan::n_resources; ++i)
        {
            trans = false;
            suffix = current_path_suffix_res(i).val();
            prefix = current_path_prefix_res(i).val();

            //std::cout << "Prefix " << prefix << std::endl;
            //std::cout << "Res " << Transplan::resources[i].name << std::endl;

            if (!next(suffix, trans).assigned())
            {
                int min_eet = INT_MAX;
                IntSet next_dom = next(suffix, trans).dom();

                for (IntSet::iterator itr = next_dom.begin(); itr != next_dom.end(); ++itr)
                {
                    if (*itr != prefix)
                    {
                        int et = end_time(*itr, trans).min();
                        if (et < min_eet)
                        {
                            min_eet = et;
                        }
                    }
                }

                IntSet next_front = next_frontier(suffix, trans).dom();
                //Utils::print_set(next_front);
                for (IntSet::iterator itr = next_front.begin(); itr != next_front.end(); ++itr)
                {
                    if (*itr != prefix)
                    {
                        if (next(suffix, trans).in(*itr))
                        {
                            int st = start_time(*itr, trans).min();

                            if (st < min_eet)
                            {
                                var_index = suffix;
                                val_index = *itr;
                                //std::cout << "For " << Transplan::resources[i].name << std::endl;
                                found = true;
                                break;
                            }
                        }
                    }
                }
            }

            if (found)
                break;
        }

        if (var_index == TRANSPLAN::CSP_NO_NEXT_VAR)
        {
            for (int i = 0; i < Transplan::n_variables; ++i)
            {
                trans = true;
                suffix = current_path_suffix_sv(i).val();

                if (!next(suffix, trans).assigned())
                {
                    int t_index = *(next(suffix, trans).dom().begin());
                    int n_choices = next(suffix, trans).size();

                    int act_index = TRANS_ACT_INDEX(t_index);
                    Action* a = &Transplan::actions[act_index];

                    int rel_rr = a->res_requirements.size() - 1;

                    if (max_rr < rel_rr)
                    {
                        max_rr = rel_rr;
                        var_index = suffix;
                        min_choices = n_choices;
                    } else if (max_rr == rel_rr)
                    {
                        if (min_choices > n_choices)
                        {
                            min_choices = n_choices;
                            var_index = suffix;
                        }
                    }
                }
            }
        }

        var_type.first = var_index;
        var_type.second = trans;

        if (val_index != -1)
            return val_index;
        else if (var_index == TRANSPLAN::CSP_NO_NEXT_VAR)
            return val_index;
        else
        {
            //int sv_index = TRANS_VAR_INDEX(var_index);
            //std::cout << "For " << Transplan::state_variables[sv_index].name << std::endl;
            return get_next_val_TT(var_type);
        }

    }

    int Branch::get_next_val_VRP_h2(IntPair& var_type)
    {
        bool trans = var_type.second;
        int var_index = var_type.first;

        int val_index = TRANSPLAN::CSP_NO_NEXT_VAL;
        IntSet next_dom = next(var_index, trans).dom();
        //Utils::print_set(next_dom);
        int val_end_time = INT_MAX;

        int teardown_index = Transplan::resources[RESREQ_RES_INDEX(var_index)].goal_requirement;
        for (IntSet::iterator itr = next_dom.begin(); itr != next_dom.end(); ++itr)
        {
            if (*itr != teardown_index)
            {
                int end_min = end_time(*itr, trans).min();
                int end_max = end_time(*itr, trans).max();
                int slack = end_max - end_min;

                if (end_max + slack < val_end_time)
                {
                    val_end_time = end_max + slack;
                    val_index = *itr;
                }
            }
        }

        return val_index;
    }

    int Branch::get_next_val_MS(IntPair& var_type)
    {
        bool trans = var_type.second;
        int var_index = var_type.first;

        int val_index = TRANSPLAN::CSP_NO_NEXT_VAL;
        IntSet next_dom = next(var_index, trans).dom();
        //Utils::print_set(next_dom);
        int min_est = INT_MAX;

        int teardown_index = Transplan::state_variables[TRANS_VAR_INDEX(var_index)].goal_trans_index;
        for (IntSet::iterator itr = next_dom.begin(); itr != next_dom.end(); ++itr)
        {
            if (*itr != teardown_index)
            {
                int est = start_time(*itr, trans).min();

                if (min_est > est)
                {
                    min_est = est;
                    val_index = *itr;
                }
            }
        }

        return val_index;
    }

    int Branch::get_next_val_TT(IntPair& var_type)
    {
        //std::cout << "here....\n";
        bool trans = var_type.second;
        int var_index = var_type.first;

        int val_index = TRANSPLAN::CSP_NO_NEXT_VAL;
        IntSet next_dom = next(var_index, trans).dom();
        //Utils::print_set(next_dom);
        int val_end_time = INT_MAX;

        if (!trans)
        {
            /*
            int teardown_index = Transplan::resources[RESREQ_RES_INDEX(var_index)].goal_requirement;
            for (IntSet::iterator itr = next_dom.begin(); itr != next_dom.end(); ++itr)
            {
              if (*itr != teardown_index)
              {
                if (next_frontier(var_index, trans).in(*itr))
                {
	    
                }
              }
              }*/
        } else
        {
            double max_act_flex_score = -DBL_MAX;
            val_index = -1;

            for (IntSet::iterator itr = next_dom.begin(); itr != next_dom.end(); ++itr)
            {
                int act_index = TRANS_ACT_INDEX(*itr);
                double score = get_act_flexibility(act_index);

                if (score > max_act_flex_score)
                {
                    max_act_flex_score = score;
                    val_index = (*itr);
                }
            }
        }

        return val_index;
    }

    double Branch::get_flex_score(int index, bool trans)
    {
        IntSet all;
        if (trans)
            Transplan::get_all_transitions(index, all);
        else
            Transplan::get_all_res_req(index, all);

        int non_assign_count = 0;

        for (IntSet::iterator itr = all.begin(); itr != all.end(); ++itr)
        {
            if (!next_assigned(*itr, trans) || !prev_assigned(*itr, trans))
            {
                non_assign_count++;
            }
        }

        return non_assign_count / (all.size() - 2);

    }


    //static 

    double Branch::get_act_flexibility(int act_index)
    {
        double min_flex_score = DBL_MAX;

        IntSet trans = Transplan::actions[act_index].transitions;

        for (IntSet::iterator itr = trans.begin(); itr != trans.end(); ++itr)
        {
            double flex_score = get_flex_score(TRANS_VAR_INDEX(*itr), true);
            if (min_flex_score > flex_score)
            {
                min_flex_score = flex_score;
            }
        }

        trans = Transplan::actions[act_index].res_requirements;

        for (IntSet::iterator itr = trans.begin(); itr != trans.end(); ++itr)
        {
            double flex_score = get_flex_score(RESREQ_RES_INDEX(*itr), false);
            if (min_flex_score > flex_score)
            {
                min_flex_score = flex_score;
            }
        }

        return min_flex_score;
    }

    /******** Access for Transitions *****************/

    CSPIntVar& Branch::next(int index, bool trans)
    {
        return search->next(index, trans);
    }

    CSPIntVar& Branch::prev(int index, bool trans)
    {
        return search->prev(index, trans);
    }

    bool Branch::next_assigned(int index, bool trans)
    {
        return search->next_assigned(index, trans);
    }

    bool Branch::prev_assigned(int index, bool trans)
    {
        return search->prev_assigned(index, trans);
    }

    CSPIntervalVar& Branch::start_time(int index, bool trans)
    {
        return search->start_time(index, trans);
    }

    CSPIntervalVar& Branch::end_time(int index, bool trans)
    {
        return search->end_time(index, trans);
    }

    CSPIntervalVar& Branch::duration(int index, bool trans)
    {
        return search->duration(index, trans);
    }

    CSPIntervalVar& Branch::start_cost(int index, bool trans)
    {
        return search->start_cost(index, trans);
    }

    CSPIntervalVar& Branch::end_cost(int index, bool trans)
    {
        return search->end_cost(index, trans);
    }

    CSPIntervalVar& Branch::cost(int index, bool trans)
    {
        return search->cost(index, trans);
    }

    CSPAuxSetVar& Branch::must_be_next(int index, bool trans)
    {
        return search->must_be_next(index, trans);
    }

    CSPAuxSetVar& Branch::must_be_previous(int index, bool trans)
    {
        return search->must_be_previous(index, trans);
    }

    CSPAuxSetVar& Branch::next_frontier(int index, bool trans)
    {
        return search->next_frontier(index, trans);
    }

    CSPAuxSetVar& Branch::previous_frontier(int index, bool trans)
    {
        return search->previous_frontier(index, trans);
    }

    CSPAuxSetVar& Branch::est_parents(int index, bool trans)
    {
        return search->est_parents(index, trans);
    }

    CSPAuxSetVar& Branch::let_parents(int index, bool trans)
    {
        return search->let_parents(index, trans);
    }

    CSPAuxSetVar& Branch::est_children(int index, bool trans)
    {
        return search->est_children(index, trans);
    }

    CSPAuxSetVar& Branch::let_children(int index, bool trans)
    {
        return search->let_children(index, trans);
    }

    CSPAuxSetVar& Branch::mnsc_parents(int index, bool trans)
    {
        return search->mnsc_parents(index, trans);
    }

    CSPAuxSetVar& Branch::mxec_parents(int index, bool trans)
    {
        return search->mxec_parents(index, trans);
    }

    CSPAuxSetVar& Branch::mnsc_children(int index, bool trans)
    {
        return search->mnsc_children(index, trans);
    }

    CSPAuxSetVar& Branch::mxec_children(int index, bool trans)
    {
        return search->mxec_children(index, trans);
    }

    int Branch::get_setup_time(int from_index, int to_index, bool trans)
    {
        return search->get_setup_time(from_index, to_index, trans);
    }

    /******** Access for Resource requirements *****************/

    /*
    CSPIntervalVar& Branch::r_setup_time(int r_index)
    {
      return search->r_setup_time(r_index);
    }

    CSPAuxIntVar& Branch::r_setup_time_min_parent(int r_index)
    {
      return search->r_setup_time_min_parents(r_index);
    }

    CSPAuxIntVar& Branch::r_setup_time_max_parent(int r_index)
    {
      return search->r_setup_time_max_parent(r_index);
    }

    CSPIntervalVar& Branch::r_setup_cost(int r_index)
    {
      return search->r_setup_cost(r_index);
    }

    CSPAuxIntVar& Branch::r_setup_cost_min_parent(int index)
    {
      return search->r_setup_cost_min_parent(index);
    }

    CSPAuxIntVar& Branch::r_setup_cost_max_parent(int index)
    {
      return search->r_setup_cost_max_parent(index);
    }
     */

    CSPIntervalVar& Branch::r_start_level(int r_index)
    {
        return search->r_start_level(r_index);
    }

    CSPIntervalVar& Branch::r_end_level(int r_index)
    {
        return search->r_start_level(r_index);
    }

    CSPIntervalVar& Branch::r_requirement(int r_index)
    {
        return search->r_requirement(r_index);
    }

    CSPAuxSetVar& Branch::r_mnsl_parents(int r_index)
    {
        return search->r_mnsl_parents(r_index);
    }

    CSPAuxSetVar& Branch::r_mxel_parents(int r_index)
    {
        return search->r_mxel_parents(r_index);
    }

    CSPAuxSetVar& Branch::r_mnsl_children(int r_index)
    {
        return search->r_mnsl_children(r_index);
    }

    CSPAuxSetVar& Branch::r_mxel_children(int r_index)
    {
        return search->r_mxel_children(r_index);
    }

    /**************** Access of action variables *************************************/

    CSPBoolVar& Branch::a_inplan(int act_index)
    {
        return search->a_inplan(act_index);
    }

    CSPIntervalVar& Branch::a_start_time(int act_index)
    {
        return search->a_start_time(act_index);
    }

    CSPIntervalVar& Branch::a_start_cost(int act_index)
    {
        return search->a_start_cost(act_index);
    }

    SearchDelta& Branch::current_delta()
    {
        return search->current_delta();
    }

    CSPAuxSetVar& Branch::act_disjuncts(int act_index)
    {
        return search->act_disjuncts(act_index);
    }

    CSPAuxIntVar& Branch::current_path_suffix_sv(int sv_index)
    {
        return search->current_path_suffix_sv(sv_index);
    }

    CSPAuxIntVar& Branch::current_path_prefix_sv(int sv_index)
    {
        return search->current_path_prefix_sv(sv_index);
    }

    CSPAuxIntVar& Branch::current_path_suffix_res(int res_index)
    {
        return search->current_path_suffix_res(res_index);
    }

    CSPAuxIntVar& Branch::current_path_prefix_res(int res_index)
    {
        return search->current_path_prefix_res(res_index);
    }

    CSPMaxCounterVar& Branch::max_counter(int counter_index)
    {
        return search->max_counter(counter_index);
    }

    IntSet Branch::t_max_counters(int trans_index)
    {
        return search->t_max_counters(trans_index);
    }

    CSPAuxSetVar& Branch::atl_next_sv(int sv_index)
    {
        return search->atl_next_sv(sv_index);
    }

    CSPAuxSetVar& Branch::atl_prev_sv(int sv_index)
    {
        return search->atl_prev_sv(sv_index);
    }

    CSPAuxSetVar& Branch::atl_next_res(int res_index)
    {
        return search->atl_next_res(res_index);
    }

    CSPAuxSetVar& Branch::atl_prev_res(int res_index)
    {
        return search->atl_prev_res(res_index);
    }


}
