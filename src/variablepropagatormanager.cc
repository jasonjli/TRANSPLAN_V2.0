#include "transplan.h"
//#include "propagatorManager.h"
#include "variablepropagatormanager.h"
#include "svtrans.h"
#include "resourcetrans.h"
#include "configure.h"
#include "search.h"
#include "searchstate.h"
//#include "cspvariable.h"
/*
 #include "cspintvar.h"
 #include "cspintervalvar.h"
 #include "cspboolvar.h"
 #include "cspauxsetvar.h"
 #include "cspauxsetmonotonicincvar.h"
 #include "cspauxsetmonotonicdecvar.h"
 #include "cspauxintvar.h"
 #include "cspmaxcountervar.h"
 #include "cspgraphvar.h"
 */
#include "svtnextpropagator.h"
#include "svtprevpropagator.h"
#include "transitioninplanpropagator.h"
#include "transitionstarttimepropagator.h"
#include "transitionendtimepropagator.h"
#include "transitionmustbeafterpropagator.h"
#include "transitionmustbebeforepropagator.h"
#include "transitionmustnotafterpropagator.h"
#include "transitionsmustnotbeforepropagator.h"
#include "rtremainingdemandpropagator.h"
#include "rtremainingsupportpropagator.h"
#include "rtpossiblesupplierpropagator.h"
#include "rtpossiblecustomerpropagator.h"
#include "rtcustomerpropagator.h"
#include "rtsupplierpropagator.h"
#include "rtsupportpropagator.h"
#include "actioninplanpropagator.h"
#include "actionstarttimepropagator.h"
#include "actiondistancepropagator.h"
#include "svtloweboundinferator.h"
#include "svtupperboundinferator.h"
#include "prevailassignmentinferator.h"

namespace TRANSPLAN
{

	VariablePropagatorManager::VariablePropagatorManager(Search& search) :
		search(search)
	{
		propagatorCount = 0;
	}

	void VariablePropagatorManager::createCSP()
	{
		//PropagatorManager::init_propagators();

		std::cout << "Creating CSP Variables for SV-Trans\n";
		VariablePropagatorManager::createSVTransVariables();
		std::cout << "Creating CSP Variables for Res-Trans\n";
		VariablePropagatorManager::createRTransVariables();
		std::cout << "Creating CSP Variables for Actions\n";
		VariablePropagatorManager::createActionVariables();
		std::cout << "Creating CSP Variables for Resources and SVs\n";
		VariablePropagatorManager::createResAndSVVariables();

	}

	void VariablePropagatorManager::clearCSP()
	{
		//========================= FOR SV TRANS ===============================================
		VariablePropagatorManager::svt_next.clear(); //CSPInteger
		VariablePropagatorManager::svt_prev.clear(); //CSPInteger
		VariablePropagatorManager::svt_start_time.clear(); //CSPInterval
		VariablePropagatorManager::svt_end_time.clear(); //CSPInterval
		VariablePropagatorManager::svt_duration.clear(); //CSPInterval
		VariablePropagatorManager::svt_inplan_var.clear(); //CSPBoolVar

		//========================== FOR RES TRANS ==============================================

		VariablePropagatorManager::rt_remaining_demand.clear(); //CSPInterval
		VariablePropagatorManager::rt_remaining_support.clear(); //CSPInterval
		VariablePropagatorManager::rt_direct_inflow.clear(); //CSPInterval

		VariablePropagatorManager::rt_poss_pred.clear(); //CSPAuxMonoDecSet
		VariablePropagatorManager::rt_poss_succ.clear(); //CSPAuxMonoDecSet
		VariablePropagatorManager::rt_customers.clear(); //CSPAuxMonoIncSet
		VariablePropagatorManager::rt_suppliers.clear(); //CSPAuxMonoIncSet
		VariablePropagatorManager::rt_support_matrix.clear(); //CSPInterval
		VariablePropagatorManager::rt_start_time.clear(); //CSPInterval
		VariablePropagatorManager::rt_end_time.clear(); //CSPInterval
		VariablePropagatorManager::rt_duration.clear(); //CSPInterval
		VariablePropagatorManager::rt_inplan_var.clear(); //CSPBoolVar

		//========================== FOR ACTIONS ===================================================
		VariablePropagatorManager::a_start_time.clear(); //CSPInterval
		VariablePropagatorManager::a_inplan_var.clear(); //CSPBoolVar
		VariablePropagatorManager::a_disjunctions.clear(); //CSPAuxSet

		//========================== FOR RESOURCES ==================================================
		VariablePropagatorManager::r_only_can_supp.clear(); //CSPAuxSet
		VariablePropagatorManager::r_can_supp.clear(); //CSPAuxSet
		VariablePropagatorManager::r_need_supp.clear(); //CSPAuxSet
		VariablePropagatorManager::r_only_need_supp.clear(); //CSPAuxSet

		//========================== FOR STATE VARIABLES =============================================
		VariablePropagatorManager::sv_prefix.clear(); //CSPAuxInt
		VariablePropagatorManager::sv_only_can_supp.clear(); //CSPAuxSet
		VariablePropagatorManager::sv_can_supp.clear(); //CSPAuxSet
		VariablePropagatorManager::sv_need_supp.clear(); //CSPAuxSet
		VariablePropagatorManager::sv_only_need_supp.clear(); //CSPAuxSet

		//====================================================================================================

	}

