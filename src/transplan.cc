#include "transplan.h"
#include "defaultBranchStrategy.h"
#include "search.h"
#include <sstream>
//#include <time.h>

namespace TRANSPLAN
{
	/// Number of activities
	int Transplan::n_activities;

	//set of all activities
	std::vector<Activity> Transplan::activities;

	// Number of resources
	int Transplan::n_resources;

	// resources
	std::vector<Resource> Transplan::resources;

	//resource name-index map
	std::map<std::string, int> Transplan::r_name_index_map;

	//number of state variables
	int Transplan::n_statevars;

	//State Variables
	std::vector<StateVariable> Transplan::statevars;

	//state variable name-index map
	std::map<std::string, int> Transplan::sv_name_index_map;

	//Total number of state variable transitions
	int Transplan::n_sv_trans;

	// transitions
	std::vector<SVTrans> Transplan::sv_trans;

	//Total number of resource transitions
	int Transplan::n_r_trans;

	// transitions
	std::vector<RTrans> Transplan::r_trans;

	//// number of matrices
	int Transplan::n_sq_matrices;

	//// matrices
	std::vector<SQMatrix> Transplan::sqmatrices;

	std::string Transplan::problem_name;

	int Transplan::UPPER_BOUND;
	int Transplan::HORIZON;
	int Transplan::DEFAULT;

	int Transplan::run_time_start;
	int Transplan::run_time_end;

	int Transplan::dummy_start_action;
	int Transplan::dummy_end_action;

	IntVector Transplan::must_be_inplan_actions;

	IntPairVector Transplan::act_precedence_user;

	/**
	 Print the statistics about the size of the problem
	 */
	void Transplan::print_stat()
	{
		std::cout << "Number of Activities      = " << Transplan::n_activities << std::endl << "Number of State Variables = " << Transplan::n_statevars << std::endl << "Number of Resources       = " << Transplan::n_resources << std::endl << "Number of SV Transitions  = "
				<< Transplan::n_sv_trans << std::endl << "Number of Res Transitions = " << Transplan::n_r_trans << std::endl << "Number of Sq Matrices     = " << Transplan::n_sq_matrices << std::endl;

	}

	Transplan::Transplan()
	{
		//initialization
		Transplan::n_activities = 0;
		Transplan::n_resources = 0;
		Transplan::n_statevars = 0;
		Transplan::n_sv_trans = 0;
		Transplan::n_r_trans = 0;

		UPPER_BOUND = 0;
		HORIZON = 0;
		DEFAULT = 10000; //INT_MAX;

		//read problem and create state variable, action and transitoins
		// update actions according the constraints
		//reader = new Reader();

		UPPER_BOUND = HORIZON;
		std::cout << "Created ...." << std::endl;

	}

	/**
	 Create a new activity. returns the global index, default reslease date and deadline
	 */

	int Transplan::createNewActivity(std::string name)
	{
		Activity act(name, Transplan::n_activities++, Transplan::n_statevars, Transplan::n_resources);
		Transplan::activities.push_back(act);
		return act.index;
	}

	/**
	 Create new state variable, returns the global index of the state variable
	 */
	int Transplan::createNewStateVariable(std::string name)
	{
		StateVariable sv(name, Transplan::n_statevars++);
		Transplan::sv_name_index_map.insert(std::make_pair(sv.name, sv.index));
		Transplan::statevars.push_back(sv);

		return sv.index;
	}

	/////////////// State variable creation interface

	int Transplan::getSVStateIndex(int state_var_index, std::string state)
	{
		Transplan::statevars[state_var_index].getStateIndex(state);
	}

	bool Transplan::addState(int state_var_index, std::string state_name)
	{
		return Transplan::statevars[state_var_index].add_domain_value(state_name);
	}

	void Transplan::setInitialState(int state_var_index, std::string state)
	{
		Transplan::statevars[state_var_index].set_init_state(state);
	}

	void Transplan::setGoalState(int state_var_index, std::string state)
	{
		Transplan::statevars[state_var_index].set_goal_state(state);
	}

