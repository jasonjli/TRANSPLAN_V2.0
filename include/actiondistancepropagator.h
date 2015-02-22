/* 
 * File:   actiondistancechangedpropagator.h
 * Author: debdeep
 *
 * Created on 20 November 2014, 6:32 PM
 */

#ifndef ACTIONDISTANCEPROPAGATOR_H
#define	ACTIONDISTANCEPROPAGATOR_H

#include "propagator.h"
namespace TRANSPLAN
{
	class ActionDistancePropagator: public Propagator
	{
		private:
			PROP_STATUS propgateActionStartTime(int fromActIndex, int toActIndex, int dist, bool isLB)
			{
				if (isLB)
				{
					int minStart = manager.a_start(currentState, fromActIndex).min() + dist;
					RETURN_IF_FAILURE( manager.a_start(currentState, toActIndex).gq(minStart));
				}
				else
				{
					int maxStart = manager.a_start(currentState, toActIndex).max() - dist;
					RETURN_IF_FAILURE( manager.a_start(currentState, fromActIndex).lq(maxStart));
				}

				return TRANSPLAN::FIX;
			}

			///forward closure
			PROP_STATUS propagateForwardClosure(int fromActIndex, int toActIndex, int dist)
			{
				CSPGraphVar& distances = manager.a_dist(currentState);
				IntPairSet forwardSet;
				distances.getOutgoingEdgesToIndexSet(toActIndex, forwardSet);
				IntPairSet::iterator itr = forwardSet.begin();

				while (itr != forwardSet.end())
				{
					IntPair act = *itr;
					int totalDist = dist + act.second;
					RETURN_IF_FAILURE( distances.updateEdge(fromActIndex, act.first, totalDist));
					itr++;

				}

				return TRANSPLAN::FIX;
			}

			//backward closure
			PROP_STATUS propagateBackwardClosure(int fromActIndex, int toActIndex, int dist)
			{
				CSPGraphVar& distances = manager.a_dist(currentState);
				IntPairSet backwardSet;
				distances.getIncomingEdgesFromIndexSet(fromActIndex, backwardSet);
				IntPairSet::iterator itr = backwardSet.begin();

				while (itr != backwardSet.end())
				{
					IntPair act = *itr;
					int totalDist = dist + act.second;
					RETURN_IF_FAILURE( distances.updateEdge(act.first, toActIndex, totalDist));
					itr++;

				}
				return TRANSPLAN::FIX;
			}

			/*
			 * Infer precedence and anti-precedence constraint between transition on same domain object, based on the updated minimum distance between transitions
			 */
			PROP_STATUS inferPrecedenceConstraints(int objIndex, int fromActIndex, int toActIndex, int dist, bool isSVT)
			{
				IntVector fromTransitions = ACT(fromActIndex).getTransitionsOnDomainObject(objIndex, isSVT);
				IntVector toTransitions = ACT(toActIndex).getTransitionsOnDomainObject(objIndex, isSVT);

				if (fromTransitions.size() > 0 && toTransitions.size() > 0)
				{
					for (int j = 0; j < fromTransitions.size(); j++)
					{
						int fromTransIndex = fromTransitions[j];
						int fromTransOffset = Transplan::getTransOffset(fromTransIndex, isSVT);
						int fromTransDuration = Transplan::getTransDuration(fromTransIndex, isSVT);

						for (int k = 0; k < toTransitions.size(); k++)
						{
							int toTransIndex = toTransitions[k];
							int toTransOffset = Transplan::getTransOffset(toTransIndex, isSVT);
							int toTransDuration = Transplan::getTransDuration(toTransIndex, isSVT);

							if (dist + toTransOffset >= fromTransOffset + fromTransDuration)
							{
								RETURN_IF_FAIL( popagateTransitionPrecedence(fromTransIndex, toTransIndex, isSVT));
							}

							if (dist + toTransOffset + toTransDuration > fromTransOffset)
							{
								RETURN_IF_FAIL( popagateTransitionAntiPrecedence(toTransIndex, fromTransIndex, isSVT));
							}
						}
					}
				}

				return TRANSPLAN::FIX;
			}

			///infer precedence constraints
			PROP_STATUS propgateDistanceConstraints(int fromActIndex, int toActIndex, int dist)
			{
				int nSV = Transplan::n_statevars;
				int nRES = Transplan::n_resources;

				for (int i = 0; i < nSV; i++)
				{
					int svIndex = SV(i).index;
					RETURN_IF_FAIL( inferPrecedenceConstraints(svIndex, fromActIndex, toActIndex, dist, true));
				}

				for (int i = 0; i < nRES; i++)
				{
					int resIndex = RES(i).index;
					RETURN_IF_FAIL( inferPrecedenceConstraints(resIndex, fromActIndex, toActIndex, dist, false));

				}
				return TRANSPLAN::FIX;
			}

		public:

			ActionDistancePropagator(int propagatorIndex, VariablePropagatorManager& manager) :
				Propagator(propagatorIndex, manager)
			{

			}

			ActionDistancePropagator(const ActionDistancePropagator& prop) :
				Propagator(prop)
			{

			}

			PROP_STATUS propagate(SearchState* state)
			{
				this->currentState = state;
				CSPGraphVar& distances = manager.a_dist(currentState);

				while (!distances.updatedPair.empty())
				{
					IntPair actPair = distances.updatedPair.top();
					int fromActIndex = actPair.first;
					int toActIndex = actPair.second;

					int weight = distances.getEdgeWeight(fromActIndex, toActIndex);

					RETURN_IF_FAIL( propgateDistanceConstraints(fromActIndex, toActIndex, weight));

					if (manager.isActIncluded(currentState, fromActIndex))
					{
						RETURN_IF_FAIL( propgateActionStartTime(fromActIndex, toActIndex, weight, true));
						RETURN_IF_FAIL( propagateBackwardClosure(fromActIndex, toActIndex, weight));
					}

					if (manager.isActIncluded(currentState, toActIndex))
					{
						RETURN_IF_FAIL( propgateActionStartTime(fromActIndex, toActIndex, weight, false));
						RETURN_IF_FAIL( propagateForwardClosure(fromActIndex, toActIndex, weight));
					}

					distances.updatedPair.pop();
				}

				return TRANSPLAN::FIX;
			}

	};
}

#endif	/* ACTIONDISTANCECHANGEDPROPAGATOR_H */

