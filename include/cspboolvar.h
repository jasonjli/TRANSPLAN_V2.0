#ifndef CSPBOOLVAR_H_
#define CSPBOOLVAR_H_

#include "constants.h"
#include "cspvariable.h"

namespace TRANSPLAN
{

    class CSPBoolVar : public CSPVariable
    {
    	private:
    	int value;
    public:
        /*!
          Constructor
         */
        CSPBoolVar(int index, SearchState* state);

        /*!
          Copy constructor
         */
        CSPBoolVar(const CSPBoolVar& var);

        /*!
          = operator orverloading
         */
        CSPBoolVar& operator=(const CSPBoolVar& var);

        CSPBoolVar* clone();

        CSP_MOD_EVENT eqTrue();

        CSP_MOD_EVENT eqFalse();

        inline bool isFalse()
        {
            return (this->value == 0);
        }

        inline bool isTrue()
        {
            return (this->value == 1);
        }

        inline bool assigned()
        {
            return (this->value != TRANSPLAN::CSP_NOT_ASSIGNED);
        }

        inline void unassign()
        {
            this->value = TRANSPLAN::CSP_NOT_ASSIGNED;
        }


    };
}
#endif
