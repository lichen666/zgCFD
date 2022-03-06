#pragma once
#include "ZuoGuangCFD/struct/enums.h"
#include "ZuoGuangCFD/struct/zg_field.h"

namespace zgCFD
{
    namespace exp
    {
        //插值

        ScalarSurfaceField Interpolate(const ScalarVolumeField& svfield, TheInterpolationScheme scheme);
        VectorSurfaceField Interpolate(const VectorVolumeField& vvfield, TheInterpolationScheme scheme);

        VectorSurfaceField InterpolateGradient(const VectorVolumeField& grad_field, const ScalarVolumeField& svfield,
                                               TheInterpolationScheme scheme);
        TensorSurfaceField InterpolateGradient(const TensorVolumeField& grad_field, const VectorVolumeField& vvfield,
            TheInterpolationScheme scheme);
        //梯度
        //标量场梯度
        VectorVolumeField ComputeGradient(const ScalarVolumeField& svfield, ComputeGradientScheme scheme);
        //矢量场梯度
        TensorVolumeField ComputeGradient(const VectorVolumeField& vvfield, ComputeGradientScheme scheme);

    }  // namespace exp
}  // namespace zgCFD
