#ifndef CSPAUXSETMONOTONICINCVAR_H_
#define CSPAUXSETMONOTONICINCVAR_H_

#include "constants.h"
#include "cspvariable.h"

namespace TRANSPLAN
{

    class CSPAuxSetMonoIncVar : public CSPVariable
    {
    public:
        IntSet domain;
        IntStack addedElements;

        /*!
          Constructor
         */
        CSPAuxSetMonoIncVar(int index, IntSet dom, SearchState* state);

        /*!
          Copy constructor
         */
        CSPAuxSetMonoIncVar(const CSPAuxSetMonoIncVar& var);

        /*!
          = operator orverloading
         */
        CSPAuxSetMonoIncVar& operator=(const CSPAuxSetMonoIncVar& var);

        CSPAuxSetMonoIncVar* clone();
        /*!
          Insert a value in the domain
         */
        CSP_MOD_EVENT insert(int val);

        /*!
          Returns the current domain
         */
        inline IntSet dom()
        {
            return domain;
        }

        inline void add(int val)
        {
            domain.insert(val);
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
