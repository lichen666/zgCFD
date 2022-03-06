#pragma once
#include "ZuoGuangCFD/struct/zg_scalar.h"
#include "ZuoGuangCFD/struct/zg_vector.h"
#include "ZuoGuangCFD/struct/zg_tensor.h"
#include "ZuoGuangCFD/struct/scalar_vector_tensor_operatorl.h"

namespace zgCFD{
	using GeneralVector = Vector<Scalar>;
	using GeneralTensor = Tensor<Scalar>;
}