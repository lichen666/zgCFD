#include "fvm_matrix_base.h"
template<class Type>
FvmMatrix<Type>::FvmMatrix() :matrix_(0, 0)
{
}

//template<class Type>
//FvmMatrix<Type>::FvmMatrix(const FvmMatrix<Type> &rmatrix)
//{
////    this->matrix_.SetRow(rmatrix.number_of_rows_);
////    this->matrix_.SetColumn(rmatrix.number_of_columns_);
//
//    this->matrix_ = rmatrix.matrix_;
//    this->sorce_.assign(rmatrix.sorce_.begin(),rmatrix.sorce_.end());
//}

template<class Type>
inline FvmMatrix<Type>::~FvmMatrix()
{
}

template<class Type>
FvmMatrix<Type> &FvmMatrix<Type>::operator=(const FvmMatrix<Type>&rmatrix)
{
    this->matrix_ = rmatrix.matrix_;
    this->sorce_.assign(rmatrix.sorce_.begin(),rmatrix.sorce_.end());

    return *this;
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
