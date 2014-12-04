#ifndef BRANCH_AFSCNh1_H
#define BRANCH_AFSCNh1_H

#include "branchstrategy.h"
#include "utils.h"

namespace TRANSPLAN
{

    class AFSCNBranchH1 : public BranchStrategy
    {
    public:

        /*
          Idea here is to choose a truck which has
          1. minimum number next options
          2. minimum est 
         */

        AFSCNBranchH1()
        : BranchStrategy()
        {
            //
            //std::cout << "RCPSPmax branching created\n";
        }

        IntPair get_min_next_svt(int var)
        {
            int min_s = TRANSPLAN::DEFAULT_MAX_TIME;
            int min_idx = -1;

            if (true)//!SVTRANS( var ).is_dummy )
            {
                IntSet poss_succ = VariableManager::next(var).dom();

                poss_succ.erase(TRANSPLAN::NOT_IN_PLAN);
                poss_succ.erase(SV(SVTRANS_SV_INDEX(var)).goal_trans_index);

                //Utils::print_set( poss_succ );

                for (IntSet::iterator itr = poss_succ.begin(); itr != poss_succ.end(); ++itr)
                {
                    if (!VariableManager::prev(*itr).assigned() && !SVTRANS(*itr).is_dummy)
                    {
                        int st = VariableManager::svt_start(*itr).min();
                        if (min_s > st)
                        {
                            min_s = st;
                            min_idx = *itr;
                        }
                    }
                }

            }

            return std::make_pair(min_idx, min_s);

        }

        IntPair get_min_next_rt(int var)
        {
            int min_s = TRANSPLAN::DEFAULT_MAX_TIME;
            int min_idx = -1;

            if (true)//!Transplan::r_trans[var].is_dummy )
            {
                IntSet poss_succ = VariableManager::poss_succ(var).dom();

                poss_succ.erase(TRANSPLAN::NOT_IN_PLAN);
                poss_succ.erase(RES(RTRANS_RES_INDEX(var)).dummy_e_index);

                for (IntSet::iterator itr = poss_succ.begin(); itr != poss_succ.end(); ++itr)
                {
                    if (VariableManager::a_inplan(RTRANS_ACT_INDEX(*itr)).min() == 1 &&
                            VariableManager::rt_supp(var, *itr).min() == 0 &&
                            VariableManager::rt_supp(var, *itr).max() > 0 &&
                            VariableManager::rem_demand(*itr).max() > 0)
                    {
                        int st = VariableManager::rt_start(*itr).min();
                        if (min_s > st)
                        {
                            min_s = st;
                            min_idx = *itr;
                        }
                    }
                }
            }

            return std::make_pair(min_idx, min_s);

        }

        //// returns 0 when no branching possible, 1-when state variable branching and 2- when resource branching

        BRACH_TYPE getNextDecision(SearchState* state, IntPair& curr_ass)
        {
            //std::cout << "[" << level << "] looking for assignment "<< std::endl;
            get_assignment_res(curr_ass);

            if (curr_ass.first == -1 || curr_ass.second == -1)
            {
                get_assignment_sv(curr_ass);
                //std::cout << curr_ass.first << " " << curr_ass.second << std::endl;
                if (curr_ass.first != -1 && curr_ass.second != -1)
                {

                    //std::cout << "[" << level << "] Returning 1 On " <<  SV( SVTRANS_SV_INDEX( curr_ass.first ) ).name <<" ";
                    if (VariableManager::next(curr_ass.first).assigned())
                    {
                        std::cout << "Assigned already\n";
                        std::exit(0);
                    }
                    int from_act = SVTRANS_ACT_INDEX(curr_ass.first);
                    int to_act = SVTRANS_ACT_INDEX(curr_ass.second);

                    //std::cout << ACT(from_act).name << " ---> " << ACT( to_act ).name << "\n";
                    return TRANSPLAN::BR_SV;
                }
            } else
            {
                //std::cout << "[" << level << "] Returning 2 On " <<  RES( RTRANS_RES_INDEX( curr_ass.first ) ).name <<" ";
                int from_act = RTRANS_ACT_INDEX(curr_ass.first);
                int to_act = RTRANS_ACT_INDEX(curr_ass.second);

                //std::cout << ACT( from_act).name << " ---> " << ACT( to_act ).name << "\n";
                return TEANSPLAN::BR_RES;
            }
            std::cout << "[" << level << "] Returning 0 \n";
            return TRANSPLAN::BR_NONE;

        }

        void get_assignment_res(IntPair& assignment)
        {
            IntSet poss;
            for (int i = 0; i < Transplan::n_resources; ++i)
            {
                IntSet p = VariableManager::res_only_can_supp(i).dom();
                poss.insert(p.begin(), p.end());
            }
            Utils::print_set(poss);
            int min_e = TRANSPLAN::DEFAULT_MAX_TIME;
            int min_s = TRANSPLAN::DEFAULT_MAX_TIME;
            int min_var_idx = -1;
            int min_val_idx = -1;

            for (IntSet::iterator itr = poss.begin(); itr != poss.end(); ++itr)
            {
                IntPair val_t(-1, INT_MAX);
                if (VariableManager::rem_supp(*itr) > 0)
                    val_t = get_min_next_rt(*itr);

                if (val_t.second < min_s)
                {
                    min_e = VariableManager::rt_end(*itr).min();
                    min_s = val_t.second;
                    min_var_idx = *itr;
                    min_val_idx = val_t.first;
                } else if (val_t.second == min_s)
                {
                    int end = VariableManager::rt_end(*itr).min();
                    if (end < min_e)
                    {
                        min_e = end;
                        min_s = val_t.second;
                        min_var_idx = *itr;
                        min_val_idx = val_t.first;
                    }
                }
            }

            assignment = std::make_pair(min_var_idx, min_val_idx);
        }

        void get_assignment_sv(IntPair& assignment)
        {

            IntSet poss;
            for (int i = 0; i < Transplan::n_statevars; i++)
            {
                //std::cout << "State variable " << SV(i).name << " ";
                IntSet s = VariableManager::svar_only_can_supp(i).dom();
                poss.insert(s.begin(), s.end());

            }
            //Utils::print_set( poss );
            int min_e = TRANSPLAN::DEFAULT_MAX_TIME;
            int min_s = TRANSPLAN::DEFAULT_MAX_TIME;
            int min_var_idx = -1;
            int min_val_idx = -1;

            for (IntSet::iterator itr = poss.begin(); itr != poss.end(); ++itr)
            {
                IntPair val_t(-1, INT_MAX);

                if (!VariableManager::next(*itr).assigned())
                    val_t = get_min_next_svt(*itr);

                if (val_t.second < min_s)
                {
                    min_e = VariableManager::svt_end(*itr).min();
                    min_s = val_t.second;
                    min_var_idx = *itr;
                    min_val_idx = val_t.first;
                } else if (val_t.second == min_s)
                {
                    int end = VariableManager::svt_end(*itr).min();
                    if (end < min_e)
                    {
                        min_e = end;
                        min_s = val_t.second;
                        min_var_idx = *itr;
                        min_val_idx = val_t.first;
                    }
                }
            }

            assignment = std::make_pair(min_var_idx, min_val_idx);
        }

    };
}
#endif
