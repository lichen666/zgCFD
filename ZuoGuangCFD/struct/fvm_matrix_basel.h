#include "fvm_matrix_base.h"
template<class Type>
FvmMatrix<Type>::FvmMatrix() :matrix_(0, 0)
{
}

template<class Type>
inline FvmMatrix<Type>::~FvmMatrix()
{
}

template<class Type>
inline void zgCFD::FvmMatrix<Type>::SetVarible(BaseField<Type, 0>* var)
{
    field_ = var;
}

template<class Type>
FvmMatrix<Type> FvmMatrix<Type>::operator+(const FvmMatrix<Type>& rmatrix)
{
    FvmMatrix<Type> tmp_matrix;

    tmp_matrix.matrix_ = this->matrix_ + rmatrix.matrix_;
    tmp_matrix.sorce_.resize(rmatrix.sorce_.size());

    for (uint32_t i = 0; i < tmp_matrix.sorce_.size(); i++)
    {
        tmp_matrix.sorce_[i] = this->sorce_[i] + rmatrix.sorce_[i];
    }

    return tmp_matrix;
}

template<class Type>
FvmMatrix<Type> FvmMatrix<Type>::operator-(const FvmMatrix<Type>& rmatrix)
{
    FvmMatrix<Type> tmp_matrix;

    tmp_matrix.matrix_ = this->matrix_ - rmatrix.matrix_;
    tmp_matrix.sorce_.resize(rmatrix.sorce_.size());

    for (uint32_t i = 0; i < tmp_matrix.sorce_.size(); i++)
    {
        tmp_matrix.sorce_[i] = this->sorce_[i] - rmatrix.sorce_[i];
    }

    return tmp_matrix;
}
