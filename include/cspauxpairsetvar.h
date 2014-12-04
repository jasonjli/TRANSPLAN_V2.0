#ifndef CSPAUXPAIRSETVAR_H_
#define CSPAUXPAIRSETVAR_H_

#include "constants.h"
#include "cspvariable.h"

namespace TRANSPLAN
{

    class CSPAuxPairSetVar : public CSPVariable
    {
    public:
        IntPairVector set;
        std::stack<IntPair> addedElements;

        /*!
          Constructor
         */
        CSPAuxPairSetVar(int index, SearchState* state);

        /*!
          Copy constructor
         */
        CSPAuxPairSetVar(const CSPAuxPairSetVar& var);

        /*!
          = operator orverloading
         */
        CSPAuxPairSetVar& operator=(const CSPAuxPairSetVar& var);

        CSPAuxPairSetVar* clone();
        /*!
          Assignment of a value, equal
         */
        CSP_MOD_EVENT insert(IntPair val);

        /*!
          Returns the current domain
         */
        inline IntPairVector get_set()
        {
            return set;
        }

        inline bool in(IntPair val)
        {
            for (int i = 0; i < set.size(); ++i)
            {
                if (val.first == set[i].first && val.second == set[i].second)
                {
                    return true;
                }
            }
            return false;
        }

    };
}
#endif
