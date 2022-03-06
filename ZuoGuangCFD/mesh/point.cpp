#include "ZuoGuangCFD/mesh/point.h"

namespace zgCFD {
	point::point() :index_(-1) {}
	point::point(const Scalar& x, const Scalar& y, const Scalar& z, const int& index) {
		this->x_ = x;
		this->y_ = y;
		this->z_ = z;
		this->index_ = index;
	}

	Vector<Scalar> point::operator+(const point& p){
		Vector<Scalar> tmp_vec_;
		tmp_vec_.x() = this->x_ + p.x_;
		tmp_vec_.y() = this->y_ + p.y_;
		tmp_vec_.z() = this->z_ + p.z_;
		return tmp_vec_;
	}

	Vector<Scalar> point::operator-(const point& p) {
		Vector<Scalar> tmp_vec_;
		tmp_vec_.x() = this->x_ - p.x_;
		tmp_vec_.y() = this->y_ - p.y_;
		tmp_vec_.z() = this->z_ - p.z_;
		return tmp_vec_;
	}

}