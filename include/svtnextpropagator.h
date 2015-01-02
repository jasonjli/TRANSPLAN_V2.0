/* 
 * File:   svtnextchangedpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:16 PM
 */

#ifndef SVTNEXTCHANGEDPROPAGATOR_H
#define	SVTNEXTCHANGEDPROPAGATOR_H

#include "propagator.h"

namespace TRANSPLAN
{

	class SVTNextPropagator: public Propagator
	{
		private:
			PROP_STATUS propagateAssignment()
			{
				int assignedTransIndex = manager.next(currentState, svtIndex).val();

				if (assignedTransIndex != TRANSPLAN::NOT_IN_PLAN)
				{
					///make T inplan
					IMPLY_EXCL_ON_FAILURE(manager.svt_inplan(currentState, svtIndex).eqTrue(), Transplan::sv_trans[svtIndex].a_index);

					/// make T' inplan
					IMPLY_EXCL_ON_FAILURE(manager.svt_inplan(currentState, assignedTransIndex).eqTrue(), Transplan::sv_trans[assignedTransIndex].a_index);

					///post T-->T'
					RETURN_IF_FAIL( this->popagateTransitionPrecedence(svtIndex, assignedTransIndex, true));

					///check if any one is a PREVAIL transition
					if (Transplan::sv_trans[svtIndex].type == TRANSPLAN::PRV)
					{
						/// prevail inferator on svt_index
						currentState->activateInferator(manager.getPrevailInferatorIndex(svtIndex));
					}
					else
					{
						///prev[T'] = T
						IMPLY_EXCL_ON_FAILURE(manager.prev(currentState, assignedTransIndex).eq(svtIndex), Transplan::sv_trans[assignedTransIndex].a_index );

						///remove T from prev[T''] domain, for all T'' in next[T] domain except for T'
						IntSet nextDomain = manager.next(currentState, svtIndex).dom();
						IntSet::iterator itr = nextDomain.begin();
						for (; itr != nextDomain.end(); itr++)
						{
							if (*itr != assignedTransIndex)
							{
								IMPLY_EXCL_ON_FAILURE(manager.prev(currentState, *itr).nq(svtIndex), Transplan::sv_trans[*itr].a_index);
							}
						}

						/// remove T' from next[T''] domain, for all T'' in prev[T'] domain except for T
						IntSet prevDomain = manager.prev(currentState, assignedTransIndex).dom();
						itr = prevDomain.begin();
						for (; itr != prevDomain.end(); itr++)
						{
							if (*itr != svtIndex)
							{
								IMPLY_EXCL_ON_FAILURE(manager.next(currentState, *itr).nq(assignedTransIndex), Transplan::sv_trans[*itr].a_index);
							}
						}

					}

				}
				else
				{
					IMPLY_EXCL_ON_FAILURE(manager.svt_inplan(currentState, svtIndex).eqFalse(), Transplan::sv_trans[svtIndex].a_index);
				}

				return TRANSPLAN::FIX;
			}

			///propagate Next[T] != T'
			PROP_STATUS propagateRemovals()
			{
				while (!manager.next(currentState, svtIndex).prunedValues.empty())
				{
					int removedTransIndex = manager.next(currentState, svtIndex).prunedValues.top();

					///if removed value is NOT_IN_PLAN then action should be activated
					if (removedTransIndex == TRANSPLAN::NOT_IN_PLAN)
					{
						int actIndex = Transplan::sv_trans[svtIndex].a_index;

						RETURN_IF_FAILURE( manager.a_inplan(currentState, actIndex).eqTrue() );
					}
					else
					{
						///if this transition is a PREVAIL transition and an EFFECT transition is removed
						if (Transplan::sv_trans[svtIndex].type == TRANSPLAN::PRV)
						{
							///if the removed EFFECT transition's PREV[T] is assigned to T', i.e. T' is supporting the removed EFFECT transition
							if (manager.prev(currentState, removedTransIndex).assigned())
							{
								int assignedTrans = manager.prev(currentState, removedTransIndex).val();
								//then T' can't be assigned as supporter of this PREVAIL transition
								IMPLY_EXCL_ON_FAILURE(manager.prev(currentState, svtIndex).nq(assignedTrans), Transplan::sv_trans[svtIndex].a_index);
							}
						}
						else
						{
							///otherwise, Next[T] != T' ==> Prev[T'] = T
							IMPLY_EXCL_ON_FAILURE( manager.prev(currentState, removedTransIndex).nq(svtIndex), Transplan::sv_trans[removedTransIndex].a_index );
						}

						/// if removedIndex exists in SVT_UB set then need to call the SVT_UB inferator
						IntSet svtUBSet = manager.svt_UB(currentState, svtIndex).dom();
						if (svtUBSet.count(removedTransIndex) > 0)
						{
							currentState->activateInferator(manager.getUBInferatorIndex(true, svtIndex));
						}
					}

					manager.next(currentState, svtIndex).prunedValues.pop();
				}

				return TRANSPLAN::FIX;
			}

		public:
			int svtIndex;

			SVTNextPropagator(int svtIndex, int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager), svtIndex(svtIndex)
			{

			}

			SVTNextPropagator(const SVTNextPropagator& prop) :
				Propagator(prop), svtIndex(prop.svtIndex)
			{

			}

			/*
			 * Propagator NextChanged(T):
			 1) for the assignment: Next[T] = T':
			 if T' is not "NOT_IN_PLAN"
			 a) make Inplan[T] = Inplan[T'] = true
			 b) post T-->T'
			 b) post Prev[T']=T if EFFECT transitions, other wise call inference 6 for T' or T which ever is a PREVAIL transition
			 b) remove T' from all Prev[T'].Dom
			 c) remove T from all Next[T].Dom,
			 if T' is NOT_IN_PLAN
			 a) make Inplan[T] = false

			 2) for T' that is removed from Next[T], if T' in SVT_UB_Set of T, then call SVT_UB inference for T.
			 */
			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;
				if (manager.next(currentState, svtIndex).assigned())
				{
					RETURN_IF_FAIL(propagateAssignment());
				}
				else
				{
					RETURN_IF_FAIL(propagateRemovals());
				}
				return TRANSPLAN::FIX;
			}

	};
}

#endif	/* SVTNEXTCHANGEDPROPAGATOR_H */

