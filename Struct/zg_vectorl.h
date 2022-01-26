// * * * * * * * * * * * * * * * 构造函数 * * * * * * * * * * * * * * * //
template<class Type>
Vector<Type>::Vector() {
	this->v_[X] = 0;
	this->v_[Y] = 0;
	this->v_[Z] = 0;
}

template<class Type>
Vector<Type>::Vector(const Type& x, const Type& y, const Type& z) {
	this->v_[X] = x;
	this->v_[Y] = y;
	this->v_[Z] = z;
}

template<class Type>
Vector<Type>::Vector(const Vector<Type>& vec) {
	this->v_[X] = vec.v_[X];
	this->v_[Y] = vec.v_[Y];
	this->v_[Z] = vec.v_[Z];
}

// * * * * * * * * * * * * * * * 成员函数 * * * * * * * * * * * * * * * //
template<class Type>
const Type& Vector<Type>::x() const {
	return this->v_[X];
}

template<class Type>
const Type& Vector<Type>::y() const {
	return this->v_[Y];
}

template<class Type>
const Type& Vector<Type>::z() const {
	return this->v_[Z];
}

template<class Type>
Type& Vector<Type>::x() {
	return this->v_[X];
}

template<class Type>
Type& Vector<Type>::y() {
	return this->v_[Y];
}

template<class Type>
Type& Vector<Type>::z() {
	return this->v_[Z];
}

template<class Type>
Scalar Vector<Type>::Magnitude() {
	return sqrt(pow(v_[X], 2) + pow(v_[Y], 2) + pow(v_[Z], 2));
}

template<class Type>
std::ostream& operator<<(std::ostream& output, Vector<Type>& v){

	output << v.x() << " " << v.y() << " " << v.z() << std::endl;

	return output;
}

//等于
template<class Type>
template<class Type2>
bool Vector<Type>::operator ==(const Vector<Type2>& v) {

	for (int i = 0; i < 3; i++)
	{
		if (abs(this->v_[i] - v.v_[i]) > 0.00001) return false;
	}

	return true;
}

//输出
template<class Type>
std::ostream& operator<<(std::ostream& output, const Vector<Type>& v) {

	output << v.x() << " " << v.y() << " " << v.z();

	return output;
}