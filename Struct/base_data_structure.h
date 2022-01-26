#pragma once
#include "zg_scalar.h"
#include "zg_vector.h"
#include "zg_tensor.h"
#include "scalar_vector_tensor_operatorl.h"

namespace zgCFD{
	using GeneralVector = Vector<Scalar>;
	using GeneralTensor = Tensor<Scalar>;
}