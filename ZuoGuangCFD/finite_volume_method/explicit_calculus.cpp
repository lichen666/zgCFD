#include "ZuoGuangCFD/finite_volume_method/explicit_calculus.h"

namespace zgCFD
{
    namespace exp
    {
        //体标量场插值到面
        ScalarSurfaceField Interpolate(ScalarVolumeField& svfield, TheInterpolationScheme scheme)
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

                        ssfield.interior_value_[i] =
                            (1 - gf) * svfield.interior_value_[owner] + gf * svfield.interior_value_[neighbour];
                    }

                    //边界面，直接根据边界条件计算
                    for (uint32_t i = 0; i < svfield.mesh_->number_of_boundaries_; i++)
                    {
                        uint32_t start_face = svfield.mesh_->boundaries_[i].start_face_;
                        uint32_t number_of_faces = svfield.mesh_->boundaries_[i].number_of_faces_;
                        std::string boundary_name = svfield.mesh_->boundaries_[i].boundary_name_;
                        BoundaryCondationType boundary_condation_type =
                            svfield.boundary_condation_[boundary_name].bc_type_;

                        switch (boundary_condation_type)
                        {
                            case zgCFD::BoundaryCondationType::SPECIFIEDVALUE:
                                for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                                {
                                    uint32_t index = j - svfield.mesh_->number_of_interior_faces_;

                                    ssfield.boundary_value_[index] =
                                        svfield.boundary_condation_[boundary_name].bc_value_;
                                }
                                break;
                            case zgCFD::BoundaryCondationType::SPECIFIEDFLUX:
                                break;
                            case zgCFD::BoundaryCondationType::ZEROFLUX:
                                break;
                            case zgCFD::BoundaryCondationType::ZEROGRADIENT:
                                for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                                {
                                    uint32_t index = j - svfield.mesh_->number_of_interior_faces_;
                                    int owner = svfield.mesh_->faces_[j].owner_;

                                    ssfield.boundary_value_[index] = svfield.interior_value_[owner];
                                }
                                break;
                            case ::zgCFD::BoundaryCondationType::CALCULATED:
                                for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                                {
                                    uint32_t index = j - svfield.mesh_->number_of_interior_faces_;

                                    ssfield.boundary_value_[index] = svfield.boundary_value_[index];
                                }
                                break;
                            case zgCFD::BoundaryCondationType::SLIP:
                                break;
                            case zgCFD::BoundaryCondationType::NOSLIP:
                                break;
                            case zgCFD::BoundaryCondationType::INLET:
                                break;
                            case zgCFD::BoundaryCondationType::OUTLET:
                                break;
                            case zgCFD::BoundaryCondationType::EMPTY:
                                break;
                            default:
                                break;
                        }
                    }
                    break;
            }

            return ssfield;
        }

        VectorSurfaceField Interpolate(VectorVolumeField& vvfield, TheInterpolationScheme scheme)
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

                        vsfield.interior_value_[i] =
                            (1 - gf) * vvfield.interior_value_[owner] + gf * vvfield.interior_value_[neighbour];
                    }

                    //边界面，直接根据边界条件计算
                    for (uint32_t i = 0; i < vvfield.mesh_->number_of_boundaries_; i++)
                    {
                        uint32_t start_face = vvfield.mesh_->boundaries_[i].start_face_;
                        uint32_t number_of_faces = vvfield.mesh_->boundaries_[i].number_of_faces_;
                        std::string boundary_name = vvfield.mesh_->boundaries_[i].boundary_name_;
                        BoundaryCondationType boundary_condation_type =
                            vvfield.boundary_condation_[boundary_name].bc_type_;

                        switch (boundary_condation_type)
                        {
                            case zgCFD::BoundaryCondationType::SPECIFIEDVALUE:
                                for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                                {
                                    uint32_t index = j - vvfield.mesh_->number_of_interior_faces_;

                                    vsfield.boundary_value_[index] =
                                        vvfield.boundary_condation_[boundary_name].bc_value_;
                                }
                                break;
                            case ::zgCFD::BoundaryCondationType::CALCULATED:
                                for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                                {
                                    uint32_t index = j - vvfield.mesh_->number_of_interior_faces_;

                                    vsfield.boundary_value_[index] = vvfield.boundary_value_[index];
                                }
                                break;
                            case zgCFD::BoundaryCondationType::SPECIFIEDFLUX:
                                break;
                            case zgCFD::BoundaryCondationType::ZEROFLUX:
                                break;
                            case zgCFD::BoundaryCondationType::ZEROGRADIENT:
                                for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                                {
                                    uint32_t index = j - vvfield.mesh_->number_of_interior_faces_;
                                    int owner = vvfield.mesh_->faces_[j].owner_;

                                    vsfield.boundary_value_[index] = vvfield.interior_value_[owner];
                                }
                                break;
                            case zgCFD::BoundaryCondationType::SLIP:
                                break;
                            case zgCFD::BoundaryCondationType::NOSLIP:
                                break;
                            case zgCFD::BoundaryCondationType::INLET:
                                break;
                            case zgCFD::BoundaryCondationType::OUTLET:
                                break;
                            case zgCFD::BoundaryCondationType::EMPTY:
                                break;
                            default:
                                break;
                        }
                    }
                    break;
            }

            return vsfield;
        }

        //体梯度场插值到面
        VectorSurfaceField InterpolateGradient(VectorVolumeField& grad_field, ScalarVolumeField& svfield,
                                               TheInterpolationScheme scheme)
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

                        vsfield.interior_value_[i] =
                            (1 - gf) * grad_field.interior_value_[owner] + gf * grad_field.interior_value_[neighbour];
                    }

                    //边界面，直接根据边界条件计算
                    for (uint32_t i = 0; i < svfield.mesh_->number_of_boundaries_; i++)
                    {
                        uint32_t start_face = svfield.mesh_->boundaries_[i].start_face_;
                        uint32_t number_of_faces = svfield.mesh_->boundaries_[i].number_of_faces_;
                        std::string boundary_name = svfield.mesh_->boundaries_[i].boundary_name_;
                        BoundaryCondationType boundary_condation_type =
                            svfield.boundary_condation_[boundary_name].bc_type_;

                        switch (boundary_condation_type)
                        {
                            case zgCFD::BoundaryCondationType::SPECIFIEDVALUE:
                                for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                                {
                                    uint32_t index = j - svfield.mesh_->number_of_interior_faces_;
                                    int owner = svfield.mesh_->faces_[j].owner_;

                                    vsfield.boundary_value_[index] = grad_field.interior_value_[owner];
                                }
                                break;
                            case zgCFD::BoundaryCondationType::SPECIFIEDFLUX:
                                break;
                            case zgCFD::BoundaryCondationType::ZEROFLUX:
                                break;
                            case zgCFD::BoundaryCondationType::ZEROGRADIENT:
                                for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                                {
                                    uint32_t index = j - svfield.mesh_->number_of_interior_faces_;

                                    vsfield.boundary_value_[index] = GeneralVector(0, 0, 0);
                                }
                                break;
                            case zgCFD::BoundaryCondationType::SLIP:
                                break;
                            case zgCFD::BoundaryCondationType::NOSLIP:
                                break;
                            case zgCFD::BoundaryCondationType::INLET:
                                break;
                            case zgCFD::BoundaryCondationType::OUTLET:
                                break;
                            case zgCFD::BoundaryCondationType::EMPTY:
                                break;
                            default:
                                break;
                        }
                    }
                    break;

                case TheInterpolationScheme::AVERAGECORRECTED:
                    //内部面，插值
                    for (int i = 0; i < grad_field.mesh_->number_of_interior_faces_; i++)
                    {
                        int owner = grad_field.mesh_->faces_[i].owner_;
                        int neighbour = grad_field.mesh_->faces_[i].neighbour_;

                        Scalar gf = grad_field.mesh_->faces_[i].gf_;

                        vsfield.interior_value_[i] =
                            (1 - gf) * grad_field.interior_value_[owner] + gf * grad_field.interior_value_[neighbour];

                        GeneralVector d_cf = grad_field.mesh_->faces_[i].cn_;
                        GeneralVector e_cf = grad_field.mesh_->faces_[i].e_cn_;

                        Scalar local_grad_mag_f =
                            (svfield.interior_value_[neighbour] - svfield.interior_value_[owner]) / d_cf.Magnitude();
                        GeneralVector local_grad = local_grad_mag_f * e_cf;

                        Scalar local_avg_grad_mag = vsfield.interior_value_[i] & e_cf;
                        GeneralVector local_avg_grad = local_avg_grad_mag * e_cf;

                        vsfield.interior_value_[i] = vsfield.interior_value_[i] - local_avg_grad + local_grad;
                    }

                    //边界面，直接根据边界条件计算
                    for (uint32_t i = 0; i < svfield.mesh_->number_of_boundaries_; i++)
                    {
                        uint32_t start_face = svfield.mesh_->boundaries_[i].start_face_;
                        uint32_t number_of_faces = svfield.mesh_->boundaries_[i].number_of_faces_;
                        std::string boundary_name = svfield.mesh_->boundaries_[i].boundary_name_;
                        BoundaryCondationType boundary_condation_type =
                            svfield.boundary_condation_[boundary_name].bc_type_;

                        switch (boundary_condation_type)
                        {
                            case zgCFD::BoundaryCondationType::SPECIFIEDVALUE:
                                for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                                {
                                    uint32_t index = j - svfield.mesh_->number_of_interior_faces_;
                                    int owner = svfield.mesh_->faces_[j].owner_;

                                    vsfield.boundary_value_[index] = grad_field.interior_value_[owner];
                                }
                                break;
                            case zgCFD::BoundaryCondationType::SPECIFIEDFLUX:
                                break;
                            case zgCFD::BoundaryCondationType::ZEROFLUX:
                                break;
                            case zgCFD::BoundaryCondationType::ZEROGRADIENT:
                                for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                                {
                                    uint32_t index = j - svfield.mesh_->number_of_interior_faces_;

                                    vsfield.boundary_value_[index] = GeneralVector(0, 0, 0);
                                }
                                break;
                            case zgCFD::BoundaryCondationType::SLIP:
                                break;
                            case zgCFD::BoundaryCondationType::NOSLIP:
                                break;
                            case zgCFD::BoundaryCondationType::INLET:
                                break;
                            case zgCFD::BoundaryCondationType::OUTLET:
                                break;
                            case zgCFD::BoundaryCondationType::EMPTY:
                                break;
                            default:
                                break;
                        }
                    }
                    break;
            }

            return vsfield;
        }

        TensorSurfaceField InterpolateGradient(TensorVolumeField& grad_field, VectorVolumeField& vvfield,
                                               TheInterpolationScheme scheme)
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

                        tsfield.interior_value_[i] =
                            (1 - gf) * grad_field.interior_value_[owner] + gf * grad_field.interior_value_[neighbour];
                    }

                    //边界面，直接根据边界条件计算
                    for (uint32_t i = 0; i < vvfield.mesh_->number_of_boundaries_; i++)
                    {
                        uint32_t start_face = vvfield.mesh_->boundaries_[i].start_face_;
                        uint32_t number_of_faces = vvfield.mesh_->boundaries_[i].number_of_faces_;
                        std::string boundary_name = vvfield.mesh_->boundaries_[i].boundary_name_;
                        BoundaryCondationType boundary_condation_type =
                            vvfield.boundary_condation_[boundary_name].bc_type_;

                        switch (boundary_condation_type)
                        {
                            case zgCFD::BoundaryCondationType::SPECIFIEDVALUE:
                                for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                                {
                                    uint32_t index = j - vvfield.mesh_->number_of_interior_faces_;
                                    int owner = vvfield.mesh_->faces_[j].owner_;

                                    tsfield.boundary_value_[index] = grad_field.interior_value_[owner];
                                }
                                break;
                            case zgCFD::BoundaryCondationType::SPECIFIEDFLUX:
                                break;
                            case zgCFD::BoundaryCondationType::ZEROFLUX:
                                break;
                            case zgCFD::BoundaryCondationType::ZEROGRADIENT:
                                for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                                {
                                    uint32_t index = j - vvfield.mesh_->number_of_interior_faces_;

                                    tsfield.boundary_value_[index] = GeneralTensor(0, 0, 0, 0, 0, 0, 0, 0, 0);
                                }
                                break;
                            case zgCFD::BoundaryCondationType::SLIP:
                                break;
                            case zgCFD::BoundaryCondationType::NOSLIP:
                                break;
                            case zgCFD::BoundaryCondationType::INLET:
                                break;
                            case zgCFD::BoundaryCondationType::OUTLET:
                                break;
                            case zgCFD::BoundaryCondationType::EMPTY:
                                break;
                            default:
                                break;
                        }
                    }
                    break;

                case TheInterpolationScheme::AVERAGECORRECTED:
                    //内部面，插值
                    for (int i = 0; i < grad_field.mesh_->number_of_interior_faces_; i++)
                    {
                        int owner = grad_field.mesh_->faces_[i].owner_;
                        int neighbour = grad_field.mesh_->faces_[i].neighbour_;

                        Scalar gf = grad_field.mesh_->faces_[i].gf_;

                        tsfield.interior_value_[i] =
                            (1 - gf) * grad_field.interior_value_[owner] + gf * grad_field.interior_value_[neighbour];

                        GeneralVector d_cf = grad_field.mesh_->faces_[i].cn_;
                        GeneralVector e_cf = grad_field.mesh_->faces_[i].e_cn_;

                        GeneralVector local_grad_mag_f =
                            (vvfield.interior_value_[neighbour] - vvfield.interior_value_[owner]) / d_cf.Magnitude();
                        GeneralTensor local_grad(local_grad_mag_f.x() * e_cf, local_grad_mag_f.y() * e_cf,
                                                 local_grad_mag_f.z() * e_cf);

                        Scalar local_avg_grad_mag1 =
                            GeneralVector(tsfield.interior_value_[i].xx(), tsfield.interior_value_[i].xy(),
                                          tsfield.interior_value_[i].xz()) &
                            e_cf;

                        Scalar local_avg_grad_mag2 =
                            GeneralVector(tsfield.interior_value_[i].yx(), tsfield.interior_value_[i].yy(),
                                          tsfield.interior_value_[i].yz()) &
                            e_cf;
                        Scalar local_avg_grad_mag3 =
                            GeneralVector(tsfield.interior_value_[i].zx(), tsfield.interior_value_[i].zy(),
                                          tsfield.interior_value_[i].zz()) &
                            e_cf;

                        GeneralTensor local_avg_grad(local_avg_grad_mag1 * e_cf, local_avg_grad_mag2 * e_cf,
                                                     local_avg_grad_mag3 * e_cf);

                        tsfield.interior_value_[i] = tsfield.interior_value_[i] - local_avg_grad + local_grad;
                    }

                    //边界面，直接根据边界条件计算
                    for (uint32_t i = 0; i < vvfield.mesh_->number_of_boundaries_; i++)
                    {
                        uint32_t start_face = vvfield.mesh_->boundaries_[i].start_face_;
                        uint32_t number_of_faces = vvfield.mesh_->boundaries_[i].number_of_faces_;
                        std::string boundary_name = vvfield.mesh_->boundaries_[i].boundary_name_;
                        BoundaryCondationType boundary_condation_type =
                            vvfield.boundary_condation_[boundary_name].bc_type_;

                        switch (boundary_condation_type)
                        {
                            case zgCFD::BoundaryCondationType::SPECIFIEDVALUE:
                                for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                                {
                                    uint32_t index = j - vvfield.mesh_->number_of_interior_faces_;
                                    int owner = vvfield.mesh_->faces_[j].owner_;

                                    tsfield.boundary_value_[index] = grad_field.interior_value_[owner];
                                }
                                break;
                            case zgCFD::BoundaryCondationType::SPECIFIEDFLUX:
                                break;
                            case zgCFD::BoundaryCondationType::ZEROFLUX:
                                break;
                            case zgCFD::BoundaryCondationType::ZEROGRADIENT:
                                for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                                {
                                    uint32_t index = j - vvfield.mesh_->number_of_interior_faces_;

                                    tsfield.boundary_value_[index] = GeneralTensor(0, 0, 0, 0, 0, 0, 0, 0, 0);
                                }
                                break;
                            case zgCFD::BoundaryCondationType::SLIP:
                                break;
                            case zgCFD::BoundaryCondationType::NOSLIP:
                                break;
                            case zgCFD::BoundaryCondationType::INLET:
                                break;
                            case zgCFD::BoundaryCondationType::OUTLET:
                                break;
                            case zgCFD::BoundaryCondationType::EMPTY:
                                break;
                            default:
                                break;
                        }
                    }
                    break;
            }

            return tsfield;
        }

        //标量场梯度
        VectorVolumeField ComputeGradient(ScalarVolumeField& svfield, ComputeGradientScheme scheme)
        {
            VectorVolumeField vvfield(*svfield.mesh_, "tmp", "tmp");

            ScalarSurfaceField interpolated_field = Interpolate(svfield, TheInterpolationScheme::AVERAGE);

            //内部面的计算
            for (int i = 0; i < svfield.mesh_->number_of_interior_faces_; i++)
            {
                int owner = svfield.mesh_->faces_[i].owner_;
                int neighbour = svfield.mesh_->faces_[i].neighbour_;
                GeneralVector Sf = svfield.mesh_->faces_[i].sf_;

                vvfield.interior_value_[owner] =
                    vvfield.interior_value_[owner] + interpolated_field.interior_value_[i] * Sf;
                vvfield.interior_value_[neighbour] =
                    vvfield.interior_value_[neighbour] - interpolated_field.interior_value_[i] * Sf;
            }

            for (uint32_t i = svfield.mesh_->number_of_interior_faces_; i < svfield.mesh_->number_of_faces_; i++)
            {
                uint32_t index = i - svfield.mesh_->number_of_interior_faces_;
                int owner = svfield.mesh_->faces_[i].owner_;
                GeneralVector Sf = svfield.mesh_->faces_[i].sf_;
                vvfield.interior_value_[owner] =
                    vvfield.interior_value_[owner] + interpolated_field.boundary_value_[index] * Sf;
            }

            for (int i = 0; i < svfield.mesh_->number_of_elements_; i++)
            {
                vvfield.interior_value_[i] = vvfield.interior_value_[i] / svfield.mesh_->elements_[i].volume_;
            }

            //边界面的计算
            for (uint32_t i = 0; i < svfield.mesh_->number_of_boundaries_; i++)
            {
                uint32_t start_face = svfield.mesh_->boundaries_[i].start_face_;
                uint32_t number_of_faces = svfield.mesh_->boundaries_[i].number_of_faces_;
                std::string boundary_name = svfield.mesh_->boundaries_[i].boundary_name_;
                BoundaryCondationType boundary_condation_type = svfield.boundary_condation_[boundary_name].bc_type_;

                switch (boundary_condation_type)
                {
                    case zgCFD::BoundaryCondationType::SPECIFIEDVALUE:
                        for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                        {
                            uint32_t index = j - svfield.mesh_->number_of_interior_faces_;

                            int owner = svfield.mesh_->faces_[j].owner_;

                            vvfield.boundary_value_[index] = vvfield.interior_value_[owner];
                        }
                        break;
                    case zgCFD::BoundaryCondationType::SPECIFIEDFLUX:
                        break;
                    case zgCFD::BoundaryCondationType::ZEROFLUX:
                        break;
                    case zgCFD::BoundaryCondationType::ZEROGRADIENT:
                        for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                        {
                            uint32_t index = j - svfield.mesh_->number_of_interior_faces_;

                            vvfield.boundary_value_[index] = GeneralVector(0, 0, 0);
                        }
                        break;
                    case zgCFD::BoundaryCondationType::SLIP:
                        break;
                    case zgCFD::BoundaryCondationType::NOSLIP:
                        break;
                    case zgCFD::BoundaryCondationType::INLET:
                        break;
                    case zgCFD::BoundaryCondationType::OUTLET:
                        break;
                    case zgCFD::BoundaryCondationType::EMPTY:
                        break;
                    default:
                        break;
                }
            }

            return vvfield;
        }

        TensorVolumeField ComputeGradient(VectorVolumeField& vvfield, ComputeGradientScheme scheme)
        {
            TensorVolumeField tvfield(*vvfield.mesh_, "tmp", "tmp");

            VectorSurfaceField interpolated_field = Interpolate(vvfield, TheInterpolationScheme::AVERAGE);

            //内部面的计算
            for (int i = 0; i < vvfield.mesh_->number_of_interior_faces_; i++)
            {
                int owner = vvfield.mesh_->faces_[i].owner_;
                int neighbour = vvfield.mesh_->faces_[i].neighbour_;
                GeneralVector Sf = vvfield.mesh_->faces_[i].sf_;

                GeneralVector vx = interpolated_field.interior_value_[i].x() * Sf;
                GeneralVector vy = interpolated_field.interior_value_[i].y() * Sf;
                GeneralVector vz = interpolated_field.interior_value_[i].z() * Sf;

                GeneralTensor t(vx, vy, vz);
                tvfield.interior_value_[owner] = tvfield.interior_value_[owner] + t;
                tvfield.interior_value_[neighbour] = tvfield.interior_value_[neighbour] - t;
            }

            for (uint32_t i = vvfield.mesh_->number_of_interior_faces_; i < vvfield.mesh_->number_of_faces_; i++)
            {
                uint32_t index = i - vvfield.mesh_->number_of_interior_faces_;
                int owner = vvfield.mesh_->faces_[i].owner_;
                GeneralVector Sf = vvfield.mesh_->faces_[i].sf_;

                GeneralVector vx = interpolated_field.boundary_value_[index].x() * Sf;
                GeneralVector vy = interpolated_field.boundary_value_[index].y() * Sf;
                GeneralVector vz = interpolated_field.boundary_value_[index].z() * Sf;

                GeneralTensor t(vx, vy, vz);
                tvfield.interior_value_[owner] = tvfield.interior_value_[owner] + t;
            }

            for (int i = 0; i < vvfield.mesh_->number_of_elements_; i++)
            {
                tvfield.interior_value_[i] = tvfield.interior_value_[i] / vvfield.mesh_->elements_[i].volume_;
            }

            //边界面的计算
            for (uint32_t i = 0; i < vvfield.mesh_->number_of_boundaries_; i++)
            {
                uint32_t start_face = vvfield.mesh_->boundaries_[i].start_face_;
                uint32_t number_of_faces = vvfield.mesh_->boundaries_[i].number_of_faces_;
                std::string boundary_name = vvfield.mesh_->boundaries_[i].boundary_name_;
                BoundaryCondationType boundary_condation_type = vvfield.boundary_condation_[boundary_name].bc_type_;

                switch (boundary_condation_type)
                {
                    case zgCFD::BoundaryCondationType::SPECIFIEDVALUE:
                        for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                        {
                            uint32_t index = j - vvfield.mesh_->number_of_interior_faces_;

                            int owner = vvfield.mesh_->faces_[j].owner_;

                            tvfield.boundary_value_[index] = tvfield.interior_value_[owner];
                        }
                        break;
                    case zgCFD::BoundaryCondationType::SPECIFIEDFLUX:
                        break;
                    case zgCFD::BoundaryCondationType::ZEROFLUX:
                        break;
                    case zgCFD::BoundaryCondationType::ZEROGRADIENT:
                        for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                        {
                            uint32_t index = j - vvfield.mesh_->number_of_interior_faces_;

                            tvfield.boundary_value_[index] = GeneralTensor(0, 0, 0, 0, 0, 0, 0, 0, 0);
                        }
                        break;
                    case zgCFD::BoundaryCondationType::SLIP:
                        break;
                    case zgCFD::BoundaryCondationType::NOSLIP:
                        break;
                    case zgCFD::BoundaryCondationType::INLET:
                        break;
                    case zgCFD::BoundaryCondationType::OUTLET:
                        break;
                    case zgCFD::BoundaryCondationType::EMPTY:
                        break;
                    default:
                        break;
                }
            }

            return tvfield;
        }

        ScalarVolumeField Convection(ScalarSurfaceField& phi)
        {
            ScalarVolumeField svfield(*(phi.mesh_), "tmp", "tmp");

            //内部面
            for (uint32_t i = 0; i < phi.mesh_->number_of_interior_faces_; i++)
            {
                uint32_t owner = phi.mesh_->faces_[i].owner_;
                uint32_t neighbour = phi.mesh_->faces_[i].neighbour_;

                svfield.interior_value_[owner] = svfield.interior_value_[owner] + phi.interior_value_[i];
                svfield.interior_value_[neighbour] = svfield.interior_value_[neighbour] - phi.interior_value_[i];
            }
            //边界面
            for (uint32_t i = phi.mesh_->number_of_interior_faces_; i < phi.mesh_->number_of_faces_; i++)
            {
                uint32_t owner = phi.mesh_->faces_[i].owner_;
                uint32_t index = i - phi.mesh_->number_of_interior_faces_;

                svfield.interior_value_[owner] = svfield.interior_value_[owner] + phi.boundary_value_[index];
            }

            return svfield;
        }

    }  // namespace exp
}  // namespace zgCFD
