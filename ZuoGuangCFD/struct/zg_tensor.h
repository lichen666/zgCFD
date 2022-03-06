#pragma once
#include "ZuoGuangCFD/struct/zg_scalar.h"
#include "ZuoGuangCFD/struct/zg_vector.h"
#include <iostream>

namespace zgCFD {

	template<class Type>
	class Tensor
	{
	public:
		//构造函数

		Tensor();
		Tensor(const Vector<Type>& v1, const Vector<Type>& v2, const Vector<Type>& v3);
		Tensor(const Type& xx, const Type& xy, const Type& xz, const Type& yx, const Type& yy, const Type& yz, const Type& zx, const Type& zy, const Type& zz);
		Tensor(const Tensor& tensor);

		enum components { XX, XY, XZ, YX, YY, YZ, ZX, ZY, ZZ };

		//获取相应的值

		Type& v();

		const Type& xx() const;
		const Type& xy() const;
		const Type& xz() const;
		const Type& yx() const;
		const Type& yy() const;
		const Type& yz() const;
		const Type& zx() const;
		const Type& zy() const;
		const Type& zz() const;

		Type& xx();
		Type& xy();
		Type& xz();
		Type& yx();
		Type& yy();
		Type& yz();
		Type& zx();
		Type& zy();
		Type& zz();

		//输出
		template<class Type>
		friend std::ostream& operator<<(std::ostream& output, const Tensor<Type>& t);

		//等于
		template<class Type2>
		bool operator ==(const Tensor<Type2>& t);

		Type v_[9];

	};

	#include "zg_tensorl.h"
}
