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
			void addToLBSet(int rt_index)
			{
				manager.rt_LBChildren(currentState, svt_index).insert(svtIndex);
			}

			void clearLBSet()
			{
				IntSet children = manager.rt_LBChildren(currentState, rtIndex).domain;

				for (IntSet::iterator itr = children.begin(); itr != children.end(); itr++)
				{
					manager.rt_LBChildren(currentState, *itr).remove(rtIndex);
				}

			}
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
				return TRANSPLAN::FIX;
			}
	};
}

#endif	/* RTLOWERBOUNDINFERATOR_H */

