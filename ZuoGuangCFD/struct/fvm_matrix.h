#pragma once
#include "ZuoGuangCFD/struct/fvm_matrix_base.h"

namespace zgCFD {

	using FvmScalarMatrix = FvmMatrix<Scalar>;
	using FvmVectorMatrix = FvmMatrix<GeneralVector>;
}
