#pragma once

#include "ZuoGuangCFD/finite_volume_method/implicit_calculus.h"
#include "ZuoGuangCFD/solver/solver.h"
#include "ZuoGuangCFD/struct/fvm_matrix.h"
#include "ZuoGuangCFD/struct/zg_field.h"
#include "ZuoGuangCFD/output/output_vtu.h"
#include "ZuoGuangCFD/output/output_of.h"

#include<io.h>
#include <direct.h>

namespace zgCFD
{
    class Piso
    {
    public:
        Piso() {};
        Piso(const Scalar& start_time, const Scalar& delta_t, const Scalar& end_time, const Scalar& output_time,
             const uint32_t& number_of_correctors, const Scalar& nu_, VectorVolumeField* v, ScalarVolumeField* p);
        ~Piso();


        void run();

        bool IsRunnig();

        Scalar start_time_;
        Scalar end_time_;
        Scalar current_time_;
        Scalar delta_t_;
        Scalar output_time_;
        Scalar current_output_time_;

        Scalar nu_;

        // PISOÀ„∑®œ‡πÿ
        uint32_t number_of_correctors;

        ScalarVolumeField* p_;
        VectorVolumeField* v_;
    };

}  // namespace zgCFD
