#pragma once
#include <string>

#include "ZuoGuangCFD/struct/enums.h"

namespace zgCFD
{
    class boundary
    {
      public:
        boundary();
        boundary(const std::string& boundary_name, const std::string& type, const int& index, const int& start_face,
                 const int& number_of_faces);
        std::string boundary_name_;
        std::string type_of_boundary_;
        int index_;
        int start_face_;
        int number_of_faces_;
    };
}  // namespace zgCFD
