#ifndef READER_H_
#define READER_H_

#include "constants.h"
#include "structures.h"

namespace TRANSPLAN
{
    class Utils;
    class Resource;
    class Activity;
    class StateVariable;
    class SVTrans;
    class RTrans;

    class Reader
    {
    public:

        Reader()
        {
        };

        //Read problem file and create state-variables, actions and transitions
        void read(std::string data_file, std::string res_file);



        // void read_preced_matrix(std::string preced_matrix_file);
    };
}
#endif
