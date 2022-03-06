#include "ZuoGuangCFD/mesh/boundary.h"

namespace zgCFD {
	boundary::boundary(){}
	boundary::boundary(const std::string& boundary_name, const std::string& type_of_boundary, const int& index, const int& start_face, const int& number_of_faces) {
		this->boundary_name_ = boundary_name;
		this->type_of_boundary_ = type_of_boundary;
		this->index_ = index;
		this->start_face_ = start_face;
		this->number_of_faces_ = number_of_faces;
	}
}