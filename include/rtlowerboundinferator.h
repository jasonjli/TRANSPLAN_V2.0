/* 
 * File:   rtlowerboundinferator.h
 * Author: debdeep
 *
 * Created on 22 November 2014, 9:55 AM
 */

#ifndef RTLOWERBOUNDINFERATOR_H
#define	RTLOWERBOUNDINFERATOR_H

#include "inferator.h"

namespace TRANSPLAN
{

	class RTLowerBoundInferator: public Inferator
	{
		private:

		public:
			int rtIndex;

			RTLowerBoundInferator(int inferatorIndex, int rtIndex, VariablePropagatorManager& manager) :
				Inferator(inferatorIndex, manager), rtIndex(rtIndex)
			{

			}

			RTLowerBoundInferator(const RTLowerBoundInferator& infer) :
				Inferator(infer), rtIndex(rtIndex)
			{

			}

			PROP_STATUS infer(SearchState* state)
			{
				this->currentState = state;

				clearLBSet(rtIndex, true);
				int remDemand = manager.getRemDemand(currentState, rtIndex);

				if ( remDemand > 0 )
				{
					//if this variable is still unsupported

					IntSet previous = manager.poss_pred(currentState, rtIndex).dom();

					IntPairVector suppVec;

					IntSet::iterator itr = previous.begin();

					while (itr != previous.end())
					{
						int prevIndex = *itr;

						if (prevIndex != NOT_IN_PLAN)
						{
							int endVal = manager.getTransEnd(currentState, prevIndex, false).min();
							suppVec.push_back(std::make_pair<int, int>(prevIndex, endVal));
						}
						itr++;
					}

					std::sort(suppVec.begin(), suppVec.end(), CompareIntPairSecondValMin());
					int totalSupply = 0;
					int startLBVal = 0;
					for( int i = 0; i < suppVec.size() and totalSupply < remDemand; i++)
					{
						IntPair supp = suppVec[i];
						totalSupply = totalSupply + manager.getRTSupportMax(currentState, supp.first, rtIndex);
						startLBVal = std::max(startLBVal, supp.second);
						addToLBSet(rtIndex, supp.first, false);
					}

					IMPLY_EXCL_ON_FAILURE(manager.getTransStart(currentState, rtIndex, false).gq(startLBVal), Transplan::getTransActIndex(rtIndex, false));


				}

				return TRANSPLAN::FIX;
			}
	};
}

#endif	/* RTLOWERBOUNDINFERATOR_H */

