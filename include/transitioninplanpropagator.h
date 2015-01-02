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
		private:
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

			PROP_STATUS propagatePrecedenceRelation()
			{
				IntSet mustBeBefore = manager.getTransMustBefore(currentState, tIndex, isSVT).dom();
				IntSet mustBeAfter = manager.getTransMustAfter(currentState, tIndex, isSVT).dom();

				for (IntSet::iterator itr_before = mustBeBefore.begin(); itr_before != mustBeBefore.end(); itr_before++)
				{
					int tBefore = *itr_before;
					for (IntSet::iterator itr_after = mustBeAfter.begin(); itr_after != mustBeAfter.end(); itr_after++)
					{
						int tAfter = *itr_after;
						RETURN_IF_FAIL(popagateTransitionPrecedence(tBefore, tAfter, isSVT));
					}
				}

				/////
				for (IntSet::iterator itr = mustBeBefore.begin(); itr != mustBeBefore.end(); itr++)
				{
					currentState->activateInferator(manager.getUBInferatorIndex(isSVT, *itr));
				}

				for (IntSet::iterator itr = mustBeAfter.begin(); itr != mustBeAfter.end(); itr++)
				{
					currentState->activateInferator(manager.getLBInferatorIndex(isSVT, *itr));
				}

				return TRANSPLAN::FIX;
			}

			PROP_STATUS propagateSVTActivation(bool isIncluded, int actIndex)
			{
				if (isIncluded)
				{
					IMPLY_EXCL_ON_FAILURE(manager.prev(currentState, tIndex).nq(TRANSPLAN::NOT_IN_PLAN), actIndex);
					IMPLY_EXCL_ON_FAILURE(manager.next(currentState, tIndex).nq(TRANSPLAN::NOT_IN_PLAN), actIndex);

					RETURN_IF_FAIL(propagatePrecedenceRelation());

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
					RETURN_IF_FAIL(propagatePrecedenceRelation());

				}
				else
				{

					IntSet possSupporters = manager.poss_pred(currentState, tIndex).dom();
					IntSet possCustomers = manager.poss_succ(currentState, tIndex).dom();

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

			/*
			 *Propagator InplanChanged(T):
			 1) post Inplan(Act(T)) = True/False
			 1) inplan[t] = true
			 a) t=svt
			 1) remove NOT_IN_PLAN from Next[T] and Prev[T]
			 2)for each Transiton T' in MustBeBefore, T'' in MustBeAfter: Post T'--->T''
			 3) call SVT_LB inference for all T' in MustBeAfter set of T, and call SVT_UB inference for all T' in MustBeBefore set of T
			 b) t=rt
			 1) for each Transiton T' in MustBeBefore, T'' in MustBeAfter: Post T'--->T''
			 3) call SVT_LB inference for all T' in MustBeAfter set of T, and call SVT_UB inference for all T' in MustBeBefore set of T
			 2) inplan[t] = false
			 a) t=svt
			 1) assign NOT_IN_PLAN to Next[T] and PREV[T]
			 2) remove T from Next[T'] for each T' in Prev[T],
			 3) remove T from Prev[T'] for each T' in Next[T]
			 b) t=rt
			 1) set support[T,T'] = 0 for T' in possible customer set.
			 2) set support[T',T] = 0 for T' in possible supporter set.

			 4) Simple Mutex Propagation ???
			 5) Trigger USB inference for all transition in MustBeAfter set.
			 ///// if T is excluded then
			 */
			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;
				int actIndex = Transplan::getTransActIndex(tIndex, isSVT);
				bool isIncluded = manager.getTransInplan(currentState, tIndex, isSVT).isTrue();

				RETURN_IF_FAIL(propagateAction(isIncluded, actIndex));
				RETURN_IF_FAIL(propagateSVTActivation(isIncluded, actIndex));
				RETURN_IF_FAIL(propagateRTActivation(isIncluded, actIndex));

				return TRANSPLAN::FIX;
			}

	};
}

#endif	/* TRANSITIONINPLANCHANGEDPROPAGATOR_H */

