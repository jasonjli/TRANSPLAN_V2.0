/* 
 * File:   transitioninplanchangedpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:16 PM
 */

#ifndef TRANSITIONINPLANCHANGEDPROPAGATOR_H
#define	TRANSITIONINPLANCHANGEDPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{
	class TInplanPropagator: public Propagator
	{
		public:
			int tIndex;
			bool isSVT;

			TInplanPropagator(int tIndex, bool isSVT, int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager), tIndex(tIndex), isSVT(isSVT)
			{

			}

			TInplanPropagator(const TInplanPropagator& prop) :
				Propagator(prop), tIndex(prop.tIndex), isSVT(prop.isSVT)
			{

			}

			PROP_STATUS propagateAction(bool isIncluded, int actIndex)
			{
				if (isIncluded)
				{
					RETURN_IF_FAILURE(manager.a_inplan(currentState, actIndex).eqTrue());
				}
				else
				{
					RETURN_IF_FAILURE(manager.a_inplan(currentState, actIndex).eqFalse());
				}

				return TRANSPLAN::FIX;
			}

			PROP_STATUS propagateSVTActivation(bool isIncluded, int actIndex)
			{
				if (isIncluded)
				{
					IMPLY_EXCL_ON_FAILURE(manager.prev(currentState, tIndex).nq(TRANSPLAN::NOT_IN_PLAN), actIndex);
					IMPLY_EXCL_ON_FAILURE(manager.next(currentState, tIndex).nq(TRANSPLAN::NOT_IN_PLAN), actIndex);

					IntSet mustBeBefore = manager.svt_must_before(currentState, tIndex).dom();
					IntSet mustBeAfter = manager.svt_must_after(currentState, tIndex).dom();

					for (IntSet::iterator itr_before = mustBeBefore.begin(); itr_before != mustBeBefore.end(); itr_before++)
					{
						int tBefore = *itr_before;
						for (IntSet::iterator itr_after = mustBeAfter.begin(); itr_after != mustBeAfter.end(); itr_after++)
						{
							int tAfter = *itr_after;
							RETURN_IF_FAIL(popagateTransitionPrecedence(tBefore, tAfter, true));
						}
					}

					/////
					for (IntSet::iterator itr = mustBeBefore.begin(); itr != mustBeBefore.end(); itr++)
					{
						currentState->activateInferator(manager.getUBInferatorIndex(true, *itr));
					}

					for (IntSet::iterator itr = mustBeAfter.begin(); itr != mustBeAfter.end(); itr++)
					{
						currentState->activateInferator(manager.getLBInferatorIndex(true, *itr) );
					}

				}
				else
				{
					IMPLY_EXCL_ON_FAILURE(manager.prev(currentState, tIndex).eq(TRANSPLAN::NOT_IN_PLAN), actIndex);
					IMPLY_EXCL_ON_FAILURE(manager.next(currentState, tIndex).eq(TRANSPLAN::NOT_IN_PLAN), actIndex);

					IntSet prevDom = manager.prev(currentState, tIndex).dom();
					IntSet nextDom = manager.next(currentState, tIndex).dom();

					for (IntSet::iterator itr = prevDom.begin(); itr != prevDom.end(); itr++)
					{
						IMPLY_EXCL_ON_FAILURE(manager.next(currentState, *itr).nq(tIndex), actIndex);
					}

					for (IntSet::iterator itr = nextDom.begin(); itr != nextDom.end(); itr++)
					{
						IMPLY_EXCL_ON_FAILURE(manager.prev(currentState, *itr).nq(tIndex), actIndex);
					}
				}

				return TRANSPLAN::FIX;
			}

			PROP_STATUS propagateRTActivation(bool isIncluded, int actIndex)
			{
				if (isIncluded)
				{
					IntSet mustBeBefore = manager.rt_must_before(currentState, tIndex).dom();
					IntSet mustBeAfter = manager.rt_must_after(currentState, tIndex).dom();

					for (IntSet::iterator itr_before = mustBeBefore.begin(); itr_before != mustBeBefore.end(); itr_before++)
					{
						int tBefore = *itr_before;
						for (IntSet::iterator itr_after = mustBeAfter.begin(); itr_after != mustBeAfter.end(); itr_after++)
						{
							int tAfter = *itr_after;
							RETURN_IF_FAIL(popagateTransitionPrecedence(tBefore, tAfter, false));
						}
					}


					for (IntSet::iterator itr = mustBeBefore.begin(); itr != mustBeBefore.end(); itr++)
					{
						currentState->activateInferator(manager.getUBInferatorIndex(false, *itr));
					}

					for (IntSet::iterator itr = mustBeAfter.begin(); itr != mustBeAfter.end(); itr++)
					{
						currentState->activateInferator(manager.getLBInferatorIndex(false, *itr));
					}

				}
				else
				{

					IntSet possSupporters = manager.poss_pred(currentState, tIndex).dom();
					IntSet possCustomers  = manager.poss_succ(currentState, tIndex).dom();

					for (IntSet::iterator itr = possSupporters.begin(); itr != possSupporters.end(); itr++)
					{
						IMPLY_EXCL_ON_FAILURE(manager.rt_supp(currentState, *itr, tIndex).eq(0), actIndex);
					}

					for (IntSet::iterator itr = possCustomers.begin(); itr != possCustomers.end(); itr++)
					{
						IMPLY_EXCL_ON_FAILURE(manager.rt_supp(currentState, tIndex, *itr).eq(0), actIndex);
					}

				}

				return TRANSPLAN::FIX;
			}

			/*
			 * Propagator InplanChanged(T):
			 1) post Inplan(Act(T)) = True/False
			 3) remove/assign dummy variable from next and prev domain and set support[T,T] = 0/req(T) to support variable.
			 2) for each Transiton T' in MustBeBefore, T'' in MustBeAfter: Post T'--->T''

			 4) Simple Mutex Propagation ???
			 5) Trigger USB inference for all transition in MustBeAfter set.
			 */
			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;
				int actIndex = -1;
				bool isIncluded = false;

				if (isSVT)
				{
					actIndex = Transplan::sv_trans[tIndex].a_index;
					isIncluded = manager.svt_inplan(currentState, tIndex).isTrue();
				}
				else
				{
					actIndex = Transplan::r_trans[tIndex].a_index;
					isIncluded = manager.rt_inplan(currentState, tIndex).isTrue();
				}

				RETURN_IF_FAIL(propagateAction(isIncluded, actIndex));
				RETURN_IF_FAIL(propagateSVTActivation(isIncluded, actIndex));
				RETURN_IF_FAIL(propagateRTActivation(isIncluded, actIndex));

				return TRANSPLAN::FIX;
			}

	};
}

#endif	/* TRANSITIONINPLANCHANGEDPROPAGATOR_H */

