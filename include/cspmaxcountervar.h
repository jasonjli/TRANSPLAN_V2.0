#ifndef CSPMAXCOUNTERVAR_H_
#define CSPMAXCOUNTERVAR_H_

#include "constants.h"
#include "cspvariable.h"
namespace TRANSPLAN
{

    class CSPMaxCounterVar : public CSPVariable
    {
    public:
        IntSet domain;
        int max_size;
        IntSet element_set;

        /*!
          Constructor
         */
        CSPMaxCounterVar(int index, int upper_bound, IntSet element_set, SearchState* state);

        /*!
          Copy constructor
         */
        CSPMaxCounterVar(const CSPMaxCounterVar& var);

        /*!
          = operator orverloading
         */
        CSPMaxCounterVar& operator=(const CSPMaxCounterVar& var);

        CSPMaxCounterVar* clone();
        /*!
          Increament
         */
        CSP_MOD_EVENT increament(int trans_index);

        inline bool max()
        {
            return (domain.size() <= max_size);
        }

    };
}
#endif
