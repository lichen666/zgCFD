#include "ZuoGuangCFD/finite_volume_method/explicit_calculus.h"

namespace zgCFD
{
    namespace exp
    {
        //体标量场插值到面
        ScalarSurfaceField Interpolate(const ScalarVolumeField& svfield, TheInterpolationScheme scheme)
        {
            ScalarSurfaceField ssfield(*svfield.mesh_, "tmp", "tmp");

            switch (scheme)
            {
            case TheInterpolationScheme::AVERAGE:
                    //内部面，插值
                    for (int i = 0; i < svfield.mesh_->number_of_interior_faces_; i++)
                    {
                        int owner = svfield.mesh_->faces_[i].owner_;
                        int neighbour = svfield.mesh_->faces_[i].neighbour_;

                        Scalar gf = svfield.mesh_->faces_[i].gf_;

                        ssfield.value_[i] = gf * svfield.value_[owner] + (1 - gf) * svfield.value_[neighbour];
                    }

                    //边界面，直接根据边界条件计算
                    for (int i = 0; i < svfield.mesh_->number_of_faces_ - svfield.mesh_->number_of_interior_faces_; i++)
                    {   
                        int num1 = i + svfield.mesh_->number_of_interior_faces_;
                        int num2 = i + svfield.mesh_->number_of_elements_;
                        ssfield.value_[num1] = svfield.value_[num2];
                    }
                    break;
            }

            return ssfield;
        }

        //体梯度场插值到面
        VectorSurfaceField InterpolateGradient(const VectorVolumeField& grad_field, const ScalarVolumeField& svfield,TheInterpolationScheme scheme)
        {
            VectorSurfaceField vsfield(*grad_field.mesh_, "tmp", "tmp");

            switch (scheme)
            {
            case TheInterpolationScheme::AVERAGE:
                //内部面，插值
                for (int i = 0; i < grad_field.mesh_->number_of_interior_faces_; i++)
                {
                    int owner = grad_field.mesh_->faces_[i].owner_;
                    int neighbour = grad_field.mesh_->faces_[i].neighbour_;

                    Scalar gf = grad_field.mesh_->faces_[i].gf_;

                    vsfield.value_[i] = gf * grad_field.value_[owner] + (1 - gf) * grad_field.value_[neighbour];
                }

                //边界面，直接根据边界条件计算
                for (int i = 0; i < grad_field.mesh_->number_of_faces_ - grad_field.mesh_->number_of_interior_faces_; i++)
                {
                    int num1 = i + grad_field.mesh_->number_of_interior_faces_;
                    int num2 = i + grad_field.mesh_->number_of_elements_;
                    vsfield.value_[num1] = grad_field.value_[num2];
                }
                break;

            case TheInterpolationScheme::AVERAGECORRECTED:
                //内部面，插值
                for (int i = 0; i < grad_field.mesh_->number_of_interior_faces_; i++)
                {
                    int owner = grad_field.mesh_->faces_[i].owner_;
                    int neighbour = grad_field.mesh_->faces_[i].neighbour_;

                    Scalar gf = grad_field.mesh_->faces_[i].gf_;

                    vsfield.value_[i] = gf * grad_field.value_[owner] + (1 - gf) * grad_field.value_[neighbour];

                    GeneralVector d_cf = grad_field.mesh_->faces_[i].cn_;
                    GeneralVector e_cf = grad_field.mesh_->faces_[i].e_cn_;

                    Scalar local_grad_mag_f = (svfield.value_[neighbour] - svfield.value_[owner]) / d_cf.Magnitude();
                    GeneralVector local_grad = local_grad_mag_f * e_cf;

                    Scalar local_avg_grad_mag = vsfield.value_[i] & e_cf;
                    GeneralVector local_avg_grad = local_avg_grad_mag * e_cf;

                    vsfield.value_[i] = vsfield.value_[i] - local_avg_grad + local_grad;
                }

                //边界面，直接根据边界条件计算
                for (int i = 0; i < grad_field.mesh_->number_of_faces_ - grad_field.mesh_->number_of_interior_faces_; i++)
                {
                    int num1 = i + grad_field.mesh_->number_of_interior_faces_;
                    int num2 = i + grad_field.mesh_->number_of_elements_;
                    vsfield.value_[num1] = grad_field.value_[num2];
                }
                break;
            }

            return vsfield;
        }

