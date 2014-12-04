#ifndef VARIABLEPROPAGATORMANAGER_H
#define VARIABLEPROPAGATORMANAGER_H

#include "constants.h"
#include "utils.h"
#include <vector>
#include <set>

namespace TRANSPLAN
{
	class Search;
	class SearchState;
	class Propagator;
	class Inferator;

	class VariablePropagatorManager
	{
		private:
			Search& search;

			//========================= FOR SV TRANS ===============================================
			IntVector svt_next; //CSPInteger
			IntVector svt_prev; //CSPInteger
			IntVector svt_start_time; //CSPInterval
			IntVector svt_end_time; //CSPInterval
			IntVector svt_duration; //CSPInterval
			IntVector svt_inplan_var; //CSPBoolVar
			IntVector svt_must_before_set; ///CSPAuxMonoIncSet
			IntVector svt_must_after_set; ////CSPAuxMonoIncSet
			IntVector svt_mustnot_before_set; //CSPAuxMonoIncSet
			IntVector svt_mustnot_after_set; //CSPAuxMonoIncSet
			IntVector svt_LB_set; //CSPAuxSet
			IntVector svt_UB_set; //CSPAuxSet
			IntVector svt_LBChildren_set; //CSPAuxSet
			IntVector svt_UBChildren_set; //CSPAuxSet

			//========================== FOR RES TRANS ==============================================

			IntVector rt_remaining_demand; //CSPInterval
			IntVector rt_remaining_support; //CSPInterval
			IntVector rt_direct_inflow; //CSPInterval

			IntVector rt_poss_pred; //CSPAuxMonoDecSet
			IntVector rt_poss_succ; //CSPAuxMonoDecSet
			IntVector rt_customers; //CSPAuxMonoIncSet
			IntVector rt_suppliers; //CSPAuxMonoIncSet
			IntMatrix rt_support_matrix; //CSPInterval
			IntVector rt_start_time; //CSPInterval
			IntVector rt_end_time; //CSPInterval
			IntVector rt_duration; //CSPInterval
			IntVector rt_inplan_var; //CSPBoolVar

			IntVector rt_must_before_set; ///CSPAuxMonoIncSet
			IntVector rt_must_after_set; ////CSPAuxMonoIncSet
			IntVector rt_mustnot_before_set; //CSPAuxMonoIncSet
			IntVector rt_mustnot_after_set; //CSPAuxMonoIncSet

			IntVector rt_LB_set; //CSPAuxSet
			IntVector rt_UB_set; //CSPAuxSet
			IntVector rt_LBChildren_set; //CSPAuxSet
			IntVector rt_UBChildren_set; //CSPAuxSet

			//========================== FOR ACTIONS ===================================================
			IntVector a_start_time; //CSPInterval
			IntVector a_inplan_var; //CSPBoolVar
			IntVector a_disjunctions; //CSPAuxSet

			//========================== FOR RESOURCES ==================================================
			IntVector r_only_can_supp; //CSPAuxSet
			IntVector r_can_supp; //CSPAuxSet
			IntVector r_need_supp; //CSPAuxSet
			IntVector r_only_need_supp; //CSPAuxSet

			//========================== FOR STATE VARIABLES =============================================
			IntVector sv_prefix; //CSPAuxInt
			IntVector sv_only_can_supp; //CSPAuxSet
			IntVector sv_can_supp; //CSPAuxSet
			IntVector sv_need_supp; //CSPAuxSet
			IntVector sv_only_need_supp; //CSPAuxSet

			//====================================================================================================
			int actionDistanceGraphIndex;

			//====================================================================================================
			IntIntMap svt_LB_inferators;
			IntIntMap svt_UB_inferators;
			IntIntMap rt_LB_inferators;
			IntIntMap rt_UB_inferators;
			IntIntMap prevail_inferators;

			void createSVTransVariables();
			void createRTransVariables();
			void createRTransSupportVariables();
			void createActionVariables();
			void createActionDistanceVariables();
			void createResAndSVVariables();
			void createMaxOccurVariables();

			void addSVTPropagators(int trans_idx);
			void addRTPropagators(int trans_idx);
			void addRTSupportPropagators();
			void addActionPropagators(int act_idx);
			void addActionDistancePropagator();

		public:
			//constructor
			VariablePropagatorManager(Search& search);

			std::vector<Propagator*> propagators;
			std::vector<Inferator*> inferators;
			int propagatorCount;
			int inferatorCount;
			//delete CSP
			void clearCSP();
			///Creating CSP
			void createCSP();

			int createNewIntegerVar(IntSet dom);
			int createNewIntervalVar(IntPair range);
			int createNewBoolVar();
			int createNewAuxMonoDecSet(IntSet set);
			int createNewAuxMonoIncSet(IntSet set);
			int createNewAuxSet(IntSet set);
			int createNewAuxIntVar(int init_val);

			//////////////////////////////////////////////////////////////////////////////
			//////  Access CSP variables for given search-state////////////////////////////////////////////////
			////////////////////////////////////////////////////////////////////////////


