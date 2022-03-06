#pragma once
#include "ZuoGuangCFD/struct/enums.h"
#include "ZuoGuangCFD/struct/zg_field.h"

namespace zgCFD
{
    namespace exp
    {
        //��ֵ

        ScalarSurfaceField Interpolate(const ScalarVolumeField& svfield, TheInterpolationScheme scheme);
        VectorSurfaceField Interpolate(const VectorVolumeField& vvfield, TheInterpolationScheme scheme);

        VectorSurfaceField InterpolateGradient(const VectorVolumeField& grad_field, const ScalarVolumeField& svfield,
                                               TheInterpolationScheme scheme);
        TensorSurfaceField InterpolateGradient(const TensorVolumeField& grad_field, const VectorVolumeField& vvfield,
            TheInterpolationScheme scheme);
        //�ݶ�
        //�������ݶ�
        VectorVolumeField ComputeGradient(const ScalarVolumeField& svfield, ComputeGradientScheme scheme);
        //ʸ�����ݶ�
        TensorVolumeField ComputeGradient(const VectorVolumeField& vvfield, ComputeGradientScheme scheme);

    }  // namespace exp
}  // namespace zgCFD