	void Transplan::setNonGoalState(int state_var_index, std::string state)
	{
		Transplan::statevars[state_var_index].add_non_goal_state(state);
	}

	void Transplan::setMaxPersist(int state_var_index, std::string state, int val)
	{
		Transplan::statevars[state_var_index].setMaxPersist(state, val);
	}

	void Transplan::setMinPersist(int state_var_index, std::string state, int val)
	{
		Transplan::statevars[state_var_index].setMinPersist(state, val);
	}

	void Transplan::setStabilityOffset(int state_var_index, std::string state, int val)
	{
		Transplan::statevars[state_var_index].setStabilityOffset(state, val);
	}

	void Transplan::addSVSetupMatrix(int state_var_index, int matrix_id)
	{
		Transplan::statevars[state_var_index].setup_matrix_id = matrix_id;
	}

	void Transplan::addRESSetupMatrix(int resource_index, int matrix_id)
	{
		Transplan::resources[resource_index].setup_matrix_id = matrix_id;
	}

	/**
	 Create new resource, returns the global index
	 */

	int Transplan::createNewResource(std::string name, int cap, RES_TYPE type = TRANSPLAN::REUSE_R)
	{
		Resource res(name, Transplan::n_resources++, cap, type);
		Transplan::r_name_index_map.insert(std::make_pair(res.name, res.index));
		Transplan::resources.push_back(res);

		return res.index;
	}

	void Transplan::setResourceInitLevel(int res_idx, int val)
	{
		RES(res_idx).init_level = val;
	}

	void Transplan::setResourceGoalLevel(int res_idx, int val_min, int val_max)
	{
		RES(res_idx).goal_level_min = val_min;
		RES(res_idx).goal_level_max = val_max;
	}

	void Transplan::addActPrecedence(int from_act, int to_act)
	{
		//std::cout << "Adding " << from_act << "--> "<<  to_act << std::endl;
		Transplan::act_precedence_user.push_back(std::make_pair(from_act, to_act));
	}

	void Transplan::addActInplan(int act_idx)
	{
		Transplan::must_be_inplan_actions.push_back(act_idx);
	}

	/**
	 Create new state variable transiton, returns the global index
	 */
	int Transplan::createNewStateVariableTransition(int sv_index, int a_index, int from, int to, int dur, int offset, int setup_state)
	{
		SVTrans s_t(Transplan::n_sv_trans++, sv_index, a_index, from, to, dur, offset, setup_state);
		Transplan::sv_trans.push_back(s_t);

		return s_t.index;

	}

	/**
	 Create new resource transition and returns the global index
	 */
	int Transplan::createNewResourceTransition(int r_index, int a_index, int req, TRANS_TYPE type, int dur, int offset = 0, int setup_state = 0, int mode = 0)
	{
		RTrans r_t(Transplan::n_r_trans++, r_index, a_index, req, type, dur, offset, setup_state, mode);
		Transplan::r_trans.push_back(r_t);

		return r_t.index;
	}

	/**
	 Create new square matrix and returns the global index
	 */
	int Transplan::createNewSquareMatrix(int n_size, int def_val = -1)
	{
		SQMatrix mat(Transplan::n_sq_matrices++, n_size, def_val);
		Transplan::sqmatrices.push_back(mat);

		return mat.index;
	}

	int Transplan::getSetupTime(int sq_matrix_idx, int from_state, int to_state)
	{
		if (sq_matrix_idx == -1)
		{
			return 0;
		}

		return Transplan::sqmatrices[sq_matrix_idx].getElementValue(from_state, to_state);
	}

	void Transplan::initialize()
	{
		std::cout << "Initializing....\n";
		//// add dummy goal state: IG to state variables where no goal state is defined
		//// also need to create dummy actions-transition from all potential goal state to IG.
		add_IG_prod_cons();

		/// add dummy global START and END action
		std::cout << "Adding DUMMY START and END action \n";
		add_DUMMY_S_E();

		//// update the relations and information between transitions, state variables, resources and actions
		std::cout << "Update transitions \n";
		update();

		/// create mutex action relation
		std::cout << "Creating resource mutex \n";
		create_mutex();

		/// create for each transition the possible predecessor and successor transition set.
		std::cout << "Initializing possible successor and predecessor \n";
		create_possible_pred_succ_for_transitions();

		std::cout << "Updating act res transitions precedence \n";
		updateActionTransitionOrdering();

		/// print statistics about transitions and actions and state vars and resources
		print_stat();

	}