	void VariablePropagatorManager::createSVTransVariables()
	{
		for (int i = 0; i < Transplan::n_sv_trans; ++i)
		{
			SVTrans* t = &Transplan::sv_trans[i];

			assert(t->index == i);

			IntSet prev = t->poss_pred;
			IntSet next = t->poss_succ;

			if (!t->is_init(DUMMY_START) && !t->is_goal(DUMMY_END))
			{
				next.insert(TRANSPLAN::NOT_IN_PLAN);
				prev.insert(TRANSPLAN::NOT_IN_PLAN);
			}

			if (t->is_init(DUMMY_START))
			{
				prev.insert(TRANSPLAN::DUMMY_VAL);
			}

			if (t->is_goal(DUMMY_END))
			{
				next.insert(TRANSPLAN::DUMMY_VAL);
			}

			svt_prev.push_back(createNewIntegerVar(prev));

			svt_next.push_back(createNewIntegerVar(next));

			//create start variable
			int act_deadline = ACT(t->a_index).deadline;
			int act_release = ACT(t->a_index).release_date;
			IntPair default_s_time(act_release, act_deadline - (t->duration));

			svt_start_time.push_back(createNewIntervalVar(default_s_time));

			//create end variable
			IntPair default_e_time(act_release + t->duration, act_deadline);

			svt_end_time.push_back(createNewIntervalVar(default_e_time));

			//inplan variables
			IntPair bool_pair(0, 1);
			svt_inplan_var.push_back(createNewIntervalVar(bool_pair));

			//////
			svt_must_after_set.push_back(createNewAuxMonoIncSet(IntSet()));
			svt_must_before_set.push_back(createNewAuxMonoIncSet(IntSet()));
			svt_mustnot_after_set.push_back(createNewAuxMonoIncSet(IntSet()));
			svt_mustnot_before_set.push_back(createNewAuxMonoIncSet(IntSet()));

			svt_LB_set.push_back(createNewAuxSet(IntSet()));
			svt_UB_set.push_back(createNewAuxSet(IntSet()));
			svt_LBChildren_set.push_back(createNewAuxSet(IntSet()));
			svt_UBChildren_set.push_back(createNewAuxSet(IntSet()));

			////////// Inferators //////////
			this->inferators.push_back(new SVTLowerBoundInferator(inferatorCount, t->index, *this));
			svt_LB_inferators.insert(std::make_pair(t->index, inferatorCount++));

			this->inferators.push_back(new SVTUpperBoundInferator(inferatorCount, t->index, *this));
			svt_UB_inferators.insert(std::make_pair(t->index, inferatorCount++));

			if (t->type = TRANSPLAN::PRV)
			{
				this->inferators.push_back(new PrevailAssignmentInferator(inferatorCount, t->index, *this));
				prevail_inferators.insert(std::make_pair(t->index, inferatorCount++));
			}
		}

	}

