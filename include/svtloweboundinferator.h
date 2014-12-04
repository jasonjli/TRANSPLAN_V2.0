/* 
 * File:   svtloweboundinferator.h
 * Author: debdeep
 *
 * Created on 22 November 2014, 9:55 AM
 */

#ifndef SVTLOWEBOUNDINFERATOR_H
#define	SVTLOWEBOUNDINFERATOR_H

#include "inferator.h"

namespace TRANSPLAN
{

	class SVTLowerBoundInferator: public Inferator
	{
		private:
			void addToLBSet(int svt_index)
			{
				manager.svt_LBChildren(currentState, svt_index).insert(svtIndex);
			}

			void clearLBSet()
			{
				IntSet children = manager.svt_LBChildren(currentState, svtIndex).domain;

				for (IntSet::iterator itr = children.begin(); itr != children.end(); itr++)
				{
					manager.svt_LBChildren(currentState, svtIndex).remove(*itr);
				}

			}

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
				return TRANSPLAN::FIX;
			}
	};
}

#endif	/* SVTLOWEBOUNDINFERATOR_H */

