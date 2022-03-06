#pragma once
#include <vector>

#include "ZuoGuangCFD/struct/matrix_csr.h"
#include "ZuoGuangCFD/struct/base_data_structure.h"
#include "ZuoGuangCFD/struct/zg_field.h"

namespace zgCFD
{
    template <class Type>
    class FvmMatrix
    {
      public:
        FvmMatrix();
        ~FvmMatrix();

        void SetVarible(BaseField<Type, 0> * var);

        FvmMatrix operator+(const FvmMatrix& rmatrix);
        FvmMatrix operator-(const FvmMatrix& rmatrix);

        //系数矩阵A
        CsrMatrix matrix_;
        //源项b
        std::vector<Type> sorce_;

        //场
        BaseField<Type, 0>* field_;

      private:
    };

#include "ZuoGuangCFD/struct/fvm_matrix_basel.h"


}  // namespace zgCFD