	void VariablePropagatorManager::createRTransVariables()
	{
		for (int i = 0; i < Transplan::n_r_trans; ++i)
		{
			RTrans* t = &Transplan::r_trans[i];

			assert(t->index == i);

			assert(t->req != 0);

			IntPair p_rem_demand(0, t->req);
			IntPair p_rem_supp(0, t->req);
			IntPair p_dir_inflow(0, t->req);

			if (t->is_init(DUMMY_START))
			{
				p_rem_demand.second = 0;
				p_dir_inflow.first = t->req;
			}
			else if (t->is_goal(DUMMY_END))
			{
				p_rem_supp.second = 0;
			}

			//remaining demand
			rt_remaining_demand.push_back(createNewIntervalVar(p_rem_demand));

			//remaining support
			rt_remaining_support.push_back(createNewIntervalVar(p_rem_supp));

			//direct inflow
			rt_direct_inflow.push_back(createNewIntervalVar(p_dir_inflow));

			/// possible predecessors
			rt_poss_pred.push_back(createNewAuxMonoDecSet(t->poss_pred));

			//possible successors
			rt_poss_succ.push_back(createNewAuxMonoDecSet(t->poss_succ));

			rt_customers.push_back(createNewAuxMonoIncSet(IntSet()));

			rt_suppliers.push_back(createNewAuxMonoIncSet(IntSet()));

			int act_deadline = Transplan::activities[t->a_index].deadline;
			int act_release = Transplan::activities[t->a_index].release_date;
			IntPair default_s_time(act_release, act_deadline - (t->duration));

			//create start variable
			rt_start_time.push_back(createNewIntervalVar(default_s_time));

			IntPair default_e_time(t->duration, act_deadline);
			//create end variable
			rt_end_time.push_back(createNewIntervalVar(default_e_time));

			//inplan variables
			IntPair bool_pair(0, 1);
			rt_inplan_var.push_back(createNewIntervalVar(bool_pair));

			rt_must_after_set.push_back(createNewAuxMonoIncSet(IntSet()));
			rt_must_before_set.push_back(createNewAuxMonoIncSet(IntSet()));
			rt_mustnot_after_set.push_back(createNewAuxMonoIncSet(IntSet()));
			rt_mustnot_before_set.push_back(createNewAuxMonoIncSet(IntSet()));

			rt_LB_set.push_back(createNewAuxSet(IntSet()));
			rt_UB_set.push_back(createNewAuxSet(IntSet()));
			rt_LBChildren_set.push_back(createNewAuxSet(IntSet()));
			rt_UBChildren_set.push_back(createNewAuxSet(IntSet()));

			////////// Inferators   /////////////////////////////////////////////
			this->inferators.push_back(new SVTLowerBoundInferator(inferatorCount, t->index, *this));
			rt_LB_inferators.insert(std::make_pair(t->index, inferatorCount++));

			this->inferators.push_back(new SVTUpperBoundInferator(inferatorCount, t->index, *this));
			rt_UB_inferators.insert(std::make_pair(t->index, inferatorCount++));

		}

		std::cout << "Creating RT-SUpport variables " << std::endl;
		createRTransSupportVariables();

	}

	//static

	void VariablePropagatorManager::createRTransSupportVariables()
	{
		for (int i = 0; i < Transplan::n_r_trans; ++i)
		{
			rt_support_matrix.push_back(std::vector<int>());

			for (int j = 0; j < Transplan::n_r_trans; ++j)
			{
				rt_support_matrix[i].push_back(-1);
			}
		}

		for (int r = 0; r < Transplan::n_resources; r++)
		{
			IntVector trans = RES(r).transitions;
			for (int t = 0; t < trans.size(); t++)
			{
				int i = trans[t];
				IntSet poss_succ = RTRANS(i).poss_succ;
				for (IntSet::iterator itr = poss_succ.begin(); itr != poss_succ.end(); itr++)
				{
					int j = *itr;
					int max_supp = std::min(RTRANS_RES_REQ(RTRANS(i).index), RTRANS_RES_REQ(RTRANS(j).index));
					IntPair supp(0, max_supp);

					//create support  variables
					//std::cout << "from "<< i << "  to " << j << std::endl;
					rt_support_matrix[i][j] = createNewIntervalVar(supp);
				}
			}
		}

		//PropagatorManager::addRTSupportProps();
	}

