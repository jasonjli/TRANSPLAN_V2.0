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
			void addToUBSet(int rt_index)
			{
				manager.rt_UBChildren(currentState, rt_index).insert(rtIndex);
			}

			void clearUBSet()
			{
				IntSet children = manager.rt_UBChildren(currentState, rtIndex).domain;

				for (IntSet::iterator itr = children.begin(); itr != children.end(); itr++)
				{
					manager.rt_UBChildren(currentState, rtIndex).remove(*itr);
				}

			}
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
				return TRANSPLAN::FIX;
			}
	};
}
#endif	/* RTUPPERBOUNDINFERATOR_H */

