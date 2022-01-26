using ScalarVolumeField = ScalarField<0>;
using ScalarSurfaceField = ScalarField<1>;
using VectorVolumeField = VectorField<0>;
using VectorSurfaceField = VectorField<1>;
using TensorVolumeField = TensorField<0>;
using TensorSurfaceField = TensorField<1>;

#define FIELD_OPERATOR_WITH_FIELD(return_type,type1,type2,the_operator)																	\
template<int vol_or_surface>																											\
return_type##<vol_or_surface> operator the_operator##(const type1##<vol_or_surface>& field1, const type2##<vol_or_surface>& field2) {	\
																																		\
	if (field1.value_.size() != field2.value_.size())																					\
	{																																	\
		std::cerr << "请检查两个场的大小是否一致！" << std::endl;																			\
		exit(-1);																														\
	}																																	\
																																		\
	return_type##<vol_or_surface> tmp_field("tmp_name","tmp_path");																		\
																																		\
	tmp_field.value_.resize(field1.value_.size());																						\
																																		\
	for (int i = 0; i < field1.value_.size(); i++)																						\
	{																																	\
		tmp_field.value_[i] = field1.value_[i] the_operator field2.value_[i];															\
	}																																	\
																																		\
	return tmp_field;																													\
}

FIELD_OPERATOR_WITH_FIELD(TensorField, TensorField, TensorField, +);
FIELD_OPERATOR_WITH_FIELD(TensorField, TensorField, TensorField, -);
FIELD_OPERATOR_WITH_FIELD(ScalarField, TensorField, TensorField, &&);
FIELD_OPERATOR_WITH_FIELD(TensorField, TensorField, TensorField, &);

FIELD_OPERATOR_WITH_FIELD(VectorField, TensorField, VectorField, &);

FIELD_OPERATOR_WITH_FIELD(TensorField, TensorField, ScalarField, *);
FIELD_OPERATOR_WITH_FIELD(TensorField, TensorField, ScalarField, /);

FIELD_OPERATOR_WITH_FIELD(VectorField, VectorField, VectorField, +);
FIELD_OPERATOR_WITH_FIELD(VectorField, VectorField, VectorField, -);
FIELD_OPERATOR_WITH_FIELD(ScalarField, VectorField, VectorField, &);
FIELD_OPERATOR_WITH_FIELD(VectorField, VectorField, VectorField, ^);

FIELD_OPERATOR_WITH_FIELD(VectorField, VectorField, ScalarField, *);
FIELD_OPERATOR_WITH_FIELD(VectorField, VectorField, ScalarField, /);

FIELD_OPERATOR_WITH_FIELD(TensorField, ScalarField, TensorField, *);

FIELD_OPERATOR_WITH_FIELD(VectorField, ScalarField, VectorField, *);

FIELD_OPERATOR_WITH_FIELD(ScalarField, ScalarField, ScalarField, +);
FIELD_OPERATOR_WITH_FIELD(ScalarField, ScalarField, ScalarField, -);
FIELD_OPERATOR_WITH_FIELD(ScalarField, ScalarField, ScalarField, *);
FIELD_OPERATOR_WITH_FIELD(ScalarField, ScalarField, ScalarField, /);

#define STRUCT_OPERATOR_WITH_FIELD(return_type,type1,type2,the_operator)											\
template<int vol_or_surface>																						\
return_type##<vol_or_surface> operator the_operator##(const type1##& st, const type2##<vol_or_surface>& field) {	\
																													\
	return_type##<vol_or_surface> tmp_field("tmp_name","tmp_path");													\
																													\
	tmp_field.value_.resize(field.value_.size());																	\
																													\
	for (int i = 0; i < field.value_.size(); i++)																	\
	{																												\
		tmp_field.value_[i] = st the_operator field.value_[i];														\
	}																												\
																													\
	return tmp_field;																								\
}

STRUCT_OPERATOR_WITH_FIELD(TensorField, GeneralTensor, TensorField, +);
STRUCT_OPERATOR_WITH_FIELD(TensorField, GeneralTensor, TensorField, -);
STRUCT_OPERATOR_WITH_FIELD(ScalarField, GeneralTensor, TensorField, &&);
STRUCT_OPERATOR_WITH_FIELD(TensorField, GeneralTensor, TensorField, &);