	void VariablePropagatorManager::printSupportMatrix(SearchState* state, int res_index)
	{
		IntSet poss_succ = RTRANS(res_index).poss_succ;
		for (int j = 0; j < Transplan::n_r_trans; ++j)
		{

			if (poss_succ.count(RTRANS(j).index) != 0)
			{
				int max_supp = std::min(RTRANS_RES_REQ(RTRANS(res_index).index), RTRANS_RES_REQ(RTRANS(j).index));
				IntPair supp(0, max_supp);
				std::cout << "Supp (" << res_index << "," << RTRANS(j).index << "= " << max_supp << std::endl;
				//create support  variables
				// rt_support_matrix[i].push_back( createNewIntervalVar( supp ) );
			}
			else
			{
				std::cout << "Supp (" << res_index << "," << RTRANS(j).index << "= " << 0 << std::endl;
			}
		}
	}

	void VariablePropagatorManager::createActionVariables()
	{
		//std::cout<<"Creating variabls for actions....\n";
		/******************************************************************************
		 /**************  create variables for actions
		 ******************************************************************************/
		for (int i = 0; i < Transplan::n_activities; ++i)
		{
			Activity* act = &Transplan::activities[i];

			assert(act->index == i);

			//inplan variables
			IntPair bool_pair(0, 1);
			a_inplan_var.push_back(createNewIntervalVar(bool_pair));

			a_disjunctions.push_back(createNewAuxSet(IntSet()));

			///start time variables
			IntPair default_s_time(Transplan::activities[act->index].release_date, Transplan::activities[act->index].deadline);
			a_start_time.push_back(createNewIntervalVar(default_s_time));

			//PropagatorManager::addActProp(act->index);
			//PropagatorManager::createPropagatorsForAction(act->index);
		}

	}

	void VariablePropagatorManager::createActionDistanceVariables()
	{
		this->actionDistanceGraphIndex = search.getInitialState()->newCSPGraphVar(Transplan::n_activities);
	}

	void VariablePropagatorManager::createResAndSVVariables()
	{
		/****************************************************************************
		 *****************   create variables for  resources ************************
		 ***** for each resource create s ********************
		 ****************************************************************************/

		for (int i = 0; i < Transplan::n_resources; ++i)
		{
			r_only_can_supp.push_back(createNewAuxSet(IntSet()));
			r_can_supp.push_back(createNewAuxSet(IntSet()));

			r_need_supp.push_back(createNewAuxSet(IntSet()));
			r_only_need_supp.push_back(createNewAuxSet(IntSet()));

		}

		for (int i = 0; i < Transplan::n_statevars; ++i)
		{
			sv_prefix.push_back(createNewAuxIntVar(SV(i).init_trans_index));

			sv_only_can_supp.push_back(createNewAuxSet(IntSet()));
			sv_can_supp.push_back(createNewAuxSet(IntSet()));

			sv_need_supp.push_back(createNewAuxSet(IntSet()));
			sv_only_need_supp.push_back(createNewAuxSet(IntSet()));
		}

	}

