#ifndef PROPAGATOR_H
#define PROPAGATOR_H

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
#include "transplan.h"
#include "variablepropagatormanager.h"

namespace TRANSPLAN
{

#define RETURN_IF_FAILURE(csp_mod_event)	\
  {						\
    if (csp_mod_event == TRANSPLAN::FAILURE)	\
      return TRANSPLAN::FAIL;			\
  }

#define RETURN_IF_FAIL(prop_event)     \
  {				       \
    if (prop_event == TRANSPLAN::FAIL) \
      return TRANSPLAN::FAIL;	       \
  }

#define IMPLY_EXCL_ON_FAILURE(csp_mod_event, act_index)		\
  {								\
    if (csp_mod_event == TRANSPLAN::FAILURE)			\
      {								\
	RETURN_IF_FAILURE(manager.a_inplan(currentState, act_index).eqFalse());		\
      }								\
  }

	class SearchState;
	class Transition;
	class Transplan;
	class VariablePropagatorManager;

	class Propagator
	{
		public:
			SearchState* currentState;
			int propIndex;
			VariablePropagatorManager& manager;

			Propagator(int p_idx, VariablePropagatorManager& manager) :
				propIndex(p_idx), manager(manager)
			{
				//do nothing
			}

			Propagator(const Propagator& prop) :
				propIndex(prop.propIndex), manager(prop.manager)
			{
				//do nothing
			}

			virtual PROP_STATUS propagate(SearchState* state)=0;

			/*
			 * PropagatorHelperMethod Post(T-->T'):
			 1) Add T' in MustBeAfter(T)
			 2) Add T in MustBeBefore(T')
			 3) Post T'--X-->T
			 4) ActionDistance(a(T), a(T')) >= offset(T) + duration(T) - offset(T')
			 */
			PROP_STATUS popagateTransitionPrecedence(int fromIndex, int toIndex, bool isSVT)
			{
				RETURN_IF_FAILURE(manager.getTransMustAfter(currentState, fromIndex, isSVT).insert(toIndex));
				RETURN_IF_FAILURE(manager.getTransMustBefore(currentState, toIndex, isSVT).insert(fromIndex));

				int distanceLB = Transplan::getTransOffset(fromIndex, isSVT) + Transplan::getTransDuration(fromIndex, isSVT) - Transplan::getTransOffset(toIndex, isSVT);
				int fromAct = Transplan::getTransActIndex(fromIndex, isSVT);
				int toAct = Transplan::getTransActIndex(toIndex, isSVT);

				RETURN_IF_FAILURE(manager.a_dist(currentState).updateEdge(fromAct, toAct, distanceLB));

				RETURN_IF_FAIL(popagateTransitionAntiPrecedence(fromIndex, toIndex, isSVT));

				return TRANSPLAN::FIX;
			}

			/*
			 * PropagatorHelperMethod Post(T--X-->T');
			 1) Add T in MustNOTBefore(T')
			 2) Add T' in MustNotAfter(T)
			 3)ActionDistance(a(T'), a(T)) >= - offset(T) - duration(T) + offset(T') + 1
			 */
			PROP_STATUS popagateTransitionAntiPrecedence(int fromIndex, int toIndex, bool isSVT)
			{
				RETURN_IF_FAILURE(manager.getTransMustNOTAfter(currentState, fromIndex, isSVT).insert(toIndex));
				RETURN_IF_FAILURE(manager.getTransMustNOTBefore(currentState, toIndex, isSVT).insert(fromIndex));

				int distanceLB = Transplan::getTransOffset(toIndex, isSVT) - Transplan::getTransOffset(fromIndex, isSVT) - Transplan::getTransDuration(fromIndex, isSVT) + 1;
				int fromAct = Transplan::getTransActIndex(fromIndex, isSVT);
				int toAct = Transplan::getTransActIndex(toIndex, isSVT);

				RETURN_IF_FAILURE(manager.a_dist(currentState).updateEdge(toAct, fromAct, distanceLB));

				return TRANSPLAN::FIX;
			}

			bool hasPrecendenceConstraint(int fromIndex, int toIndex, bool isSVT)
			{
				return manager.getTransMustAfter(currentState, fromIndex, isSVT).dom().count(toIndex) > 0;
			}

	};
}
#endif