	void Transplan::add_IG_prod_cons()
	{
		std::cout << "Adding IG to state variables \n";

		for (int sv_idx = 0; sv_idx < Transplan::n_statevars; ++sv_idx)
		{
			assert(SV(sv_idx).index == sv_idx);
			//SV( sv_idx ).print();
			if (SV(sv_idx).goal_state_index == -1)
			{
				std::string val = "IG";
				SV(sv_idx).add_domain_value(val);

				SV(sv_idx).goal_state_index = SV(sv_idx).getStateIndex(val);
				int to = SV(sv_idx).goal_state_index;

				std::vector<StateVariableState>::const_iterator itr = SV(sv_idx).states.begin();
				while (itr != SV(sv_idx).states.end())
				{
					if (itr->name != "S" && itr->name != "G" && itr->name != "IG" && !itr->is_non_goal_state)
					{
						int from = itr->index;

						//creating dummy action
						std::stringstream name;
						name << "Dummy_Goal_Act_SV-" << SV(sv_idx).name << "_" << SV(sv_idx).getStateName(from) << "_" << SV(sv_idx).getStateName(to);
						int act_idx = this->createNewActivity(name.str());

						int dummy_id = this->createNewStateVariableTransition(sv_idx, act_idx, from, to, 0, 0, 0);

						SVTRANS(dummy_id).is_dummy = true;

					}

					itr++;
				}
			}

			assert(SV(sv_idx).goal_state_index != -1);
			//SV( sv_idx ).print();
		}

		////for reserver resources
		for (int r = 0; r < n_resources; ++r)
		{
			if (RES_TYPE(r) == TRANSPLAN::RESERV_R)
			{
				int init_level = RES(r).init_level;
				int init_free = RES_CAP(r) - init_level;

				if (init_level > 0)
				{
					//creating dummy action
					std::stringstream name_p;
					name_p << "Start_PROD_Action_" << RES(r).name;

					int act_idx = this->createNewActivity(name_p.str());

					Transplan::must_be_inplan_actions.push_back(act_idx);

					int dummy_id = this->createNewResourceTransition(r, act_idx, init_level, TRANSPLAN::PROD, 0, 0, 0, 0);

					RES(r).dummy_start_prod_idx = dummy_id;
					RTRANS(dummy_id).is_dummy = true;

				}

				if (init_free > 0)
				{
					//creating dummy action
					std::stringstream name_c;
					name_c << "Start_CONS_Action_" << RES(r).name;
					int act_idx = this->createNewActivity(name_c.str());

					Transplan::must_be_inplan_actions.push_back(act_idx);

					int dummy_id = this->createNewResourceTransition(r, act_idx, init_free, TRANSPLAN::CONS, 0, 0, 0, 0);
					RES(r).dummy_start_cons_idx = dummy_id;
					RTRANS(dummy_id).is_dummy = true;

				}

				int goal_level_min = RES(r).goal_level_min;
				int goal_free_min = RES_CAP(r) - RES(r).goal_level_max;

				if (goal_level_min > 0)
				{
					//creating dummy action
					std::stringstream name_gp;
					name_gp << "End_CONS_Action_" << RES(r).name;
					int act_idx = this->createNewActivity(name_gp.str());

					Transplan::must_be_inplan_actions.push_back(act_idx);

					int dummy_id = this->createNewResourceTransition(r, act_idx, goal_level_min, TRANSPLAN::CONS, 0, 0, 0, 0);

					RES(r).dummy_end_cons_idx = dummy_id;
					RTRANS(dummy_id).is_dummy = true;

				}

				if (goal_free_min > 0)
				{
					//creating dummy action
					std::stringstream name_gc;
					name_gc << "End_PROD_Action_" << RES(r).name;
					int act_idx = this->createNewActivity(name_gc.str());

					Transplan::must_be_inplan_actions.push_back(act_idx);

					int dummy_id = this->createNewResourceTransition(r, act_idx, goal_free_min, TRANSPLAN::PROD, 0, 0, 0, 0);
					RES(r).dummy_end_prod_idx = dummy_id;
					RTRANS(dummy_id).is_dummy = true;

				}

			}
		}

	}

