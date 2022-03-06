#pragma once
#include "ZuoGuangCFD/struct/fvm_matrix.h"
#include "ZuoGuangCFD/struct/zg_field.h"
#include "ZuoGuangCFD/finite_volume_method/explicit_calculus.h"

namespace zgCFD
{
    namespace imp
    {

        //��ɢ����ɢ
        FvmScalarMatrix Diffusion(const ScalarVolumeField& gamma, ScalarVolumeField& variable);
        FvmScalarMatrix Diffusion(const Scalar& gamma, ScalarVolumeField& variable);
        FvmVectorMatrix Diffusion(const ScalarVolumeField& gamma, VectorVolumeField& variable);
        FvmVectorMatrix Diffusion(const Scalar& gamma, VectorVolumeField& variable);

        ////��������ɢ
        FvmScalarMatrix Convection(const ScalarSurfaceField phi, ScalarVolumeField& variable);
        FvmVectorMatrix Convection(const ScalarSurfaceField phi, VectorVolumeField& variable);

        //ʱ������ɢ
        //���EULER
        FvmScalarMatrix Ddt(ScalarVolumeField& variable);
        FvmVectorMatrix Ddt(VectorVolumeField& variable);

        //Դ����ɢ
        FvmScalarMatrix Source(ScalarVolumeField& variable);
        FvmVectorMatrix Source(VectorVolumeField& variable);

    }  // namespace imp
}  // namespace zgCFD