        TensorSurfaceField InterpolateGradient(const TensorVolumeField& grad_field, const VectorVolumeField& vvfield, TheInterpolationScheme scheme)
        {
            TensorSurfaceField tsfield(*grad_field.mesh_, "tmp", "tmp");

            switch (scheme)
            {
            case TheInterpolationScheme::AVERAGE:
                //内部面，插值
                for (int i = 0; i < grad_field.mesh_->number_of_interior_faces_; i++)
                {
                    int owner = grad_field.mesh_->faces_[i].owner_;
                    int neighbour = grad_field.mesh_->faces_[i].neighbour_;

                    Scalar gf = grad_field.mesh_->faces_[i].gf_;

                    tsfield.value_[i] = gf * grad_field.value_[owner] + (1 - gf) * grad_field.value_[neighbour];
                }

                //边界面，直接根据边界条件计算
                for (int i = 0; i < grad_field.mesh_->number_of_faces_ - grad_field.mesh_->number_of_interior_faces_; i++)
                {
                    int num1 = i + grad_field.mesh_->number_of_interior_faces_;
                    int num2 = i + grad_field.mesh_->number_of_elements_;
                    tsfield.value_[num1] = grad_field.value_[num2];
                }
                break;

            case TheInterpolationScheme::AVERAGECORRECTED:
                //内部面，插值
                for (int i = 0; i < grad_field.mesh_->number_of_interior_faces_; i++)
                {
                    int owner = grad_field.mesh_->faces_[i].owner_;
                    int neighbour = grad_field.mesh_->faces_[i].neighbour_;

                    Scalar gf = grad_field.mesh_->faces_[i].gf_;

                    tsfield.value_[i] = gf * grad_field.value_[owner] + (1 - gf) * grad_field.value_[neighbour];

                    GeneralVector d_cf = grad_field.mesh_->faces_[i].cn_;
                    GeneralVector e_cf = grad_field.mesh_->faces_[i].e_cn_;

                    GeneralVector local_grad_mag_f = (vvfield.value_[neighbour] - vvfield.value_[owner]) / d_cf.Magnitude();
                    GeneralTensor local_grad(local_grad_mag_f.x() * e_cf, local_grad_mag_f.y() * e_cf, local_grad_mag_f.z() * e_cf);

                    Scalar local_avg_grad_mag1 = GeneralVector(tsfield.value_[i].xx(), tsfield.value_[i].xy(), tsfield.value_[i].xz()) & e_cf;
                    Scalar local_avg_grad_mag2 = GeneralVector(tsfield.value_[i].yx(), tsfield.value_[i].yy(), tsfield.value_[i].yz()) & e_cf;
                    Scalar local_avg_grad_mag3 = GeneralVector(tsfield.value_[i].zx(), tsfield.value_[i].zy(), tsfield.value_[i].zz()) & e_cf;
                        

                    GeneralTensor local_avg_grad(local_avg_grad_mag1 * e_cf, local_avg_grad_mag2 * e_cf, local_avg_grad_mag2 * e_cf);

                    tsfield.value_[i] = tsfield.value_[i] - local_avg_grad + local_grad;
                }

                //边界面，直接根据边界条件计算
                for (int i = 0; i < grad_field.mesh_->number_of_faces_ - grad_field.mesh_->number_of_interior_faces_; i++)
                {
                    int num1 = i + grad_field.mesh_->number_of_interior_faces_;
                    int num2 = i + grad_field.mesh_->number_of_elements_;
                    tsfield.value_[num1] = grad_field.value_[num2];
                }
                break;
            }

            return tsfield;
        }

