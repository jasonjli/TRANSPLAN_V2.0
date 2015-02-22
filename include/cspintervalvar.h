#ifndef CSPINTERVALVAR_H_
#define CSPINTERVALVAR_H_

#include "constants.h"
#include "cspvariable.h"

namespace TRANSPLAN
{

	class CSPIntervalVar: public CSPVariable
	{
		private:
			IntPair domain;
		public:
			//first = lower bound, second = upper bound

			bool isMaxUpdate;
			bool isMinUpdate;

			/*!
			 Constructor
			 */
			CSPIntervalVar(int index, IntPair dom, SearchState* state);

			/*!
			 Copy constructor
			 */
			CSPIntervalVar(const CSPIntervalVar& var);

			/*!
			 = operator orverloading
			 */
			CSPIntervalVar& operator=(const CSPIntervalVar& var);

			/*!
			 * < operator overloading based on lower bound values
			 */

			CSPIntervalVar* clone();

			/*!
			 Assign to a fixed value
			 */
			CSP_MOD_EVENT eq(int val);

			/*!
			 Less than equal
			 */
			CSP_MOD_EVENT lq(int val);

			/*!
			 Greater than equal
			 */
			CSP_MOD_EVENT gq(int val);

			/*!
			 If the interval has fixed value
			 */
			inline bool assigned()
			{
				return (domain.first == domain.second);
			}

			/*!
			 Lower bound of the interval
			 */
			inline int min()
			{
				return domain.first;
			}

			/*!
			 Upper bound of the interval
			 */
			inline int max()
			{
				return domain.second;
			}

			inline IntPair dom()
			{
				return domain;
			}
	};

}

#endif
