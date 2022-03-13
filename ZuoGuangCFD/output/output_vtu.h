#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

#include "ZuoGuangCFD/mesh/mesh.h"
#include "ZuoGuangCFD/struct/zg_field.h"
#include "ZuoGuangCFD/finite_volume_method/explicit_calculus.h"

namespace zgCFD
{
    class OutputVTU
    {
      public:

        OutputVTU(const std::string& file_name, ScalarVolumeField* p, VectorVolumeField* v, Mesh* mesh);

        void Write();

        bool WriteFile(const std::string& file_name);
        void WritePoints();
        void WriteCells();
        void WriteCellData();

        bool StartWrite(const std::string& file_name);
        bool EndWrite();
        void WriteXmlHead(const std::string& str);
        void WriteXmlEnd(const std::string& str);
        void WriteSpaceEnter(int index, int interval);

        std::ofstream out_file_;
        std::string format_;
        std::string file_name_;

        ScalarVolumeField* p_;
        VectorVolumeField *v_;

        Mesh* mesh_;
    };
}  // namespace zgCFD
