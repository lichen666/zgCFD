#pragma once
#include <string>
#include <fstream>
#include <vector>
#include "point.h"
#include "face.h"
#include "element.h"
#include "boundary.h"

namespace zgCFD {
	class Mesh
	{
	public:
		Mesh();

		int number_of_points_;
		int number_of_owners_;
		int number_of_neighbours_;
		int number_of_boundaries_;
		int number_of_elements_;
		int number_of_faces_;
		int number_of_interior_faces_;
		int number_of_boundary_faces_;

		std::vector<point> points_;
		std::vector<face> faces_;
		std::vector<element> elements_;
		std::vector<boundary> boundaries_;

	protected:
		std::string case_directory_;

		std::fstream points_file_;
		std::fstream faces_file_;
		std::fstream owner_file_;
		std::fstream neighbour_file_;
		std::fstream boundary_file_;


	};
}

