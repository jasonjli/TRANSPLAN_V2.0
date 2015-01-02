#ifndef TRANSPLAN_H_
#define TRANSPLAN_H_

#include <vector>
#include <map>
#include <set>
#include <stack>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cassert>
#include <limits>
#include "constants.h"
#include "utils.h"
#include "structures.h"
#include "activity.h"
#include "resource.h"
#include "statevariable.h"
#include "svtrans.h"
#include "resourcetrans.h"
#include "square_matrix.h"
#include "reader.h"
#include "configure.h"

//#include "transition.h"
//#include "tcn.h"
namespace TRANSPLAN
{

	//class Search;
	class Digraph;
	class Reader;
	class SVTrans;
	class RTrans;
	class Resource;
	class Activity;
	class StateVariable;
	class SQMatrix;

#define SV(sv)                    Transplan::statevars[sv]
#define RES(res)                  Transplan::resources[res]
#define ACT(act)                  Transplan::activities[act]
#define MAT(m)                    Transplan::matrices[m]

#define SV_SETUP_MAT(sv)          Transplan::statevars[sv].setup_matrix_id
#define RES_SETUP_MAT(sv)         Transplan::resources[sv].setup_matrix_id

#define SVTRANS(trans)            Transplan::sv_trans[trans]
#define SVTRANS_ACT_INDEX(trans)  Transplan::sv_trans[trans].a_index
#define SVTRANS_SV_INDEX(trans)   Transplan::sv_trans[trans].sv_index
#define SVTRANS_DUR(trans)        Transplan::sv_trans[trans].duration
#define SVTRANS_PRE(trans)        Transplan::sv_trans[trans].pre
#define SVTRANS_POST(trans)       Transplan::sv_trans[trans].post
#define SVTRANS_NEED(trans)       Transplan::sv_trans[trans].need
#define SVTRANS_TYPE(trans)       Transplan::sv_trans[trans].type

#define SVTRANS_SETUP_STATE(trans) Transplan::sv_trans[trans].setup_state

#define RTRANS(trans)             Transplan::r_trans[trans]
#define RTRANS_ACT_INDEX(trans)   Transplan::r_trans[trans].a_index
#define RTRANS_RES_INDEX(trans)   Transplan::r_trans[trans].r_index
#define RTRANS_DUR(trans)         Transplan::r_trans[trans].duration
#define RTRANS_RES_REQ(trans)     Transplan::r_trans[trans].req
#define RTRANS_TYPE(trans)        Transplan::r_trans[trans].type
#define RTRANS_SETUP_STATE(trans) Transplan::r_trans[trans].setup_state

#define RES_TYPE(res)             Transplan::resources[res].type
#define RES_CAP(res)              Transplan::resources[res].cap
#define RES_DUMMY_START(res)      Transplan::resources[res].dummy_s_index
#define RES_DUMMY_END(res)        Transplan::resources[res].dummy_e_index
#define SV_DUMMY_START(sv)        Transplan::statevars[sv].init_trans_index
#define SV_DUMMY_END(sv)          Transplan::statevars[sv].goal_trans_index

#define DUMMY_START               Transplan::dummy_start_action
#define DUMMY_END                 Transplan::dummy_end_action

	class Transplan
	{
		private:
			void add_IG_prod_cons();
			void add_DUMMY_S_E();
			void update();
			void create_mutex();
			void create_possible_pred_succ_for_transitions();
		public:

			//problem reader
			Reader* reader;

			// Number of actions
			static int n_activities;

			//actions
			static std::vector<Activity> activities;

			//number of resources
			static int n_resources;

			// resources
			static std::vector<Resource> resources;

			//resource name-index map
			static std::map<std::string, int> r_name_index_map;

			//number of resources
			static int n_statevars;

			//State Variables
			static std::vector<StateVariable> statevars;

			//state variable name-index map
			static std::map<std::string, int> sv_name_index_map;

			//Total number of state variable transitions
			static int n_sv_trans;

			// transitions
			static std::vector<SVTrans> sv_trans;

			//Total number of resource transitions
			static int n_r_trans;

			// transitions
			static std::vector<RTrans> r_trans;

