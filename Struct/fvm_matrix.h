#pragma once
#include <vector>
#include "matrix_csr.h"
#include "zg_scalar.h"

namespace zgCFD {

	class FvmMatrix
	{
	public:
		

		FvmMatrix();


		//ϵ������A
		CsrMatrix matrix_;
		//Դ��b
		std::vector<Scalar> sorce_;
		
		~FvmMatrix();

	private:

	};
}


