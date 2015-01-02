/* 
 * File:   svtprevchangedpropagator.h
 * Author: debdeep
 *
 * Created on 19 November 2014, 5:17 PM
 */

#ifndef SVTPREVCHANGEDPROPAGATOR_H
#define	SVTPREVCHANGEDPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{
	class SVTPrevPropagator: public Propagator
	{
		private:
			PROP_STATUS propagateAssignment()
			{
				int assignedTransIndex = manager.prev(currentState, svtIndex).val();

				if (assignedTransIndex != TRANSPLAN::NOT_IN_PLAN)
				{
					///check if any one is a PREVAIL transition
					if (Transplan::sv_trans[svtIndex].type == TRANSPLAN::PRV)
					{
						/// prevail inferator on svt_index
						currentState->activateInferator(manager.getPrevailInferatorIndex(svtIndex));
					}
					else
					{
						///next[T'] = T
						IMPLY_EXCL_ON_FAILURE(manager.next(currentState, assignedTransIndex).eq(svtIndex), Transplan::sv_trans[assignedTransIndex].a_index );
					}

				}
				else
				{
					IMPLY_EXCL_ON_FAILURE(manager.svt_inplan(currentState, svtIndex).eqFalse(), Transplan::sv_trans[svtIndex].a_index);
				}

				return TRANSPLAN::FIX;
			}

			PROP_STATUS propagateRemovals()
			{
				while (!manager.prev(currentState, svtIndex).prunedValues.empty())
				{
					int removedTransIndex = manager.prev(currentState, svtIndex).prunedValues.top();

					if (removedTransIndex == TRANSPLAN::NOT_IN_PLAN)
					{
						int actIndex = Transplan::sv_trans[svtIndex].a_index;

						RETURN_IF_FAILURE( manager.a_inplan(currentState, actIndex).eqTrue() );
					}
					else
					{
						if (Transplan::sv_trans[svtIndex].type == TRANSPLAN::PRV)
						{
							if (manager.next(currentState, removedTransIndex).assigned())
							{
								int assignedTrans = manager.next(currentState, removedTransIndex).val();
								IMPLY_EXCL_ON_FAILURE(manager.next(currentState, svtIndex).nq(assignedTrans), Transplan::sv_trans[svtIndex].a_index);
							}
						}
						else
						{
							///Prev[T] != T' ==> Prev[T'] = T
							IMPLY_EXCL_ON_FAILURE( manager.next(currentState, removedTransIndex).nq(svtIndex), Transplan::sv_trans[removedTransIndex].a_index );
						}

						/// if removedIndex exists in SVT_LB set then need to call the SVT_LB inferator
						IntSet svtLBSet = manager.svt_LB(currentState, svtIndex).dom();
						if (svtLBSet.count(removedTransIndex) > 0)
						{
							currentState->activateInferator(manager.getLBInferatorIndex(true, svtIndex));
						}
					}

					manager.prev(currentState, svtIndex).prunedValues.pop();
				}
				return TRANSPLAN::FIX;
			}
		public:
			int svtIndex;

			SVTPrevPropagator(int sctIndex, int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager), svtIndex(svtIndex)
			{

			}

			SVTPrevPropagator(const SVTPrevPropagator& prop) :
				Propagator(prop), svtIndex(prop.svtIndex)
			{

			}

			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;

				if (manager.prev(currentState, svtIndex).assigned())
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

#endif	/* SVTPREVCHANGEDPROPAGATOR_H */

