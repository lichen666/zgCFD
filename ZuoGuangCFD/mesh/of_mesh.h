#pragma once
#include <fstream>
#include <vector>

#include "ZuoGuangCFD/mesh/mesh.h"
#include "ZuoGuangCFD/struct/base_data_structure.h"

namespace zgCFD
{
    // OpenFOAMÍø¸ñ
    class OfMesh : public Mesh
    {
      public:
        OfMesh(const std::string& case_directory);

        bool ReadPointsFile();
        bool ReadFacesFile();
        bool ReadOwnerFile();
        bool ReadNeighbourFile();
        bool ReadBoundaryFile();
        void SetElements();
        void SetPointConnectinities();

        void ProcessOFMesh();
        void SetFaceWeightingFactor();
    };
}  // namespace zgCFD
