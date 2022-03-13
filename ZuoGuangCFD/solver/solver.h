#pragma once
#include "3rdparty/eigen/Eigen/Sparse"
#include "3rdparty/eigen/Eigen/IterativeLinearSolvers"
#include "3rdparty/eigen/Eigen/SparseLU"
#include "ZuoGuangCFD/struct/fvm_matrix.h"

namespace zgCFD {
	class Solver {

	public:
		bool Solve(FvmScalarMatrix& fsm);
		bool Solve(FvmVectorMatrix& fvm);

	private:
		bool EigenSolve(FvmScalarMatrix& fsm);
		bool EigenSolve(FvmVectorMatrix& fvm);
	};
}