	void Transplan::add_DUMMY_S_E()
	{
		std::string s_name("DUMMY_S");
		DUMMY_START = this->createNewActivity(s_name);

		Transplan::must_be_inplan_actions.push_back(DUMMY_START);

		std::string e_name("DUMMY_E");
		DUMMY_END = this->createNewActivity(e_name);

		Transplan::must_be_inplan_actions.push_back(DUMMY_END);

		//for state variables
		for (int i = 0; i < Transplan::n_statevars; ++i)
		{
			int s_from = SV(i).getStateIndex(std::string("S"));
			int s_to = SV(i).init_state_index;

			int dummy_s = this->createNewStateVariableTransition(SV(i).index, DUMMY_START, s_from, s_to, 0, 0, SV(i).init_setup_state);

			SV(i).init_trans_index = dummy_s;
			SVTRANS(dummy_s).is_dummy = true;

			int e_from = SV(i).goal_state_index;
			int e_to = SV(i).getStateIndex(std::string("G"));

			int dummy_e = this->createNewStateVariableTransition(SV(i).index, DUMMY_END, e_from, e_to, 0, 0, SV(i).goal_setup_state);
			SV(i).goal_trans_index = dummy_e;
			SVTRANS(dummy_e).is_dummy = true;

		}

		//for resources
		for (int i = 0; i < Transplan::n_resources; ++i)
		{
			int req = RES(i).cap;

			int dummy_s = this->createNewResourceTransition(RES(i).index, DUMMY_START, req, TRANSPLAN::BRR, 0, 0, RES(i).init_setup_state, 0);

			RES(i).dummy_s_index = dummy_s;
			RTRANS(dummy_s).is_dummy = true;

			int dummy_e = this->createNewResourceTransition(RES(i).index, DUMMY_END, req, TRANSPLAN::BRR, 0, 0, RES(i).init_setup_state, 0);

			RES(i).dummy_e_index = dummy_e;
			RTRANS(dummy_e).is_dummy = true;

		}

	}

	/**
	 Update() updates the data structures for Resources, State Variables and Actions with regards to the transitions.
	 */
	void Transplan::update()
	{
		//std::cout << "Updating state variable transitions\n";
		///for each state variable transition
		for (int i = 0; i < Transplan::n_sv_trans; ++i)
		{
			//for state variable
			int sv_idx = SVTRANS(i).sv_index;
			int t_idx = SVTRANS(i).index;
			int a_idx = SVTRANS(i).a_index;
			int pre = SVTRANS(i).pre;
			int post = SVTRANS(i).post;

			//SV(sv_idx).print();
			SV(sv_idx).transitions.push_back(t_idx);
			SV(sv_idx).acts.push_back(a_idx);
			SV(sv_idx).act_trans_map.insert(std::make_pair(a_idx, t_idx));
			SV(sv_idx).trans_act_map.insert(std::make_pair(t_idx, a_idx));

			if (pre != post)
			{
				//std::cout <<"Adding Incoming and Outgoing transitions : "<< post <<" "<< pre <<" "<< t_idx <<"\n";
				SV(sv_idx).addIncomingTransition(post, t_idx); //states[post].incoming_trans.push_back(t_idx);
				SV(sv_idx).addOutgoingTransition(pre, t_idx); //states[pre].outgoing_trans.push_back(t_idx);
			}
			else
			{
				//std::cout <<"Adding Prevail transitions\n";
				SV(sv_idx).addPrevailTransition(pre, t_idx); //states[pre].prevail_trans.push_back(t_idx);
			}

			///for actions
			ACT(a_idx).sv_trans.push_back(t_idx);
			ACT(a_idx).sv_trans_map[sv_idx].push_back(t_idx);
		}

		//std::cout << "Updating resource  transitions\n";
		///for each resource transition
		for (int i = 0; i < Transplan::n_r_trans; ++i)
		{
			//for resource
			int r_idx = RTRANS(i).r_index;
			int idx = RTRANS(i).index;
			int a_idx = RTRANS(i).a_index;
			int req = RTRANS(i).req;
			if (req == 0)
			{
				std::cout << "Action " << a_idx << " has a req = 0\n";
			}
			TRANS_TYPE type = RTRANS(i).type;

			RES(r_idx).transitions.push_back(idx);
			RES(r_idx).acts.push_back(a_idx);
			RES(r_idx).act_trans_map.insert(std::make_pair(a_idx, idx));
			RES(r_idx).trans_act_map.insert(std::make_pair(idx, a_idx));

			if (type == TRANSPLAN::BRR)
				RES(r_idx).borrow_trans.push_back(idx);
			else if (type == TRANSPLAN::CONS)
				RES(r_idx).cons_trans.push_back(idx);
			else if (type == TRANSPLAN::PROD)
				RES(r_idx).prod_trans.push_back(idx);
			else
			{
				std::cout << "RES TRANS TYPE is wrong....\n";
				std::exit(0);
			}

			///for actions
			ACT(a_idx).r_trans.push_back(idx);
			ACT(a_idx).res_trans_map[r_idx].push_back(idx);

		}

	}