			//====================== State Variable Transitions ===================================
			CSPIntVar& prev(SearchState* state, int svt_index);
			CSPIntVar& next(SearchState* state, int svt_index);
			CSPIntervalVar& svt_start(SearchState* state, int svt_index);
			CSPIntervalVar& svt_end(SearchState* state, int svt_index);
			CSPBoolVar& svt_inplan(SearchState* state, int svt_index);
			CSPAuxSetMonoIncVar& svt_must_before(SearchState* state, int svt_index);
			CSPAuxSetMonoIncVar& svt_must_after(SearchState* state, int svt_index);
			CSPAuxSetMonoIncVar& svt_mustnot_before(SearchState* state, int svt_index);
			CSPAuxSetMonoIncVar& svt_mustnot_after(SearchState* state, int svt_index);
			CSPAuxSetVar& svt_LB(SearchState* state, int svt_index);
			CSPAuxSetVar& svt_UB(SearchState* state, int svt_index);
			CSPAuxSetVar& svt_LBChildren(SearchState* state, int svt_index);
			CSPAuxSetVar& svt_UBChildren(SearchState* state, int svt_index);

			//======================== Resource Transitions =========================================

			CSPIntervalVar& rem_demand(SearchState* state, int rt_index);
			CSPIntervalVar& rem_supp(SearchState* state, int rt_index);
			CSPIntervalVar& direct_inflow(SearchState* state, int rt_index);
			CSPAuxSetMonoDecVar& poss_pred(SearchState* state, int rt_index);
			CSPAuxSetMonoDecVar& poss_succ(SearchState* state, int rt_index);
			CSPAuxSetMonoIncVar& customers(SearchState* state, int rt_index);
			CSPAuxSetMonoIncVar& suppliers(SearchState* state, int rt_index);
			CSPIntervalVar& rt_start(SearchState* state, int rt_index);
			CSPIntervalVar& rt_end(SearchState* state, int rt_index);
			bool has_supp(int rt_from, int rt_to);
			CSPIntervalVar& rt_supp(SearchState* state, int rt_from, int rt_to);
			CSPBoolVar& rt_inplan(SearchState* state, int rt_index);
			CSPAuxSetMonoIncVar& rt_must_before(SearchState* state, int rt_index);
			CSPAuxSetMonoIncVar& rt_must_after(SearchState* state, int rt_index);
			CSPAuxSetMonoIncVar& rt_mustnot_before(SearchState* state, int rt_index);
			CSPAuxSetMonoIncVar& rt_mustnot_after(SearchState* state, int rt_index);
			CSPAuxSetVar& rt_LB(SearchState* state, int rt_index);
			CSPAuxSetVar& rt_UB(SearchState* state, int rt_index);
			CSPAuxSetVar& rt_LBChildren(SearchState* state, int rt_index);
			CSPAuxSetVar& rt_UBChildren(SearchState* state, int rt_index);

			/*************************************************************
			 ******** Actions ********************************************
			 ************************************************************/

			CSPIntervalVar& a_start(SearchState* state, int a_index);
			CSPBoolVar& a_inplan(SearchState* state, int a_index);
			CSPGraphVar& a_dist(SearchState* state);
			CSPAuxSetVar& a_disjuncts(SearchState* state, int a_index);

			/*************************************************************
			 ********** Resource **********************
			 ************************************************************/

			CSPAuxSetVar& res_only_can_supp(SearchState* state, int r_index);
			CSPAuxSetVar& res_can_supp(SearchState* state, int r_index);
			CSPAuxSetVar& res_need_supp(SearchState* state, int r_index);
			CSPAuxSetVar& res_only_need_supp(SearchState* state, int r_index);

			//////////////////////////////////////////  State variables /////////////////////////////////////////

			CSPAuxSetVar& svar_only_can_supp(SearchState* state, int sv_index);
			CSPAuxSetVar& svar_can_supp(SearchState* state, int sv_index);
			CSPAuxSetVar& svar_need_supp(SearchState* state, int sv_index);
			CSPAuxSetVar& svar_only_need_supp(SearchState* state, int sv_index);
			CSPAuxIntVar& prefix(SearchState* state, int sv_index);

			/**************************************************************
			 *************** Search **************************************
			 *************************************************************/

			void printSupportMatrix(SearchState* state, int res_index);

			//=============== Additional Methods for accessing SVT and RT =======================/

			bool isActIncluded(SearchState* state, int act_index);

			bool isActExcluded(SearchState* state, int act_index);

			bool isSVTIncluded(SearchState* state, int svt_index);

			bool isRTIncluded(SearchState* state, int rt_index);

			int getRemDemand(SearchState* state, int rt_index);

			int getRemSupport(SearchState* state, int rt_index);

			int getInflow(SearchState* state, int rt_index);

			int getInflowGap(SearchState* state, int rt_index);

			int getRTSupport(SearchState* state, int t_from, int t_to);

			bool isRTSupporting(SearchState* state, int t_from, int t_to);

			int getLBInferatorIndex(bool isSVT, int tIndex)
			{
				if (isSVT)
				{
					return this->svt_LB_inferators[tIndex];
				}
				else
				{
					return this->rt_LB_inferators[tIndex];
				}
			}

			int getUBInferatorIndex(bool isSVT, int tIndex)
			{
				if (isSVT)
				{
					return this->svt_UB_inferators[tIndex];
				}
				else
				{
					return this->rt_UB_inferators[tIndex];
				}
			}
	};
}

#endif
