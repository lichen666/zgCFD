#pragma once
#include "ZuoGuangCFD/struct/fvm_matrix.h"
#include "ZuoGuangCFD/struct/zg_field.h"
#include "ZuoGuangCFD/finite_volume_method/explicit_calculus.h"

namespace zgCFD
{
    namespace imp
    {

        //��ɢ����ɢ
        FvmScalarMatrix Diffusion(ScalarVolumeField& gamma, ScalarVolumeField& variable);
        FvmScalarMatrix Diffusion(Scalar& gamma, ScalarVolumeField& variable);
        FvmVectorMatrix Diffusion(ScalarVolumeField& gamma, VectorVolumeField& variable);
        FvmVectorMatrix Diffusion(Scalar& gamma, VectorVolumeField& variable);

        ////��������ɢ
        FvmScalarMatrix Convection(ScalarSurfaceField phi, ScalarVolumeField& variable);
        FvmVectorMatrix Convection(ScalarSurfaceField phi, VectorVolumeField& variable);

        //ʱ������ɢ
        //���EULER
        FvmScalarMatrix Ddt(ScalarVolumeField& variable);
        FvmVectorMatrix Ddt(VectorVolumeField& variable);

        //Դ����ɢ
        FvmScalarMatrix Source(ScalarVolumeField& variable);
        FvmVectorMatrix Source(VectorVolumeField& variable);

    }  // namespace imp
}  // namespace zgCFD
