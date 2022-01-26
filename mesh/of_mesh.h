#pragma once
#include "mesh.h"
#include <fstream>
#include <vector>
#include "base_data_structure.h"

namespace zgCFD {
    //OpenFOAMÍø¸ñ
    class OfMesh :  public Mesh
    {
    public:

        OfMesh(const std::string& case_directory);

        void ReadPointsFile();
        void ReadFacesFile();
        void ReadOwnerFile();
        void ReadNeighbourFile();
        void ReadBoundaryFile();
        void SetElements();
        void SetPointConnectinities();

        void ProcessOFMesh();
        void SetFaceWeightingFactor();
    };
}

