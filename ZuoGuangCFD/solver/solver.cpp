#include "ZuoGuangCFD/solver/solver.h"

namespace zgCFD {
	bool Solver::Solve(FvmScalarMatrix& fsm)
	{
		return EigenSolve(fsm);
	}

	bool Solver::EigenSolve(FvmScalarMatrix& fsm)
	{
		//构造方程
		std::vector<Scalar> row;
		std::vector<Scalar> col;
		std::vector<Scalar> value;

		row.resize(fsm.matrix_.column_indices_.size(), -1);

		col.assign(fsm.matrix_.column_indices_.begin(), fsm.matrix_.column_indices_.end());
		value.assign(fsm.matrix_.values_.begin(), fsm.matrix_.values_.end());

		uint32_t num = 0;
		for (uint32_t i = 0; i < fsm.matrix_.row_offfsets_.size()-1; i++)
		{
			for (uint32_t j = 0; j < fsm.matrix_.row_offfsets_[i + 1] - fsm.matrix_.row_offfsets_[i]; j++)
			{
				row[num] = i;
				num++;
			}
		}

		std::vector< Eigen::Triplet<Scalar> > triplet_list; //三元组列表
		triplet_list.resize(row.size());

		for (uint32_t i = 0; i < row.size(); i++)
		{
			triplet_list[i] = Eigen::Triplet<Scalar>(row[i], col[i], value[i]);
		}

		Eigen::SparseMatrix <Scalar> eigen_matrix(fsm.matrix_.number_of_rows_,fsm.matrix_.number_of_columns_);

		eigen_matrix.setFromTriplets(triplet_list.begin(), triplet_list.end());

		Eigen::VectorX<Scalar> eigen_b(fsm.matrix_.number_of_columns_);
		Eigen::VectorX<Scalar> eigen_x(fsm.matrix_.number_of_columns_);

		for (uint32_t i = 0; i < fsm.sorce_.size(); i++)
		{
			eigen_b[i] = fsm.sorce_[i];
		}

		//构造求解器求解
		Eigen::BiCGSTAB <Eigen::SparseMatrix<Scalar>> bicgstab;

		bicgstab.compute(eigen_matrix);
		if (bicgstab.info() != Eigen::Success) {
			return false;
		}

		eigen_x = bicgstab.solve(eigen_b);
		if (bicgstab.info() != Eigen::Success) {
			return false;
		}

		//更新场
		fsm.field_->value_old_old_.assign(fsm.field_->value_old_.begin(), fsm.field_->value_old_.end());
		fsm.field_->value_old_.assign(fsm.field_->value_.begin(), fsm.field_->value_.end());

		for (uint32_t i = 0; i < eigen_x.size(); i++)
		{
			fsm.field_->value_[i] = eigen_x[i];
		}

		return true;
	}
}