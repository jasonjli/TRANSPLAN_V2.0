/* 
 * File:   svtloweboundinferator.h
 * Author: debdeep
 *
 * Created on 22 November 2014, 9:55 AM
 */

#ifndef SVTLOWEBOUNDINFERATOR_H
#define	SVTLOWEBOUNDINFERATOR_H

#include "inferator.h"
#include "algorithm"
namespace TRANSPLAN
{

	class SVTLowerBoundInferator: public Inferator
	{
		private:

		public:
			int svtIndex;

			SVTLowerBoundInferator(int inferatorIndex, int svtIndex, VariablePropagatorManager& manager) :
				Inferator(inferatorIndex, manager), svtIndex(svtIndex)
			{

			}

			SVTLowerBoundInferator(const SVTLowerBoundInferator& infer) :
				Inferator(infer), svtIndex(svtIndex)
			{

			}

			PROP_STATUS infer(SearchState* state)
			{
				this->currentState = state;
				clearLBSet(svtIndex, true);
				//if this variable has prev[] assigned
				if (!manager.prev(currentState, svtIndex).assigned())
				{
					//if this variable is still unsupported

					IntSet previous = manager.prev(currentState, svtIndex).dom();

					IntPairVector suppVec;

					IntSet::iterator itr = previous.begin();

					while (itr != previous.end())
					{
						int prevIndex = *itr;

						if (prevIndex != NOT_IN_PLAN)
						{
							int endVal = manager.getTransEnd(currentState, prevIndex, true).min();
							suppVec.push_back(std::make_pair<int, int>(prevIndex, endVal));
						}

						itr++;
					}

					std::sort(suppVec.begin(), suppVec.end(), CompareIntPairSecondValMin());

					int startLBVal = suppVec[0].second;
					int lbSupIndex = suppVec[0].first;

					IMPLY_EXCL_ON_FAILURE(manager.getTransStart(currentState, svtIndex, true).gq(startLBVal), Transplan::getTransActIndex(svtIndex, true));

					addToLBSet(svtIndex, lbSupIndex, true);
				}


				return TRANSPLAN::FIX;
			}
	};
}

#endif	/* SVTLOWEBOUNDINFERATOR_H */