	void Transplan::create_mutex()
	{
		for (int i = 0; i < Transplan::n_resources; ++i)
		{
			int cap = RES(i).cap;
			IntVector trans = RES(i).transitions;
			for (int j = 0; j < trans.size(); ++j)
			{
				int req_j = RTRANS(trans[j]).req;
				for (int k = j + 1; k < trans.size(); ++k)
				{
					int req_k = RTRANS(trans[k]).req;

					if ((req_j + req_k) > cap)//|| RTRANS( trans[j] ).mode != RTRANS( trans[k] ).mode )
					{
						RTRANS(trans[j]).mutex_trans.insert(trans[k]);
						RTRANS(trans[k]).mutex_trans.insert(trans[j]);

						ACT(RTRANS_ACT_INDEX(trans[j])).mutex.insert(RTRANS_ACT_INDEX(trans[k]));
						ACT(RTRANS_ACT_INDEX(trans[k])).mutex.insert(RTRANS_ACT_INDEX(trans[j]));
					}
				}
			}
		}
	}

	void Transplan::create_possible_pred_succ_for_transitions()
	{
		//for state variable transitions
		for (int sv = 0; sv < Transplan::n_statevars; ++sv)
		{
			for (int st = 0; st < SV(sv).domain_size; ++st)
			{
				///for EFFECT transitions
				IntVector in = SV(sv).getIncomingTransitions(st);
				IntVector out = SV(sv).getOutgoingTransitions(st);

				for (int j = 0; j < in.size(); ++j)
				{
					for (int k = 0; k < out.size(); ++k)
					{
						SVTRANS(in[j]).poss_succ.insert(out[k]);
					}
				}

				for (int j = 0; j < out.size(); ++j)
				{
					for (int k = 0; k < in.size(); ++k)
					{
						SVTRANS(out[j]).poss_pred.insert(in[k]);
					}
				}

				//for PREVAIL transitions
				IntVector need = SV(sv).getPrevailTransitions(st);

				for (int j = 0; j < need.size(); ++j)
				{
					for (int k = 0; k < in.size(); ++k)
					{
						SVTRANS(need[j]).poss_pred.insert(in[k]);
					}

					for (int k = 0; k < out.size(); ++k)
					{
						SVTRANS(need[j]).poss_succ.insert(out[k]);
					}
				}
			}
		}

		///for resource transitions
		for (int r = 0; r < n_resources; ++r)
		{
			/*
			 std::cout << "For resource " << RES(r).name
			 << " Cap " << RES(r).cap
			 << " Borrow " << RES(r ).borrow_trans.size()
			 << " Prod " << RES(r).prod_trans.size()
			 <<" Cons " << RES(r ).prod_trans.size()
			 << std::endl;
			 */
			//for BORROW transitions
			if (RES_TYPE(r) == TRANSPLAN::REUSE_R)
			{
				IntSet trans_set;
				IntVector trans = RES(r).borrow_trans;
				Utils::int_vec_to_int_set(trans, trans_set);

				for (int j = 0; j < trans.size(); ++j)
				{
					if (trans[j] != RES_DUMMY_START(r))
					{
						RTRANS(trans[j]).poss_pred = trans_set;

						RTRANS(trans[j]).poss_pred.erase(trans[j]);
						RTRANS(trans[j]).poss_pred.erase(RES_DUMMY_END(r));

					}

					if (trans[j] != RES_DUMMY_END(r))
					{
						RTRANS(trans[j]).poss_succ = trans_set;

						RTRANS(trans[j]).poss_succ.erase(trans[j]);
						RTRANS(trans[j]).poss_succ.erase(RES_DUMMY_START(r));
					}
				}
			}
			else if (RES_TYPE(r) == TRANSPLAN::RESERV_R) //for PROD and CONS transitions
			{
				IntSet prod_set;
				IntSet cons_set;

				IntVector prod_trans = RES(r).prod_trans;
				IntVector cons_trans = RES(r).cons_trans;

				Utils::int_vec_to_int_set(prod_trans, prod_set);
				Utils::int_vec_to_int_set(cons_trans, cons_set);

				int start_idx = RES(r).dummy_s_index;
				int end_idx = RES(r).dummy_e_index;

				//RTRANS( start_idx ).poss_succ = cons_set;//.insert( start_idx );
				//RTRANS( end_idx ).poss_pred = prod_set;//.insert( prod_trans[i] );
				///For PROD transitions :
				for (int i = 0; i < prod_trans.size(); ++i)
				{
					RTRANS(start_idx).poss_succ.insert(prod_trans[i]);
					RTRANS(end_idx).poss_pred.insert(prod_trans[i]);

					if (RES(r).is_dummy_s_prod(prod_trans[i]))
					{
						RTRANS(prod_trans[i]).poss_pred.insert(start_idx);

						RTRANS(prod_trans[i]).poss_succ = cons_set;
						RTRANS(prod_trans[i]).poss_succ.insert(end_idx);

					}
					else if (RES(r).is_dummy_e_prod(prod_trans[i]))
					{
						RTRANS(prod_trans[i]).poss_pred = cons_set;
						RTRANS(prod_trans[i]).poss_pred.insert(start_idx);

						RTRANS(prod_trans[i]).poss_succ.insert(end_idx);

					}
					else
					{
						RTRANS(prod_trans[i]).poss_pred = cons_set;
						RTRANS(prod_trans[i]).poss_pred.insert(start_idx);
						RTRANS(prod_trans[i]).poss_succ = cons_set;
						RTRANS(prod_trans[i]).poss_succ.insert(end_idx);

					}

					RTRANS(prod_trans[i]).poss_pred.erase(prod_trans[i]);
					RTRANS(prod_trans[i]).poss_succ.erase(prod_trans[i]);

					//std::cout << "Possible Pred for Res Transition " << prod_trans[i] ;
					//Utils::print_set( RTRANS( prod_trans[i] ).poss_pred );

				}

				///FOR CONS Transitions
				for (int i = 0; i < cons_trans.size(); ++i)
				{
					RTRANS(start_idx).poss_succ.insert(cons_trans[i]);
					RTRANS(end_idx).poss_pred.insert(cons_trans[i]);

					if (RES(r).is_dummy_s_cons(cons_trans[i]))
					{
						RTRANS(cons_trans[i]).poss_pred.insert(start_idx);
						RTRANS(cons_trans[i]).poss_succ = prod_set;
						RTRANS(cons_trans[i]).poss_succ.insert(end_idx);

					}
					else if (RES(r).is_dummy_e_cons(cons_trans[i]))
					{
						RTRANS(cons_trans[i]).poss_pred = prod_set;
						RTRANS(cons_trans[i]).poss_pred.insert(start_idx);
						RTRANS(cons_trans[i]).poss_succ.insert(end_idx);

					}
					else
					{
						RTRANS(cons_trans[i]).poss_pred = prod_set;
						RTRANS(cons_trans[i]).poss_succ = prod_set;
						RTRANS(cons_trans[i]).poss_pred.insert(start_idx);
						RTRANS(cons_trans[i]).poss_succ.insert(end_idx);

					}

					RTRANS(cons_trans[i]).poss_pred.erase(cons_trans[i]);
					RTRANS(cons_trans[i]).poss_succ.erase(cons_trans[i]);

				}
			}
		}

	}

