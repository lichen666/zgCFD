#pragma once
#include "ZuoGuangCFD/struct/fvm_matrix.h"
#include "ZuoGuangCFD/struct/zg_field.h"
#include "ZuoGuangCFD/finite_volume_method/explicit_calculus.h"

namespace zgCFD
{
    namespace imp
    {

        //扩散项离散
        FvmScalarMatrix Diffusion(ScalarVolumeField& gamma, ScalarVolumeField& variable);
        FvmScalarMatrix Diffusion(Scalar& gamma, ScalarVolumeField& variable);
        FvmVectorMatrix Diffusion(ScalarVolumeField& gamma, VectorVolumeField& variable);
        FvmVectorMatrix Diffusion(Scalar& gamma, VectorVolumeField& variable);

        ////对流项离散
        FvmScalarMatrix Convection(ScalarSurfaceField phi, ScalarVolumeField& variable);
        FvmVectorMatrix Convection(ScalarSurfaceField phi, VectorVolumeField& variable);

        //时间项离散
        //向后EULER
        FvmScalarMatrix Ddt(ScalarVolumeField& variable);
        FvmVectorMatrix Ddt(VectorVolumeField& variable);

        //源项离散
        FvmScalarMatrix Source(ScalarVolumeField& variable);
        FvmVectorMatrix Source(VectorVolumeField& variable);

    }  // namespace imp
}  // namespace zgCFD
