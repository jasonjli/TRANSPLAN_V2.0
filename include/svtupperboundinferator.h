/* 
 * File:   svtupperboundinferator.h
 * Author: debdeep
 *
 * Created on 22 November 2014, 9:55 AM
 */

#ifndef SVTUPPERBOUNDINFERATOR_H
#define	SVTUPPERBOUNDINFERATOR_H

#include "inferator.h"

namespace TRANSPLAN
{

	class SVTUpperBoundInferator: public Inferator
	{
		private:

		public:
			int svtIndex;

			SVTUpperBoundInferator(int inferatorIndex, int svtIndex, VariablePropagatorManager& manager) :
				Inferator(inferatorIndex, manager), svtIndex(svtIndex)
			{

			}

			SVTUpperBoundInferator(const SVTUpperBoundInferator& infer) :
				Inferator(infer), svtIndex(svtIndex)
			{

			}

			PROP_STATUS infer(SearchState* state)
			{
				this->currentState = state;

				clearUBSet(svtIndex, true);
				//if this variable has prev[] assigned
				if (!manager.next(currentState, svtIndex).assigned())
				{
					//if this variable is still unsupported

					IntSet nextset = manager.next(currentState, svtIndex).dom();

					IntPairVector custVec;

					IntSet::iterator itr = nextset.begin();

					while (itr != nextset.end())
					{
						int nextIndex = *itr;

						if (nextIndex != NOT_IN_PLAN)
						{
							int startVal = manager.getTransStart(currentState, nextIndex, true).max();
							custVec.push_back(std::make_pair<int, int>(nextIndex, startVal));
						}
						itr++;
					}

					std::sort(custVec.begin(), custVec.end(), CompareIntPairSecondValMax());

					int endUBVal = custVec[0].second;
					int ubCustIndex = custVec[0].first;

					IMPLY_EXCL_ON_FAILURE(manager.getTransEnd(currentState, svtIndex, true).lq(endUBVal), Transplan::getTransActIndex(svtIndex, true));

					addToUBSet(svtIndex, ubCustIndex, true);
				}
				return TRANSPLAN::FIX;
			}
	};
}

#endif	/* SVTUPPERBOUNDINFERATOR_H */

