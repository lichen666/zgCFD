#pragma once
#include <vector>
#include "zg_scalar.h"
#include "zg_vector.h"

namespace zgCFD {
	//½Úµã
	class point
	{
	public:
		point();
		point(const Scalar& x, const Scalar& y, const Scalar& z, const int& index = -1);
		Scalar x_;
		Scalar y_;
		Scalar z_;
		int index_;
		std::vector<int> faces_;
		std::vector<int> elements_;

		Vector<Scalar> operator+(const point& p);
		Vector<Scalar> operator-(const point& p);

	};
}


