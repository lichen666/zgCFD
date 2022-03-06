#pragma once
#include "ZuoGuangCFD/struct/fvm_matrix.h"
#include "ZuoGuangCFD/struct/zg_field.h"
#include "ZuoGuangCFD/finite_volume_method/explicit_calculus.h"

namespace zgCFD
{
    namespace imp
    {

        //扩散项离散
        FvmScalarMatrix Diffusion(const ScalarVolumeField& gamma, ScalarVolumeField& variable);
        FvmScalarMatrix Diffusion(const Scalar& gamma, ScalarVolumeField& variable);
        FvmVectorMatrix Diffusion(const ScalarVolumeField& gamma, VectorVolumeField& variable);
        FvmVectorMatrix Diffusion(const Scalar& gamma, VectorVolumeField& variable);

        ////对流项离散
        FvmScalarMatrix Convection(const ScalarSurfaceField phi, ScalarVolumeField& variable);
        FvmVectorMatrix Convection(const ScalarSurfaceField phi, VectorVolumeField& variable);

        //时间项离散
        //向后EULER
        FvmScalarMatrix Ddt(ScalarVolumeField& variable);
        FvmVectorMatrix Ddt(VectorVolumeField& variable);

        //源项离散
        FvmScalarMatrix Source(ScalarVolumeField& variable);
        FvmVectorMatrix Source(VectorVolumeField& variable);

    }  // namespace imp
}  // namespace zgCFD