	void Transplan::updateActionTransitionOrdering()
	{
		for (int i = 0; i < Transplan::n_activities; i++)
		{
			for (int r = 0; r < Transplan::n_resources; r++)
			{
				//std::cout << "On resource " << RES( r ).name << std::endl;
				IntVector rtrans = ACT(i).res_trans_map[r];

				for (int t1_idx = 0; t1_idx < rtrans.size(); t1_idx++)
				{
					int t1 = rtrans[t1_idx];
					int t1_s = RTRANS(t1).offset;
					int t1_e = t1_s + RTRANS(t1).duration;

					for (int t2_idx = t1_idx + 1; t2_idx < rtrans.size(); t2_idx++)
					{
						int t2 = rtrans[t2_idx];
						int t2_s = RTRANS(t2).offset;
						int t2_e = t2_s + RTRANS(t2).duration;

						/////case 1
						if ((t1_s <= t2_s && t1_e > t2_s) || (t2_s <= t1_s && t2_e > t1_s))
						{
							//std::cout << RTRANS( t1 ).index << " and " << RTRANS( t2 ).index << " must overlap " << std::endl;
							//t1 t2 parallel
							RTRANS(t1).poss_pred.erase(t2);
							RTRANS(t1).poss_succ.erase(t2);

							RTRANS(t2).poss_pred.erase(t1);
							RTRANS(t2).poss_succ.erase(t1);

						}
						else if (t1_e < t2_s)
						{
							///t1 --> t2
							//std::cout << RTRANS( t1 ).index << " and " << RTRANS( t2 ).index << " ---> " << std::endl;
							RTRANS(t1).poss_pred.erase(t2);
							RTRANS(t2).poss_succ.erase(t1);
						}
						else if (t2_e < t1_s)
						{
							///t2 --> t1
							//std::cout << RTRANS( t1 ).index << " and " << RTRANS( t2 ).index << " <--- " << std::endl;
							RTRANS(t1).poss_succ.erase(t2);
							RTRANS(t2).poss_pred.erase(t1);

						}
					}
				}

			}

			for (int sv = 0; sv < Transplan::n_statevars; sv++)
			{

			}
		}
	}

	void Transplan::search()
	{
		run_time_start = clock();
		BranchStrategy* strategy = new DefaultStrategy();
		Search* engine = new Search(strategy);
		engine->initialize();
		std::cout << "search created\n";

		if (engine->search() == TRANSPLAN::SOLVED)
		{
			engine->validatePlan();
			std::cout << "Solved\n";
		}
		else
		{
			std::cout << "No Solution\n";
		}

		delete strategy;
		delete engine;

	}

/*
 void Transplan::search_bb()
 {
 while(true)
 {
 run_time_start = clock();
 Search* engine = new Search();

 if ( engine->search() == TRANSPLAN::SOLVED )
 {
 engine->validate_plan();
 //std::cout << "Solved\n";
 //std::cout << "Cycle check failed = " << Transplan::cycle_check_fail << std::endl;
 //engine->stats.print_in_one_line(Transplan::problem_name);
 UPPER_BOUND = engine->stats.make_span-1;
 //std::cout << "New Upper Bound = " << UPPER_BOUND << std::endl;

 }
 else
 {
 std::cout << "No Solution\n";
 break;
 }

 delete engine;
 }

 }
 */

}
