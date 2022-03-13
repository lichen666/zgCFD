#pragma once
#include <fstream>
#include <string>
#include <vector>

#include "ZuoGuangCFD/mesh/boundary.h"
#include "ZuoGuangCFD/mesh/element.h"
#include "ZuoGuangCFD/mesh/face.h"
#include "ZuoGuangCFD/mesh/point.h"

namespace zgCFD
{
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
        std::string case_directory_;

        Scalar delta_t_;
        Scalar delta_t_old_;
        Scalar delta_t_old_old;

        std::vector<point> points_;
        std::vector<face> faces_;
        std::vector<element> elements_;
        std::vector<boundary> boundaries_;

        bool isSuccessfulRead();

      protected:
        std::fstream points_file_;
        std::fstream faces_file_;
        std::fstream owner_file_;
        std::fstream neighbour_file_;
        std::fstream boundary_file_;

        bool success = false;
    };
}  // namespace zgCFD