			//// number of matrices
			static int n_sq_matrices;

			//// matrices
			static std::vector<SQMatrix> sqmatrices;

			//name of the problem
			static std::string problem_name;

			static int UPPER_BOUND;
			static int DEFAULT;

			static int HORIZON;

			static int run_time_start;
			static int run_time_end;

			static int dummy_start_action;
			static int dummy_end_action;

			static IntVector must_be_inplan_actions;

			static IntPairVector act_precedence_user;
			//HSPS::STN tcn;

			/*===================================================================================================*/

			// constructor
			Transplan();

			~Transplan()
			{
				delete reader;
			}

			/* Read Problem */
			//void readAFSCN(std::string data_file, std::string res_file);

			void addActPrecedence(int from_act, int to_act);

			void addActInplan(int act_idx);

			/**
			 Get Resource index, from name
			 */
			int getResIndex(std::string name)
			{
				if (r_name_index_map.find(name) == r_name_index_map.end())
				{
					return -1;
				}

				return r_name_index_map.find(name)->second;
			}

			int getSVIndex(std::string name)
			{
				if (sv_name_index_map.find(name) == sv_name_index_map.end())
				{
					return -1;
				}

				return sv_name_index_map.find(name)->second;
			}

			///// Interface for state variable creation
			bool addState(int state_var_index, std::string state_name);
			void setInitialState(int state_var_index, std::string state);
			void setGoalState(int state_var_index, std::string state);
			void setNonGoalState(int state_var_index, std::string state);
			void setMaxPersist(int state_var_index, std::string state, int val);
			void setMinPersist(int state_var_index, std::string state, int val);
			void setStabilityOffset(int state_var_index, std::string state, int val);
			void addSVSetupMatrix(int state_var_index, int matrix_id);

			int getSVStateIndex(int state_var_index, std::string state);

			///////////// resource
			void addRESSetupMatrix(int resource_index, int matrix_id);

			void updateActionTransitionOrdering();

			/**
			 Create a new activity
			 */

			int createNewActivity(std::string name);
			/**
			 Create new state variable
			 */
			int createNewStateVariable(std::string name);

			/**
			 Create new resource
			 */

			int createNewResource(std::string name, int cap, RES_TYPE type);

			void setResourceInitLevel(int res_idx, int val);

			void setResourceGoalLevel(int res_idx, int val_min, int val_max);
			/**
			 Create new state variable transiton, returns the global index
			 */
			int createNewStateVariableTransition(int sv_index, int a_index, int from, int to, int dur, int offset, int setup_state);

			/**
			 Create new resource transition and returns the global index
			 */
			int createNewResourceTransition(int r_index, int a_index, int req, TRANS_TYPE type, int dur, int offset, int setup_state, int mode);

			/**
			 Create new square matrix and returns the global index
			 */
			int createNewSquareMatrix(int n_size, int def_val);

			static int getSetupTime(int sq_matrix_idx, int from_state, int to_state);

			/**
			 Initialize
			 */
			void initialize();

			/**
			 DFS Search
			 */
			void search();

			/**
			 DFS Branch-and-Bound
			 */
			void search_bb();

			/**
			 Print stat
			 */
			void print_stat();

			static void print_dot_distance_graph(std::string& problem_name);
			static void print_dot_distance_graph_res(int res_index, std::string& problem_name);

			static int getTransActIndex(int tIndex, bool isSVT)
			{
				if (isSVT)
				{
					return Transplan::sv_trans[tIndex].a_index;
				}
				else
					return Transplan::r_trans[tIndex].a_index;
			}

			static int getTransDuration(int tIndex, bool isSVT)
			{
				if (isSVT)
				{
					return Transplan::sv_trans[tIndex].duration;
				}
				else
					return Transplan::r_trans[tIndex].duration;
			}

			static int getTransOffset(int tIndex, bool isSVT)
			{
				if (isSVT)
				{
					return Transplan::sv_trans[tIndex].offset;
				}
				else
					return Transplan::r_trans[tIndex].offset;
			}
			/* Extract solution
			 void print_plan(Search* search);
			 */

	};
}

#endif
