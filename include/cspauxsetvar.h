#ifndef CSPAUXSETVAR_H_
#define CSPAUXSETVAR_H_

#include "constants.h"
#include "cspvariable.h"
namespace TRANSPLAN
{

    class CSPAuxSetVar : public CSPVariable
    {
    public:
        IntSet domain;

        /*!
          Constructor
         */
        CSPAuxSetVar(int index, IntSet dom, SearchState* state);

        /*!
          Copy constructor
         */
        CSPAuxSetVar(const CSPAuxSetVar& var);

        /*!
          = operator orverloading
         */
        CSPAuxSetVar& operator=(const CSPAuxSetVar& var);

        CSPAuxSetVar* clone();
        /*!
          Insert a value in the domain
         */
        CSP_MOD_EVENT insert(int val);

        /*!
          Remove the value from the domain not equal
         */
        CSP_MOD_EVENT remove(int val);

        /*!
          Remove all value from the domain
         */
        CSP_MOD_EVENT clear();

        /*!
          Returns the current domain
         */
        inline IntSet dom()
        {
            return domain;
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