        VectorSurfaceField Interpolate(const VectorVolumeField& vvfield, TheInterpolationScheme scheme)
        {
            VectorSurfaceField vsfield(*vvfield.mesh_, "tmp", "tmp");

            switch (scheme)
            {
            case TheInterpolationScheme::AVERAGE:
                //内部面，插值
                for (int i = 0; i < vvfield.mesh_->number_of_interior_faces_; i++)
                {
                    int owner = vvfield.mesh_->faces_[i].owner_;
                    int neighbour = vvfield.mesh_->faces_[i].neighbour_;

                    Scalar gf = vvfield.mesh_->faces_[i].gf_;

                    vsfield.value_[i] = gf * vvfield.value_[owner] + (1 - gf) * vvfield.value_[neighbour];
                }

                //边界面，直接根据边界条件计算
                for (int i = 0; i < vvfield.mesh_->number_of_faces_ - vvfield.mesh_->number_of_interior_faces_; i++)
                {
                    int num1 = i + vvfield.mesh_->number_of_interior_faces_;
                    int num2 = i + vvfield.mesh_->number_of_elements_;
                    vsfield.value_[num1] = vvfield.value_[num2];
                }
                break;
            }

            return vsfield;
        }

        //标量场梯度
        VectorVolumeField ComputeGradient(const ScalarVolumeField& svfield, ComputeGradientScheme scheme)
        {
            VectorVolumeField vvfield(*svfield.mesh_, "tmp", "tmp");

            ScalarSurfaceField interpolated_field = Interpolate(svfield, TheInterpolationScheme::AVERAGE);

            //内部面的计算
            for (int i = 0; i < svfield.mesh_->number_of_interior_faces_; i++)
            {
                int owner = svfield.mesh_->faces_[i].owner_;
                int neighbour = svfield.mesh_->faces_[i].neighbour_;
                GeneralVector Sf = svfield.mesh_->faces_[i].sf_;

                vvfield.value_[owner] = vvfield.value_[owner] + interpolated_field.value_[i] * Sf;
                vvfield.value_[neighbour] = vvfield.value_[neighbour] - interpolated_field.value_[i] * Sf;
            }

            //边界面的计算
            for (int i = svfield.mesh_->number_of_interior_faces_; i < svfield.mesh_->number_of_faces_ ; i++)
            {
                int owner = svfield.mesh_->faces_[i].owner_;
                GeneralVector Sf = svfield.mesh_->faces_[i].sf_;

                vvfield.value_[owner] = vvfield.value_[owner] + interpolated_field.value_[i] * Sf;
            }

            for (int i = 0; i < svfield.mesh_->number_of_elements_; i++)
            {
                vvfield.value_[i] = vvfield.value_[i] / svfield.mesh_->elements_[i].volume_;
            }

            return vvfield;
        }

        TensorVolumeField ComputeGradient(const VectorVolumeField& vvfield, ComputeGradientScheme scheme)
        {
            TensorVolumeField tvfield(*vvfield.mesh_, "tmp", "tmp");

            VectorSurfaceField interpolated_field = Interpolate(vvfield, TheInterpolationScheme::AVERAGE);

            //内部面的计算
            for (int i = 0; i < vvfield.mesh_->number_of_interior_faces_; i++)
            {
                int owner = vvfield.mesh_->faces_[i].owner_;
                int neighbour = vvfield.mesh_->faces_[i].neighbour_;
                GeneralVector Sf = vvfield.mesh_->faces_[i].sf_;

                GeneralVector vx = interpolated_field.value_[i].x() * Sf;
                GeneralVector vy = interpolated_field.value_[i].y() * Sf;
                GeneralVector vz = interpolated_field.value_[i].z() * Sf;

                GeneralTensor t(vx, vy, vz);
                tvfield.value_[owner] = tvfield.value_[owner] + t;
                tvfield.value_[neighbour] = tvfield.value_[neighbour] - t;
            }

            //边界面的计算
            for (int i = vvfield.mesh_->number_of_interior_faces_; i < vvfield.mesh_->number_of_faces_; i++)
            {
                int owner = vvfield.mesh_->faces_[i].owner_;
                GeneralVector Sf = vvfield.mesh_->faces_[i].sf_;

                GeneralVector vx = interpolated_field.value_[i].x() * Sf;
                GeneralVector vy = interpolated_field.value_[i].y() * Sf;
                GeneralVector vz = interpolated_field.value_[i].z() * Sf;

                GeneralTensor t(vx, vy, vz);
                tvfield.value_[owner] = tvfield.value_[owner] + t;
            }

            for (int i = 0; i < vvfield.mesh_->number_of_elements_; i++)
            {
                tvfield.value_[i] = tvfield.value_[i] / vvfield.mesh_->elements_[i].volume_;
            }

            return tvfield;
        }

    }  // namespace exp
}  // namespace zgCFD
