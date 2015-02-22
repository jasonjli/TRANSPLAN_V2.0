/* 
 * File:   rtupperboundinferator.h
 * Author: debdeep
 *
 * Created on 22 November 2014, 9:56 AM
 */

#ifndef RTUPPERBOUNDINFERATOR_H
#define	RTUPPERBOUNDINFERATOR_H

#include "inferator.h"

namespace TRANSPLAN
{

	class RTUpperBoundInferator: public Inferator
	{
		private:

		public:
			int rtIndex;

			RTUpperBoundInferator(int inferatorIndex, int rtIndex, VariablePropagatorManager& manager) :
				Inferator(inferatorIndex, manager), rtIndex(rtIndex)
			{

			}

			RTUpperBoundInferator(const RTUpperBoundInferator& infer) :
				Inferator(infer), rtIndex(rtIndex)
			{

			}

			PROP_STATUS infer(SearchState* state)
			{
				this->currentState = state;

				clearUBSet(rtIndex, true);
				int remSupp = manager.getRemSupport(currentState, rtIndex);

				if (remSupp > 0)
				{
					//if this variable is still unsupported

					IntSet nextset = manager.poss_succ(currentState, rtIndex).dom();

					IntPairVector custVec;

					IntSet::iterator itr = nextset.begin();

					while (itr != nextset.end())
					{
						int nextIndex = *itr;

						if (nextIndex != NOT_IN_PLAN)
						{
							int startVal = manager.getTransStart(currentState, nextIndex, false).max();
							custVec.push_back(std::make_pair<int, int>(nextIndex, startVal));
						}
						itr++;
					}

					std::sort(custVec.begin(), custVec.end(), CompareIntPairSecondValMax());
					int totalDemand = 0;
					int startUBVal = INT_MAX;
					for (int i = 0; i < custVec.size() and totalDemand < remSupp; i++)
					{
						IntPair cust = custVec[i];
						totalDemand = totalDemand + manager.getRTSupportMax(currentState, rtIndex, cust.first);
						startUBVal = std::min(startUBVal, cust.second);
						addToUBSet(rtIndex, cust.first, false);
					}

					IMPLY_EXCL_ON_FAILURE(manager.getTransStart(currentState, rtIndex, false).lq(startUBVal), Transplan::getTransActIndex(rtIndex, false));

				}
				return TRANSPLAN::FIX;
			}
	};
}
#endif	/* RTUPPERBOUNDINFERATOR_H */

