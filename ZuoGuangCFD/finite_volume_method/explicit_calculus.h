#pragma once
#include "ZuoGuangCFD/struct/enums.h"
#include "ZuoGuangCFD/struct/zg_field.h"

namespace zgCFD
{
    namespace exp
    {
        //��ֵ

        ScalarSurfaceField Interpolate(ScalarVolumeField& svfield, TheInterpolationScheme scheme);
        VectorSurfaceField Interpolate(VectorVolumeField& vvfield, TheInterpolationScheme scheme);

        VectorSurfaceField InterpolateGradient(VectorVolumeField& grad_field,ScalarVolumeField& svfield,
                                               TheInterpolationScheme scheme);
        TensorSurfaceField InterpolateGradient(TensorVolumeField& grad_field,VectorVolumeField& vvfield,
            TheInterpolationScheme scheme);
        //�ݶ�
        //�������ݶ�
        VectorVolumeField ComputeGradient(ScalarVolumeField& svfield, ComputeGradientScheme scheme);
        //ʸ�����ݶ�
        TensorVolumeField ComputeGradient(VectorVolumeField& vvfield, ComputeGradientScheme scheme);

        //ͨ��
        ScalarVolumeField Convection(ScalarSurfaceField &phi);

    }  // namespace exp
}  // namespace zgCFD