STRUCT_OPERATOR_WITH_FIELD(VectorField, GeneralTensor, VectorField, &);

STRUCT_OPERATOR_WITH_FIELD(TensorField, GeneralTensor, ScalarField, *);
STRUCT_OPERATOR_WITH_FIELD(TensorField, GeneralTensor, ScalarField, / );

STRUCT_OPERATOR_WITH_FIELD(VectorField, GeneralVector, VectorField, +);
STRUCT_OPERATOR_WITH_FIELD(VectorField, GeneralVector, VectorField, -);
STRUCT_OPERATOR_WITH_FIELD(ScalarField, GeneralVector, VectorField, &);
STRUCT_OPERATOR_WITH_FIELD(VectorField, GeneralVector, VectorField, ^);

STRUCT_OPERATOR_WITH_FIELD(VectorField, GeneralVector, ScalarField, *);
STRUCT_OPERATOR_WITH_FIELD(VectorField, GeneralVector, ScalarField, / );

STRUCT_OPERATOR_WITH_FIELD(TensorField, Scalar, TensorField, *);

STRUCT_OPERATOR_WITH_FIELD(VectorField, Scalar, VectorField, *);

STRUCT_OPERATOR_WITH_FIELD(ScalarField, Scalar, ScalarField, +);
STRUCT_OPERATOR_WITH_FIELD(ScalarField, Scalar, ScalarField, -);
STRUCT_OPERATOR_WITH_FIELD(ScalarField, Scalar, ScalarField, *);
STRUCT_OPERATOR_WITH_FIELD(ScalarField, Scalar, ScalarField, / );

#define FIELD_OPERATOR_WITH_STRUCT(return_type,type1,type2,the_operator)											\
template<int vol_or_surface>																						\
return_type##<vol_or_surface> operator the_operator##(const type1##<vol_or_surface>& field, const type2##& st) {	\
																													\
	return_type##<vol_or_surface> tmp_field("tmp_name","tmp_path");													\
																													\
	tmp_field.value_.resize(field.value_.size());																	\
																													\
	for (int i = 0; i < field.value_.size(); i++)																	\
	{																												\
		tmp_field.value_[i] = field.value_[i] the_operator st;														\
	}																												\
																													\
	return tmp_field;																								\
}

FIELD_OPERATOR_WITH_STRUCT(TensorField, TensorField, GeneralTensor, +);
FIELD_OPERATOR_WITH_STRUCT(TensorField, TensorField, GeneralTensor, -);
FIELD_OPERATOR_WITH_STRUCT(ScalarField, TensorField, GeneralTensor, &&);
FIELD_OPERATOR_WITH_STRUCT(TensorField, TensorField, GeneralTensor, &);

FIELD_OPERATOR_WITH_STRUCT(VectorField, TensorField, GeneralVector, &);

FIELD_OPERATOR_WITH_STRUCT(TensorField, TensorField, Scalar, *);
FIELD_OPERATOR_WITH_STRUCT(TensorField, TensorField, Scalar, / );

FIELD_OPERATOR_WITH_STRUCT(VectorField, VectorField, GeneralVector, +);
FIELD_OPERATOR_WITH_STRUCT(VectorField, VectorField, GeneralVector, -);
FIELD_OPERATOR_WITH_STRUCT(ScalarField, VectorField, GeneralVector, &);
FIELD_OPERATOR_WITH_STRUCT(VectorField, VectorField, GeneralVector, ^);

FIELD_OPERATOR_WITH_STRUCT(VectorField, VectorField, Scalar, *);
FIELD_OPERATOR_WITH_STRUCT(VectorField, VectorField, Scalar, / );

FIELD_OPERATOR_WITH_STRUCT(TensorField, ScalarField, GeneralTensor, *);

FIELD_OPERATOR_WITH_STRUCT(VectorField, ScalarField, GeneralVector, *);

FIELD_OPERATOR_WITH_STRUCT(ScalarField, ScalarField, Scalar, +);
FIELD_OPERATOR_WITH_STRUCT(ScalarField, ScalarField, Scalar, -);
FIELD_OPERATOR_WITH_STRUCT(ScalarField, ScalarField, Scalar, *);
FIELD_OPERATOR_WITH_STRUCT(ScalarField, ScalarField, Scalar, / );