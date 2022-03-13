#pragma once

#include <fstream>
#include <iostream>
#include <sstream>

#include "ZuoGuangCFD/mesh/mesh.h"
#include "ZuoGuangCFD/struct/zg_field.h"
#include "ZuoGuangCFD/finite_volume_method/explicit_calculus.h"

namespace zgCFD {
	class OutPutOf
	{
    public:

        OutPutOf(const std::string& file_path, const std::string& file_time, ScalarVolumeField* p, VectorVolumeField* v, Mesh* mesh);

        void Write();

        bool WriteFile(const std::string& file_name);
        void WriteHead(const std::string& str);

        void WriteP();
        void WriteV();

        void WritePBoundary();
        void WriteVBoundary();

        std::ofstream out_file_p_;
        std::ofstream out_file_v_;
        std::string file_path_;
        std::string file_time_;

        ScalarVolumeField* p_;
        VectorVolumeField* v_;

        Mesh* mesh_;

        std::string p_head;

        std::string v_head;
	};
}

