#pragma once
#include "3rdparty/eigen/Eigen/Sparse"
#include "3rdparty/eigen/Eigen/IterativeLinearSolvers"
#include "ZuoGuangCFD/struct/fvm_matrix.h"

namespace zgCFD {
	class Solver {

	public:
		bool Solve(FvmScalarMatrix& fsm);

	private:
		bool EigenSolve(FvmScalarMatrix& fsm);
	};
}