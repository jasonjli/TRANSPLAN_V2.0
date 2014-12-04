#ifndef CSPAUXINTVAR_H_
#define CSPAUXINTVAR_H_

#include "constants.h"
#include "cspvariable.h"

namespace TRANSPLAN
{

    class CSPAuxIntVar : public CSPVariable
    {
    public:
        int value;

        /*!
          Constructor
         */
        CSPAuxIntVar(int index, int value, SearchState* state);

        /*!
          Copy constructor
         */
        CSPAuxIntVar(const CSPAuxIntVar& var);

        /*!
          = operator orverloading
         */
        CSPAuxIntVar& operator=(const CSPAuxIntVar& var);

        CSPAuxIntVar* clone();

        /*!
          Assignment of a value, equal
         */
        CSP_MOD_EVENT assign(int val);

        /*!
          Returns the current domain
         */
        inline int val()
        {
            return value;
        }

    };
}
#endif
