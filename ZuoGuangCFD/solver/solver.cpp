#include "ZuoGuangCFD/solver/solver.h"

namespace zgCFD {
	bool Solver::Solve(FvmScalarMatrix& fsm)
	{
		return EigenSolve(fsm);
	}

	bool Solver::Solve(FvmVectorMatrix& fvm)
	{
		return EigenSolve(fvm);
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
		fsm.field_->interior_value_old_old_.assign(fsm.field_->interior_value_old_.begin(), fsm.field_->interior_value_old_.end());
		fsm.field_->interior_value_old_.assign(fsm.field_->interior_value_.begin(), fsm.field_->interior_value_.end());

		for (uint32_t i = 0; i < eigen_x.size(); i++)
		{
			fsm.field_->interior_value_[i] = eigen_x[i];
		}

		return true;
	}
	bool Solver::EigenSolve(FvmVectorMatrix& fvm)
	{
		//构造方程
		std::vector<Scalar> row;
		std::vector<Scalar> col;
		std::vector<Scalar> value;

		row.resize(fvm.matrix_.column_indices_.size(), -1);

		col.assign(fvm.matrix_.column_indices_.begin(), fvm.matrix_.column_indices_.end());
		value.assign(fvm.matrix_.values_.begin(), fvm.matrix_.values_.end());

		uint32_t num = 0;
		for (uint32_t i = 0; i < fvm.matrix_.row_offfsets_.size() - 1; i++)
		{
			for (uint32_t j = 0; j < fvm.matrix_.row_offfsets_[i + 1] - fvm.matrix_.row_offfsets_[i]; j++)
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

		Eigen::SparseMatrix <Scalar> eigen_matrix(fvm.matrix_.number_of_rows_, fvm.matrix_.number_of_columns_);

		eigen_matrix.setFromTriplets(triplet_list.begin(), triplet_list.end());

		Eigen::VectorX<Scalar> eigen_b_x(fvm.matrix_.number_of_columns_);
		Eigen::VectorX<Scalar> eigen_x_x(fvm.matrix_.number_of_columns_);
		Eigen::VectorX<Scalar> eigen_b_y(fvm.matrix_.number_of_columns_);
		Eigen::VectorX<Scalar> eigen_x_y(fvm.matrix_.number_of_columns_);
		Eigen::VectorX<Scalar> eigen_b_z(fvm.matrix_.number_of_columns_);
		Eigen::VectorX<Scalar> eigen_x_z(fvm.matrix_.number_of_columns_);

		for (uint32_t i = 0; i < fvm.sorce_.size(); i++)
		{
			eigen_b_x[i] = fvm.sorce_[i].x();
			eigen_b_y[i] = fvm.sorce_[i].y();
			eigen_b_z[i] = fvm.sorce_[i].z();
		}

		//构造求解器求解
		Eigen::BiCGSTAB <Eigen::SparseMatrix<Scalar>> bicgstab;

		bicgstab.compute(eigen_matrix);
		if (bicgstab.info() != Eigen::Success) {
			return false;
		}

		eigen_x_x = bicgstab.solve(eigen_b_x);
		if (bicgstab.info() != Eigen::Success) {
			return false;
		}
		eigen_x_y = bicgstab.solve(eigen_b_y);
		if (bicgstab.info() != Eigen::Success) {
			return false;
		}
		eigen_x_z = bicgstab.solve(eigen_b_z);
		if (bicgstab.info() != Eigen::Success) {
			return false;
		}

		/*Eigen::SparseLU <Eigen::SparseMatrix<Scalar>> lu;

		lu.compute(eigen_matrix);
		if (lu.info() != Eigen::Success) {
			return false;
		}

		eigen_x_x = lu.solve(eigen_b_x);
		if (lu.info() != Eigen::Success) {
			return false;
		}
		eigen_x_y = lu.solve(eigen_b_y);
		if (lu.info() != Eigen::Success) {
			return false;
		}
		eigen_x_z = lu.solve(eigen_b_z);
		if (lu.info() != Eigen::Success) {
			return false;
		}*/

		//更新场
		fvm.field_->interior_value_old_old_.assign(fvm.field_->interior_value_old_.begin(), fvm.field_->interior_value_old_.end());
		fvm.field_->interior_value_old_.assign(fvm.field_->interior_value_.begin(), fvm.field_->interior_value_.end());

		for (uint32_t i = 0; i < eigen_x_x.size(); i++)
		{
			fvm.field_->interior_value_[i].x() = eigen_x_x[i];
			fvm.field_->interior_value_[i].y() = eigen_x_y[i];
			fvm.field_->interior_value_[i].z() = eigen_x_z[i];
		}
		return true;
	}
}