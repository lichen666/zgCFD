#pragma once
#include <vector>
#include "matrix_csr.h"
#include "zg_scalar.h"

namespace zgCFD {

	class FvmMatrix
	{
	public:
		

		FvmMatrix();


		//系数矩阵A
		CsrMatrix matrix_;
		//源项b
		std::vector<Scalar> sorce_;
		
		~FvmMatrix();

	private:

	};
}


