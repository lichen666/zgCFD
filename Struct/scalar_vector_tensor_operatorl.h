#pragma once
#include "zg_scalar.h"
#include "zg_vector.h"
#include "zg_tensor.h"

namespace zgCFD {

//向量之间运算
	//负号
	template<class Type>
	Vector<Type> operator-(const Vector<Type>& v) {

		return Vector<Type>(-v.x(), -v.y(), -v.z());
	}

	//向量相加
	template<class Type1,class Type2>
	Vector<Scalar> operator+(const Vector<Type1>& v1, const Vector<Type2>& v2) {

		return Vector<Scalar>(v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z());
	}

	//向量相减
	template<class Type1, class Type2>
	Vector<Scalar> operator-(const Vector<Type1>& v1, const Vector<Type2>& v2) {

		return Vector<Scalar>(v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z());
	}

	//点乘
	template<class Type1, class Type2>
	Scalar operator&(const Vector<Type1>& v1, const Vector<Type2>& v2) {

		return Scalar(v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z());
	}

	//叉乘
	template<class Type1, class Type2>
	Vector<Scalar> operator^(const Vector<Type1>& v1, const Vector<Type2>& v2) {

		return Vector<Scalar>(v1.y() * v2.z() - v1.z() * v2.y(), v1.z() * v2.x() - v1.x() * v2.z(), v1.x() * v2.y() - v1.y() * v2.x());
	}

//张量之间运算
	//负号
	template<class Type>
	Tensor<Type> operator-(const Tensor<Type>& t) {

		return Tensor<Type>(-t.xx(), -t.xy(), -t.xz(), -t.yx(), -t.yy(), -t.yz(), -t.zx(), -t.zy(), -t.zz());
	}

	//张量相加
	template<class Type1, class Type2>
	Tensor<Scalar> operator+(const Tensor<Type1>& t1, const Tensor<Type2>& t2) {

		return Tensor<Scalar>(t1.xx() + t2.xx(), t1.xy() + t2.xy(), t1.xz() + t2.xz(), t1.yx() + t2.yx(), t1.yy() + t2.yy(), t1.yz() + t2.yz(), t1.zx() + t2.zx(), t1.zy() + t2.zy(), t1.zz() + t2.zz());
	}

	//张量相减
	template<class Type1, class Type2>
	Tensor<Scalar> operator-(const Tensor<Type1>& t1, const Tensor<Type2>& t2) {

		return Tensor<Scalar>(t1.xx() - t2.xx(), t1.xy() - t2.xy(), t1.xz() - t2.xz(), t1.yx() - t2.yx(), t1.yy() - t2.yy(), t1.yz() - t2.yz(), t1.zx() - t2.zx(), t1.zy() - t2.zy(), t1.zz() - t2.zz());
	}

	//张量间双内积
	template<class Type1, class Type2>
	Scalar operator&&(const Tensor<Type1>& t1, const Tensor<Type2>& t2) {
		
		Scalar tmp_sum = 0.;
		for (int i = 0; i < 9; i++)
		{
			tmp_sum += t1.v_[i] * t2.v_[i];
		}

		return tmp_sum;
	}

	//张量间点积
	template<class Type1, class Type2>
	Tensor<Scalar> operator&(const Tensor<Type1>& t1, const Tensor<Type2>& t2) {

		Tensor<Scalar> tmp_tensor;
		
		tmp_tensor.xx() = t1.xx() * t2.xx() + t1.xy() * t2.yx() + t1.xz() * t2.zx();
		tmp_tensor.xy() = t1.xx() * t2.xy() + t1.xy() * t2.yy() + t1.xz() * t2.zy();
		tmp_tensor.xz() = t1.xx() * t2.xz() + t1.xy() * t2.yz() + t1.xz() * t2.zz();
		tmp_tensor.yx() = t1.yx() * t2.xx() + t1.yy() * t2.yx() + t1.yz() * t2.zx();
		tmp_tensor.yy() = t1.yx() * t2.xy() + t1.yy() * t2.yy() + t1.yz() * t2.zy();
		tmp_tensor.yz() = t1.yx() * t2.xz() + t1.yy() * t2.yz() + t1.yz() * t2.zz();
		tmp_tensor.zx() = t1.zx() * t2.xx() + t1.zy() * t2.yx() + t1.zz() * t2.zx();
		tmp_tensor.zy() = t1.zx() * t2.xy() + t1.zy() * t2.yy() + t1.zz() * t2.zy();
		tmp_tensor.zz() = t1.zx() * t2.xz() + t1.zy() * t2.yz() + t1.zz() * t2.zz();

		return tmp_tensor;
	}

//向量和标量之间的运算
	//向量乘以标量
	template<class Type>
	Vector<Scalar> operator*(const Vector<Type>& v, Scalar s) {

		return Vector<Scalar>(v.x() * s, v.y() * s, v.z() * s);
	}

	//标量乘以向量
	template<class Type>
	Vector<Scalar> operator*(Scalar s, const Vector<Type>& v) {

		return Vector<Scalar>(v.x() * s, v.y() * s, v.z() * s);
	}

	//向量除以标量
	template<class Type>
	Vector<Scalar> operator/(const Vector<Type>& v, Scalar s) {

		return Vector<Scalar>(v.x() / s, v.y() / s, v.z() / s);
	}

//张量与标量之间的运算
	//张量乘以标量
	template<class Type>
	Tensor<Scalar> operator*(const Tensor<Type>& t1, const Scalar s) {

		Tensor<Scalar> tmp_tensor;
		for (int i = 0; i < 9; i++)
		{
			tmp_tensor.v_[i] = t1.v_[i] * s;
		}

		return tmp_tensor;
	}

	//标量乘以张量
	template<class Type>
	Tensor<Scalar> operator*(const Scalar s, const Tensor<Type>& t1) {

		Tensor<Scalar> tmp_tensor;
		for (int i = 0; i < 9; i++)
		{
			tmp_tensor.v_[i] = t1.v_[i] * s;
		}

		return tmp_tensor;
	}

	//张量除以标量
	template<class Type>
	Tensor<Scalar> operator/(const Tensor<Type>& t1, const Scalar s) {

		Tensor<Scalar> tmp_tensor;
		for (int i = 0; i < 9; i++)
		{
			tmp_tensor.v_[i] = t1.v_[i] / s;
		}

		return tmp_tensor;
	}

//张量与矢量之间的运算
	//张量点乘矢量
	template<class Type1, class Type2>
	Vector<Scalar> operator&(const Tensor<Type1>& t, const Vector<Type2>& v) {
	
		return Vector<Scalar>(t.xx() * v.x() + t.xy() * v.y() + t.xz() * v.z(), t.yx() * v.x() + t.yy() * v.y() + t.yz() * v.z(), t.zx() * v.x() + t.zy() * v.y() + t.zz() * v.z());
	}
}