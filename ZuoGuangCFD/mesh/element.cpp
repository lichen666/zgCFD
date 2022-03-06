#include "ZuoGuangCFD/mesh/element.h"

namespace zgCFD {
	element::element(){
		this->volume_ = 0.0;
	}
	element::element(const int& index) {
		this->volume_ = 0.0;
		this->index_ = index;
	}
}
