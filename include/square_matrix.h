#ifndef SQUARE_MATRIX_H_
#define SQUARE_MATRIX_H_

#include <vector>
#include "constants.h"

namespace TRANSPLAN
{

    class SQMatrix
    {
    private:
        int n_size;
        int default_val;
        int default_state;
        IntMatrix matrix;

    public:
        int index;

        SQMatrix(int index, int size, int def_val)
        : index(index)
        , n_size(size)
        , default_val(def_val)
        , default_state(0)
        {
            for (int i = 0; i < size; i++)
            {
                matrix.push_back(IntVector());
                for (int j = 0; j < size; j++)
                {
                    matrix[i].push_back(default_val);
                }
            }
        }

        SQMatrix(const SQMatrix& mat)
        : index(mat.index)
        , n_size(mat.n_size)
        , default_val(mat.default_val)
        , default_state(mat.default_state)
        , matrix(mat.matrix)
        {
        }

        SQMatrix& operator=(const SQMatrix& mat)
        {
            this->index = mat.index;
            this->n_size = mat.n_size;
            this->default_val = mat.default_val;
            this->default_state = mat.default_state;
            this->matrix = mat.matrix;

            return *this;
        }

        void setElementValue(int col, int row, int val)
        {
            assert(col >= 0 and col < n_size and row >= 0 and row < n_size);
            matrix[col][row] = val;
        }

        int getElementValue(int col, int row)
        {
            assert(col >= 0 and col < n_size and row >= 0 and row < n_size);

            return matrix[col][row];
        }

        void print()
        {
            for (int i = 0; i < n_size; i++)
            {
                for (int j = 0; j < n_size; j++)
                {
                    std::cout << matrix[i][j] << "\t";
                }
                std::cout << std::endl;
            }

        }
    };

}


#endif
