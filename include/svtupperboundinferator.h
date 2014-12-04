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
			void addToUBSet(int svt_index)
			{
				manager.svt_UBChildren(currentState, svt_index).insert(svtIndex);
			}

			void clearUBSet()
			{
				IntSet children = manager.svt_UBChildren(currentState, svtIndex).domain;

				for (IntSet::iterator itr = children.begin(); itr != children.end(); itr++)
				{
					manager.svt_UBChildren(currentState, svtIndex).remove(*itr);
				}

			}
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
				return TRANSPLAN::FIX;
			}
	};
}

#endif	/* SVTUPPERBOUNDINFERATOR_H */

