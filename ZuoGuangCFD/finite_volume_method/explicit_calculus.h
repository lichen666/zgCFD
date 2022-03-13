#pragma once
#include "ZuoGuangCFD/struct/enums.h"
#include "ZuoGuangCFD/struct/zg_field.h"

namespace zgCFD
{
    namespace exp
    {
        //插值

        ScalarSurfaceField Interpolate(ScalarVolumeField& svfield, TheInterpolationScheme scheme);
        VectorSurfaceField Interpolate(VectorVolumeField& vvfield, TheInterpolationScheme scheme);

        VectorSurfaceField InterpolateGradient(VectorVolumeField& grad_field,ScalarVolumeField& svfield,
                                               TheInterpolationScheme scheme);
        TensorSurfaceField InterpolateGradient(TensorVolumeField& grad_field,VectorVolumeField& vvfield,
            TheInterpolationScheme scheme);
        //梯度
        //标量场梯度
        VectorVolumeField ComputeGradient(ScalarVolumeField& svfield, ComputeGradientScheme scheme);
        //矢量场梯度
        TensorVolumeField ComputeGradient(VectorVolumeField& vvfield, ComputeGradientScheme scheme);

        //通量
        ScalarVolumeField Convection(ScalarSurfaceField &phi);

    }  // namespace exp
}  // namespace zgCFD
