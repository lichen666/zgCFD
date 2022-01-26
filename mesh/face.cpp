#include "face.h"
namespace zgCFD {
	face::face() :owner_(-1), neighbour_(-1){}
	face::face(const int& index):owner_(-1),neighbour_(-1){
		this->index_ = index;
	}
}
