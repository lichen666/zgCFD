#include "zg_tensor.h"
template<class Type>
Type& Tensor<Type>::v() {
	return v_;
}

template<class Type>
Tensor<Type>::Tensor() {
	this->v_[XX] = 0;
	this->v_[XY] = 0;
	this->v_[XZ] = 0;
	this->v_[YX] = 0;
	this->v_[YY] = 0;
	this->v_[YZ] = 0;
	this->v_[ZX] = 0;
	this->v_[ZY] = 0;
	this->v_[ZZ] = 0;
}

template<class Type>
Tensor<Type>::Tensor(const Vector<Type>& v1, const Vector<Type>& v2, const Vector<Type>& v3)
{
	this->v_[XX] = v1.x();
	this->v_[XY] = v1.y();
	this->v_[XZ] = v1.z();
	this->v_[YX] = v2.x();
	this->v_[YY] = v2.y();
	this->v_[YZ] = v2.z();
	this->v_[ZX] = v3.x();
	this->v_[ZY] = v3.y();
	this->v_[ZZ] = v3.z();
}


template<class Type>
Tensor<Type>::Tensor(const Type& xx, const Type& xy, const Type& xz, const Type& yx, const Type& yy, const Type& yz, const Type& zx, const Type& zy, const Type& zz) {
	this->v_[XX] = xx;
	this->v_[XY] = xy;
	this->v_[XZ] = xz;
	this->v_[YX] = yx;
	this->v_[YY] = yy;
	this->v_[YZ] = yz;
	this->v_[ZX] = zx;
	this->v_[ZY] = zy;
	this->v_[ZZ] = zz;
}

template<class Type>
Tensor<Type>::Tensor(const Tensor& tensor) {
	this->v_[XX] = tensor.xx();
	this->v_[XY] = tensor.xy();
	this->v_[XZ] = tensor.xz();
	this->v_[YX] = tensor.yx();
	this->v_[YY] = tensor.yy();
	this->v_[YZ] = tensor.yz();
	this->v_[ZX] = tensor.zx();
	this->v_[ZY] = tensor.zy();
	this->v_[ZZ] = tensor.zz();
}

//获取相应的值
template<class Type>
const Type& Tensor<Type>::xx() const {
	return this->v_[XX];
}

template<class Type>
const Type& Tensor<Type>::xy() const {
	return this->v_[XY];
}

template<class Type>
const Type& Tensor<Type>::xz() const {
	return this->v_[XZ];
}

template<class Type>
const Type& Tensor<Type>::yx() const {
	return this->v_[YX];
}

template<class Type>
const Type& Tensor<Type>::yy() const {
	return this->v_[YY];
}

template<class Type>
const Type& Tensor<Type>::yz() const {
	return this->v_[YZ];
}

template<class Type>
const Type& Tensor<Type>::zx() const {
	return this->v_[ZX];
}

template<class Type>
const Type& Tensor<Type>::zy() const {
	return this->v_[ZY];
}

template<class Type>
const Type& Tensor<Type>::zz() const {
	return this->v_[ZZ];
}

template<class Type>
Type& Tensor<Type>::xx() {
	return this->v_[XX];
}

template<class Type>
Type& Tensor<Type>::xy() {
	return this->v_[XY];
}

template<class Type>
Type& Tensor<Type>::xz() {
	return this->v_[XZ];
}

template<class Type>
Type& Tensor<Type>::yx() {
	return this->v_[YX];
}

template<class Type>
Type& Tensor<Type>::yy() {
	return this->v_[YY];
}

template<class Type>
Type& Tensor<Type>::yz() {
	return this->v_[YZ];
}

template<class Type>
Type& Tensor<Type>::zx() {
	return this->v_[ZX];
}

template<class Type>
Type& Tensor<Type>::zy() {
	return this->v_[ZY];
}

template<class Type>
Type& Tensor<Type>::zz() {
	return this->v_[ZZ];
}

//输出
template<class Type>
std::ostream& operator<<(std::ostream& output, const Tensor<Type>& t) {
	output << t.xx() << " " << t.xy() << " " << t.xz() << std::endl;
	output << t.yx() << " " << t.yy() << " " << t.yz() << std::endl;
	output << t.zx() << " " << t.zy() << " " << t.zz();

	return output;
}

//等于
template<class Type>
template<class Type2>
bool Tensor<Type>::operator ==(const Tensor<Type2>& t) {

	if (abs(this->xx() - t.xx()) > 0.00001) return false;
	if (abs(this->xy() - t.xy()) > 0.00001) return false;
	if (abs(this->xz() - t.xz()) > 0.00001) return false;
	if (abs(this->yx() - t.yx()) > 0.00001) return false;
	if (abs(this->yy() - t.yy()) > 0.00001) return false;
	if (abs(this->yz() - t.yz()) > 0.00001) return false;
	if (abs(this->zx() - t.zx()) > 0.00001) return false;
	if (abs(this->zy() - t.zy()) > 0.00001) return false;
	if (abs(this->zz() - t.zz()) > 0.00001) return false;

	return true;
}

