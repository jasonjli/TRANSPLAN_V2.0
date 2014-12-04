#ifndef CSPAUXSETMONOTONICDECVAR_H_
#define CSPAUXSETMONOTONICDECVAR_H_

#include "constants.h"
#include "cspvariable.h"

namespace TRANSPLAN
{

    class CSPAuxSetMonoDecVar : public CSPVariable
    {
    public:
        IntSet domain;
        IntStack deletedElements;

        /*!
          Constructor
         */
        CSPAuxSetMonoDecVar(int index, IntSet dom, SearchState* state);

        /*!
          Copy constructor
         */
        CSPAuxSetMonoDecVar(const CSPAuxSetMonoDecVar& var);

        /*!
          = operator orverloading
         */
        CSPAuxSetMonoDecVar& operator=(const CSPAuxSetMonoDecVar& var);

        CSPAuxSetMonoDecVar* clone();
        /*!
          Remove the value from the domain not equal
         */
        CSP_MOD_EVENT remove(int val);

        /*!
          Returns the current domain
         */
        inline IntSet dom()
        {
            return domain;
        }

        inline void deleteVal(int val)
        {
            domain.erase(val);
        }

        /*!
          Returns the size of the domain
         */
        inline int size()
        {
            return domain.size();
        }

        /*!
          Returns true if the value exits in the current domain
         */
        inline bool in(int val)
        {
            return (domain.count(val) != 0);
        }

    };
}
#endif
