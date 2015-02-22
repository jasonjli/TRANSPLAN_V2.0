#ifndef INFERATOR_H
#define INFERATOR_H

#include <iostream>
#include <cstdlib>
#include "constants.h"
#include <ctime>

#include "cspintvar.h"
#include "cspintervalvar.h"
#include "cspboolvar.h"
#include "cspauxsetvar.h"
#include "cspauxsetmonotonicincvar.h"
#include "cspauxsetmonotonicdecvar.h"
#include "cspauxintvar.h"
#include "cspmaxcountervar.h"
#include "cspgraphvar.h"

#include "variablepropagatormanager.h"

namespace TRANSPLAN
{

	class SearchState;
	class VariablePropagatorManager;

	class Inferator
	{

		public:
			SearchState* currentState;
			int inferIndex;
			VariablePropagatorManager& manager;

			Inferator(int inferIndex, VariablePropagatorManager& manager) :
				inferIndex(inferIndex), manager(manager)
			{
				//do nothing
			}

			Inferator(const Inferator& infer) :
				inferIndex(infer.inferIndex), manager(infer.manager)
			{
				//do nothing
			}

			virtual PROP_STATUS infer(SearchState* state)=0;

			void addToLBSet(int ownerIndex, int tIndex, bool isSVT)
			{
				manager.getTransLB(currentState, ownerIndex, isSVT).insert(tIndex);
				manager.getTransLBChildren(currentState, tIndex, isSVT).insert(ownerIndex);
			}

			void deleteFromLBSet(int ownerIndex, int tIndex, bool isSVT)
			{
				manager.getTransLB(currentState, ownerIndex, isSVT).remove(tIndex);
				manager.getTransLBChildren(currentState, tIndex, isSVT).remove(ownerIndex);
			}

			void clearLBSet(int ownerIndex, bool isSVT)
			{
				IntSet children = manager.getTransLB(currentState, ownerIndex, isSVT).domain;

				for (IntSet::iterator itr = children.begin(); itr != children.end(); itr++)
				{
					deleteFromLBSet(ownerIndex, *itr, isSVT);
				}

			}

			void addToUBSet(int ownerIndex, int tIndex, bool isSVT)
			{
				manager.getTransUB(currentState, ownerIndex, isSVT).insert(tIndex);
				manager.getTransUBChildren(currentState, tIndex, isSVT).insert(ownerIndex);
			}

			void deleteFromUBSet(int ownerIndex, int tIndex, bool isSVT)
			{
				manager.getTransUB(currentState, ownerIndex, isSVT).remove(tIndex);
				manager.getTransUBChildren(currentState, tIndex, isSVT).remove(ownerIndex);
			}

			void clearUBSet(int ownerIndex, bool isSVT)
			{
				IntSet children = manager.getTransUB(currentState, ownerIndex, isSVT).domain;

				for (IntSet::iterator itr = children.begin(); itr != children.end(); itr++)
				{
					deleteFromUBSet(ownerIndex, *itr, isSVT);
				}

			}

	};
}
#endif