	void VariablePropagatorManager::addSVTPropagators(int trans_idx)
	{
		SearchState * initState = search.getInitialState();

		////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new SVTNextPropagator(trans_idx, propagatorCount, *this));
		next(initState, trans_idx).addPropagator(propagatorCount++);

		//////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new SVTPrevPropagator(trans_idx, propagatorCount, *this));
		prev(initState, trans_idx).addPropagator(propagatorCount++);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new TInplanPropagator(trans_idx, true, propagatorCount, *this));
		svt_inplan(initState, trans_idx).addPropagator(propagatorCount++);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new TStartTimePropagator(trans_idx, true, propagatorCount, *this));
		svt_start(initState, trans_idx).addPropagator(propagatorCount++);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new TEndTimePropagator(trans_idx, true, propagatorCount, *this));
		svt_end(initState, trans_idx).addPropagator(propagatorCount++);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new TMustBeAfterPropagator(trans_idx, true, propagatorCount, *this));
		svt_must_after(initState, trans_idx).addPropagator(propagatorCount++);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new TMustBeBeforePropagator(trans_idx, true, propagatorCount, *this));
		svt_must_before(initState, trans_idx).addPropagator(propagatorCount++);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new TMustNotAfterPropagator(trans_idx, true, propagatorCount, *this));
		svt_mustnot_after(initState, trans_idx).addPropagator(propagatorCount++);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new TMustNotBeforePropagator(trans_idx, true, propagatorCount, *this));
		svt_mustnot_before(initState, trans_idx).addPropagator(propagatorCount++);

	}

	void VariablePropagatorManager::addRTPropagators(int trans_idx)
	{
		SearchState * initState = search.getInitialState();

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new RTRemainingDemandPropagator(trans_idx, propagatorCount, *this));
		rem_demand(initState, trans_idx).addPropagator(propagatorCount++);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new RTRemainingSupportPropagator(trans_idx, propagatorCount, *this));
		rem_supp(initState, trans_idx).addPropagator(propagatorCount++);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new RTPossibleSupplierPropagator(trans_idx, propagatorCount, *this));
		poss_pred(initState, trans_idx).addPropagator(propagatorCount++);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new RTPossibleCustomerPropagator(trans_idx, propagatorCount, *this));
		poss_succ(initState, trans_idx).addPropagator(propagatorCount++);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new RTCustomerPropagator(trans_idx, propagatorCount, *this));
		customers(initState, trans_idx).addPropagator(propagatorCount++);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new RTSupplierPropagator(trans_idx, propagatorCount, *this));
		suppliers(initState, trans_idx).addPropagator(propagatorCount++);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new TInplanPropagator(trans_idx, false, propagatorCount, *this));
		rt_inplan(initState, trans_idx).addPropagator(propagatorCount++);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new TStartTimePropagator(trans_idx, false, propagatorCount, *this));
		rt_start(initState, trans_idx).addPropagator(propagatorCount++);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new TEndTimePropagator(trans_idx, false, propagatorCount, *this));
		rt_end(initState, trans_idx).addPropagator(propagatorCount++);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new TMustBeAfterPropagator(trans_idx, false, propagatorCount, *this));
		rt_must_after(initState, trans_idx).addPropagator(propagatorCount++);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new TMustBeBeforePropagator(trans_idx, false, propagatorCount, *this));
		rt_must_before(initState, trans_idx).addPropagator(propagatorCount++);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new TMustNotAfterPropagator(trans_idx, false, propagatorCount, *this));
		rt_mustnot_after(initState, trans_idx).addPropagator(propagatorCount++);

		////////////////////////////////////////////////////////////////////////////////////////////////////
		this->propagators.push_back(new TMustNotBeforePropagator(trans_idx, false, propagatorCount, *this));
		rt_mustnot_before(initState, trans_idx).addPropagator(propagatorCount++);
	}

	void VariablePropagatorManager::addRTSupportPropagators()
	{
		for (int i = 0; i < Transplan::n_r_trans; ++i)
		{
			for (int j = 0; j < Transplan::n_r_trans; ++j)
			{
				if (rt_support_matrix[i][j] != -1)
				{
					propagators.push_back(new RTSupportPropagator(i, j, propagatorCount, *this));
					rt_supp(search.getInitialState(), i, j).addPropagator(propagatorCount++);
				}
			}
		}
	}

	void VariablePropagatorManager::addActionPropagators(int act_idx)
	{
		SearchState * initState = search.getInitialState();

		this->propagators.push_back(new ActionInplanPropagator(act_idx, propagatorCount, *this));
		a_inplan(initState, act_idx).addPropagator(propagatorCount++);

		this->propagators.push_back(new ActionStartTimePropagator(act_idx, propagatorCount, *this));
		a_start(initState, act_idx).addPropagator(propagatorCount++);

	}

	void VariablePropagatorManager::addActionDistancePropagator()
	{
		this->propagators.push_back(new ActionDistancePropagator(propagatorCount, *this));
		a_dist(this->search.getInitialState()).addPropagator(propagatorCount++);
	}

	int VariablePropagatorManager::createNewIntegerVar(IntSet dom)
	{
		return search.getInitialState()->newCSPIntVar(dom);
	}

	int VariablePropagatorManager::createNewIntervalVar(IntPair range)
	{
		return search.getInitialState()->newCSPIntervalVar(range);
	}

	int VariablePropagatorManager::createNewBoolVar()
	{
		return search.getInitialState()->newCSPBoolVar();
	}

	int VariablePropagatorManager::createNewAuxMonoDecSet(IntSet set)
	{
		return search.getInitialState()->newCSPAuxMonoDecSetVar(set);
	}

	int VariablePropagatorManager::createNewAuxMonoIncSet(IntSet set)
	{
		return search.getInitialState()->newCSPAuxMonoIncSetVar(set);
	}

	int VariablePropagatorManager::createNewAuxSet(IntSet set)
	{
		return search.getInitialState()->newCSPAuxSetVar(set);
	}

	int VariablePropagatorManager::createNewAuxIntVar(int init_val)
	{
		return search.getInitialState()->newCSPAuxIntVar(init_val);
	}

	//////////////////////////////////////////////////////////////////////////////
	//////  Access CSP variables ////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////


	//====================== State Variable Transitions ===================================

	CSPIntVar& VariablePropagatorManager::prev(SearchState* state, int svt_index)
	{
		return state->getCSPIntVar(VariablePropagatorManager::svt_prev[svt_index]);
	}

	CSPIntVar& VariablePropagatorManager::next(SearchState* state, int svt_index)
	{
		return state->getCSPIntVar(svt_next[svt_index]);
	}

	CSPIntervalVar& VariablePropagatorManager::svt_start(SearchState* state, int svt_index)
	{
		return state->getCSPIntervalVar(svt_start_time[svt_index]);
	}

	CSPIntervalVar& VariablePropagatorManager::svt_end(SearchState* state, int svt_index)
	{
		return state->getCSPIntervalVar(svt_end_time[svt_index]);
	}

	CSPBoolVar& VariablePropagatorManager::svt_inplan(SearchState* state, int svt_index)
	{
		return state->getCSPBoolVar(svt_inplan_var[svt_index]);
	}

	CSPAuxSetMonoIncVar& VariablePropagatorManager::svt_must_after(SearchState* state, int svt_index)
	{
		return state->getCSPAuxSetMonoIncVar(svt_must_after_set[svt_index]);
	}

	CSPAuxSetMonoIncVar& VariablePropagatorManager::svt_must_before(SearchState* state, int svt_index)
	{
		return state->getCSPAuxSetMonoIncVar(svt_must_before_set[svt_index]);
	}

	CSPAuxSetMonoIncVar& VariablePropagatorManager::svt_mustnot_after(SearchState* state, int svt_index)
	{
		return state->getCSPAuxSetMonoIncVar(svt_mustnot_after_set[svt_index]);
	}

	CSPAuxSetMonoIncVar& VariablePropagatorManager::svt_mustnot_before(SearchState* state, int svt_index)
	{
		return state->getCSPAuxSetMonoIncVar(svt_mustnot_before_set[svt_index]);
	}

	CSPAuxSetVar& VariablePropagatorManager::svt_LB(SearchState* state, int svt_index)
	{
		return state->getCSPAuxSetVar(svt_LB_set[svt_index]);
	}
	CSPAuxSetVar& VariablePropagatorManager::svt_UB(SearchState* state, int svt_index)
	{
		return state->getCSPAuxSetVar(svt_UB_set[svt_index]);
	}

	CSPAuxSetVar& VariablePropagatorManager::svt_LBChildren(SearchState* state, int svt_index)
	{
		return state->getCSPAuxSetVar(svt_LBChildren_set[svt_index]);
	}
	CSPAuxSetVar& VariablePropagatorManager::svt_UBChildren(SearchState* state, int svt_index)
	{
		return state->getCSPAuxSetVar(svt_UBChildren_set[svt_index]);
	}
	//======================== Resource Transitions =========================================

	CSPIntervalVar& VariablePropagatorManager::rem_demand(SearchState* state, int rt_index)
	{
		return state->getCSPIntervalVar(rt_remaining_demand[rt_index]);
	}

	CSPIntervalVar& VariablePropagatorManager::rem_supp(SearchState* state, int rt_index)
	{
		return state->getCSPIntervalVar(rt_remaining_support[rt_index]);
	}

	CSPIntervalVar& VariablePropagatorManager::direct_inflow(SearchState* state, int rt_index)
	{
		return state->getCSPIntervalVar(rt_direct_inflow[rt_index]);
	}

	CSPAuxSetMonoDecVar& VariablePropagatorManager::poss_pred(SearchState* state, int rt_index)
	{
		return state->getCSPAuxSetMonoDecVar(rt_poss_pred[rt_index]);
	}

	CSPAuxSetMonoDecVar& VariablePropagatorManager::poss_succ(SearchState* state, int rt_index)
	{
		return state->getCSPAuxSetMonoDecVar(rt_poss_succ[rt_index]);
	}

	CSPAuxSetMonoIncVar& VariablePropagatorManager::customers(SearchState* state, int rt_index)
	{
		return state->getCSPAuxSetMonoIncVar(rt_customers[rt_index]);
	}

	CSPAuxSetMonoIncVar& VariablePropagatorManager::suppliers(SearchState* state, int rt_index)
	{
		return state->getCSPAuxSetMonoIncVar(rt_suppliers[rt_index]);
	}

	CSPIntervalVar& VariablePropagatorManager::rt_start(SearchState* state, int rt_index)
	{
		return state->getCSPIntervalVar(rt_start_time[rt_index]);
	}

	CSPIntervalVar& VariablePropagatorManager::rt_end(SearchState* state, int rt_index)
	{
		return state->getCSPIntervalVar(rt_end_time[rt_index]);
	}

	bool VariablePropagatorManager::has_supp(int rt_from, int rt_to)
	{
		return rt_support_matrix[rt_from][rt_to] != -1;
	}

	CSPIntervalVar& VariablePropagatorManager::rt_supp(SearchState* state, int rt_from, int rt_to)
	{
		assert(rt_support_matrix[rt_from][rt_to] != -1);
		return state->getCSPIntervalVar(rt_support_matrix[rt_from][rt_to]);
	}

	CSPBoolVar& VariablePropagatorManager::rt_inplan(SearchState* state, int rt_index)
	{
		return state->getCSPBoolVar(rt_inplan_var[rt_index]);
	}

	CSPAuxSetMonoIncVar& VariablePropagatorManager::rt_must_after(SearchState* state, int rt_index)
	{
		return state->getCSPAuxSetMonoIncVar(rt_must_after_set[rt_index]);
	}

	CSPAuxSetMonoIncVar& VariablePropagatorManager::rt_must_before(SearchState* state, int rt_index)
	{
		return state->getCSPAuxSetMonoIncVar(rt_must_before_set[rt_index]);
	}

	CSPAuxSetMonoIncVar& VariablePropagatorManager::rt_mustnot_after(SearchState* state, int rt_index)
	{
		return state->getCSPAuxSetMonoIncVar(rt_mustnot_after_set[rt_index]);
	}

	CSPAuxSetMonoIncVar& VariablePropagatorManager::rt_mustnot_before(SearchState* state, int rt_index)
	{
		return state->getCSPAuxSetMonoIncVar(rt_mustnot_before_set[rt_index]);
	}

	CSPAuxSetVar& VariablePropagatorManager::rt_LB(SearchState* state, int rt_index)
	{
		return state->getCSPAuxSetVar(rt_LB_set[rt_index]);
	}

	CSPAuxSetVar& VariablePropagatorManager::rt_UB(SearchState* state, int rt_index)
	{
		return state->getCSPAuxSetVar(rt_UB_set[rt_index]);
	}

	CSPAuxSetVar& VariablePropagatorManager::rt_LBChildren(SearchState* state, int rt_index)
	{
		return state->getCSPAuxSetVar(rt_LBChildren_set[rt_index]);
	}

	CSPAuxSetVar& VariablePropagatorManager::rt_UBChildren(SearchState* state, int rt_index)
	{
		return state->getCSPAuxSetVar(rt_UBChildren_set[rt_index]);
	}

	/*************************************************************
	 ******** Actions ********************************************
	 ************************************************************/

	CSPIntervalVar& VariablePropagatorManager::a_start(SearchState* state, int a_index)
	{
		return state->getCSPIntervalVar(a_start_time[a_index]);
	}

	CSPBoolVar& VariablePropagatorManager::a_inplan(SearchState* state, int a_index)
	{
		return state->getCSPBoolVar(a_inplan_var[a_index]);
	}

	CSPGraphVar& VariablePropagatorManager::a_dist(SearchState* state)
	{
		return state->getCSPGraphVar(actionDistanceGraphIndex);
	}

	CSPAuxSetVar& VariablePropagatorManager::a_disjuncts(SearchState* state, int a_index)
	{
		return state->getCSPAuxSetVar(a_disjunctions[a_index]);
	}

	/*************************************************************
	 ********** Resource **********************
	 ************************************************************/

	CSPAuxSetVar& VariablePropagatorManager::res_only_can_supp(SearchState* state, int r_index)
	{
		return state->getCSPAuxSetVar(r_only_can_supp[r_index]);
	}

	CSPAuxSetVar& VariablePropagatorManager::res_can_supp(SearchState* state, int r_index)
	{
		return state->getCSPAuxSetVar(r_can_supp[r_index]);
	}

	CSPAuxSetVar& VariablePropagatorManager::res_need_supp(SearchState* state, int r_index)
	{
		return state->getCSPAuxSetVar(r_need_supp[r_index]);
	}

	CSPAuxSetVar& VariablePropagatorManager::res_only_need_supp(SearchState* state, int r_index)
	{
		return state->getCSPAuxSetVar(r_only_need_supp[r_index]);
	}

	//////////////////////////////////////////  State variables /////////////////////////////////////////

	CSPAuxSetVar& VariablePropagatorManager::svar_only_can_supp(SearchState* state, int r_index)
	{
		return state->getCSPAuxSetVar(sv_only_can_supp[r_index]);
	}

	CSPAuxSetVar& VariablePropagatorManager::svar_can_supp(SearchState* state, int r_index)
	{
		return state->getCSPAuxSetVar(sv_can_supp[r_index]);
	}

	CSPAuxSetVar& VariablePropagatorManager::svar_need_supp(SearchState* state, int r_index)
	{
		return state->getCSPAuxSetVar(sv_need_supp[r_index]);
	}

	CSPAuxSetVar& VariablePropagatorManager::svar_only_need_supp(SearchState* state, int r_index)
	{
		return state->getCSPAuxSetVar(sv_only_can_supp[r_index]);
	}

	CSPAuxIntVar& VariablePropagatorManager::prefix(SearchState* state, int sv_index)
	{
		return state->getCSPAuxIntVar(sv_prefix[sv_index]);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////

	bool VariablePropagatorManager::isActIncluded(SearchState* state, int act_index)
	{
		return a_inplan(state, act_index).isTrue() || !a_inplan(state, act_index).assigned();
	}

	bool VariablePropagatorManager::isActExcluded(SearchState* state, int act_index)
	{
		return a_inplan(state, act_index).isFalse() || !a_inplan(state, act_index).assigned();
	}

	bool VariablePropagatorManager::isSVTIncluded(SearchState* state, int svt_index)
	{
		return svt_inplan(state, svt_index).isTrue() || !a_inplan(state, svt_index).assigned();
	}

	bool VariablePropagatorManager::isRTIncluded(SearchState* state, int rt_index)
	{
		return rt_inplan(state, rt_index).isTrue() || !a_inplan(state, rt_index).assigned();
	}

	int VariablePropagatorManager::getRemDemand(SearchState* state, int rt_index)
	{
		return rem_demand(state, rt_index).max();
	}

	int VariablePropagatorManager::getRemSupport(SearchState* state, int rt_index)
	{
		return rem_supp(state, rt_index).max();
	}

	int VariablePropagatorManager::getInflow(SearchState* state, int rt_index)
	{
		return direct_inflow(state, rt_index).min();
	}

	int VariablePropagatorManager::getInflowGap(SearchState* state, int rt_index)
	{
		return direct_inflow(state, rt_index).max() - direct_inflow(state, rt_index).min();
	}

	int VariablePropagatorManager::getRTSupport(SearchState* state, int t_from, int t_to)
	{
		return rt_supp(state, t_from, t_to).max();
	}

	bool VariablePropagatorManager::isRTSupporting(SearchState* state, int t_from, int t_to)
	{
		int min_supp = rt_supp(state, t_from, t_to).min();

		return (min_supp > 0);
	}

}
