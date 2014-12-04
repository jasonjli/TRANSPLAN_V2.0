/*
 * cspvariable.h
 *
 *  Created on: 15/06/2009
 *      Author: banerjed
 */

#ifndef CSPVARIABLE_H
#define CSPVARIABLE_H

#include "searchstate.h"
#include "constants.h"

namespace TRANSPLAN
{

	/*This is the base class for CSP variables. This is an abstract class with virtual method clone
	 */
	class CSPVariable
	{
		public:
			int index;
			IntVector propagators;
			SearchState* state;

			/// constructor

			CSPVariable(int index, SearchState* owningState) :
				index(index),
				state(owningState)
			{
			}

			/*Copy constructor
			 */
			CSPVariable(const CSPVariable& var) :
				index(var.index),
				state(var.state),
				propagators(var.propagators)
			{

			}

			CSPVariable& operator=(const CSPVariable& var)
			{
				this->index       = var.index;
				this->state       = var.state;
				this->propagators = var.propagators;

				return *this;
			}

			/// post the propagators if any domain changes
			void postUpdate()
			{
				////post each propagators in the current search state
				for (IntVector::iterator itr = this->propagators.begin(); itr != this->propagators.end(); ++itr)
				{
					this->state->activatePropagator(*itr);
				}
			}

			///add the propagator for for this variable
			void addPropagator(int prop_idx)
			{
				propagators.push_back(prop_idx);
			}

			///clone the variable : a deep copy
			virtual CSPVariable* clone() = 0;
	};
}
#endif /* CSPVARIABLE_H_ */
