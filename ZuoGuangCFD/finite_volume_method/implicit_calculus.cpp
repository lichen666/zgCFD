#include "ZuoGuangCFD/finite_volume_method/implicit_calculus.h"

#include "ZuoGuangCFD/struct/fvm_matrix.h"

namespace zgCFD
{
    namespace imp
    {

        FvmScalarMatrix Diffusion(const ScalarVolumeField& gamma, ScalarVolumeField& variable)
        {
            FvmScalarMatrix tmp_matrix;
            tmp_matrix.sorce_.resize(variable.mesh_->number_of_elements_, 0);
            tmp_matrix.matrix_.SetColumn(variable.mesh_->number_of_elements_);
            tmp_matrix.matrix_.SetRow(variable.mesh_->number_of_elements_);

            VectorVolumeField grad_variable = exp::ComputeGradient(variable, ComputeGradientScheme::GAUSS);
            VectorSurfaceField grad_variable_f =
                exp::InterpolateGradient(grad_variable, variable, TheInterpolationScheme::AVERAGECORRECTED);

            ScalarSurfaceField gamma_f = exp::Interpolate(gamma, TheInterpolationScheme::AVERAGE);

            // ScalarSurfaceField fluxcf(*variable.mesh_, "tmp", "tmp");
            // ScalarSurfaceField fluxff(*variable.mesh_, "tmp", "tmp");
            // ScalarSurfaceField fluxvf(*variable.mesh_, "tmp", "tmp");
            // ScalarSurfaceField fluxtf(*variable.mesh_, "tmp", "tmp");

            //内部面组装矩阵
            for (uint32_t i = 0; i < variable.mesh_->number_of_interior_faces_; i++)
            {
                int owner = variable.mesh_->faces_[i].owner_;
                int neighbour = variable.mesh_->faces_[i].neighbour_;

                //线性化系数
                Scalar fluxff = gamma_f.value_[i] * variable.mesh_->faces_[i].g_diff_;
                Scalar fluxvf = (grad_variable_f.value_[i] & variable.mesh_->faces_[i].t_) * gamma_f.value_[i];

                /*  fluxcf.value_[i] = gamma_f.value_[i] * variable.mesh_->faces_[i].g_diff_;
                  fluxff.value_[i] = -gamma_f.value_[i] * variable.mesh_->faces_[i].g_diff_;
                  fluxvf.value_[i] = -gamma_f.value_[i] * (grad_variable_f.value_[i] & variable.mesh_->faces_[i].t_);
                  fluxtf.value_[i] = fluxcf.value_[i] * variable.value_[owner] + fluxff.value_[i] *
                  variable.value_[neighbour] + fluxvf.value_[i];*/

                tmp_matrix.matrix_.InsertValue(owner, neighbour, fluxff);
                tmp_matrix.matrix_.InsertValue(owner, owner, -fluxff);
                tmp_matrix.matrix_.InsertValue(neighbour, neighbour, -fluxff);
                tmp_matrix.matrix_.InsertValue(neighbour, owner, fluxff);
                tmp_matrix.sorce_[owner] -= fluxvf;
            }

            //边界面组装矩阵
            uint32_t number_of_boundaries = variable.mesh_->number_of_boundaries_;

            for (uint32_t i = 0; i < number_of_boundaries; i++)
            {
                uint32_t start_face = variable.mesh_->boundaries_[i].start_face_;
                uint32_t number_of_faces = variable.mesh_->boundaries_[i].number_of_faces_;
                std::string boundary_name = variable.mesh_->boundaries_[i].boundary_name_;
                BoundaryCondationType boundary_condation_type = variable.boundary_condation_[boundary_name].bc_type_;

                switch (boundary_condation_type)
                {
                case zgCFD::BoundaryCondationType::SPECIFIEDVALUE:
                    for (uint32_t i = start_face; i < start_face + number_of_faces; i++)
                    {
                        int owner = variable.mesh_->faces_[i].owner_;
                        uint32_t index = i + variable.mesh_->number_of_elements_ -
                            variable.mesh_->number_of_interior_faces_;
                        Scalar fluxff = gamma_f.value_[index] * variable.mesh_->faces_[i].g_diff_;
                        Scalar fluxvb =
                            fluxff * variable.boundary_condation_[boundary_name].bc_value_ +
                            (grad_variable_f.value_[i] & variable.mesh_->faces_[i].t_) * gamma_f.value_[i];

                        tmp_matrix.matrix_.InsertValue(owner, owner, -fluxff);
                        tmp_matrix.sorce_[owner] -= fluxvb;
                    }
                    break;
                case zgCFD::BoundaryCondationType::SPECIFIEDFLUX:
                    for (uint32_t i = start_face; i < start_face + number_of_faces; i++)
                    {
                        int owner = variable.mesh_->faces_[i].owner_;
                        Scalar fluxvb = variable.mesh_->faces_[i].area_ *
                            variable.boundary_condation_[boundary_name].bc_value_;
                        tmp_matrix.sorce_[owner] -= fluxvb;
                    }
                    break;
                case zgCFD::BoundaryCondationType::ZEROFLUX:
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
            return tmp_matrix;
        }

        FvmScalarMatrix Diffusion(const Scalar& gamma, ScalarVolumeField& variable)
        {
            ScalarVolumeField gammaf(gamma, *variable.mesh_, "tmp", "tmp");
            FvmScalarMatrix tmp_matrix = Diffusion(gammaf, variable);
            return tmp_matrix;
        }

        FvmVectorMatrix Diffusion(const ScalarVolumeField& gamma, VectorVolumeField& variable)
        {
            FvmVectorMatrix tmp_matrix;
            tmp_matrix.sorce_.resize(variable.mesh_->number_of_elements_, GeneralVector(0,0,0));
            tmp_matrix.matrix_.SetColumn(variable.mesh_->number_of_elements_);
            tmp_matrix.matrix_.SetRow(variable.mesh_->number_of_elements_);

            TensorVolumeField grad_variable = exp::ComputeGradient(variable, ComputeGradientScheme::GAUSS);
            TensorSurfaceField grad_variable_f =
                exp::InterpolateGradient(grad_variable, variable, TheInterpolationScheme::AVERAGECORRECTED);

            ScalarSurfaceField gamma_f = exp::Interpolate(gamma, TheInterpolationScheme::AVERAGE);

            //内部面组装矩阵
            for (uint32_t i = 0; i < variable.mesh_->number_of_interior_faces_; i++)
            {
                int owner = variable.mesh_->faces_[i].owner_;
                int neighbour = variable.mesh_->faces_[i].neighbour_;

                //线性化系数
                Scalar fluxff = gamma_f.value_[i] * variable.mesh_->faces_[i].g_diff_;
                GeneralVector fluxvf = (grad_variable_f.value_[i] & variable.mesh_->faces_[i].t_) * gamma_f.value_[i];

                /*  fluxcf.value_[i] = gamma_f.value_[i] * variable.mesh_->faces_[i].g_diff_;
                  fluxff.value_[i] = -gamma_f.value_[i] * variable.mesh_->faces_[i].g_diff_;
                  fluxvf.value_[i] = -gamma_f.value_[i] * (grad_variable_f.value_[i] & variable.mesh_->faces_[i].t_);
                  fluxtf.value_[i] = fluxcf.value_[i] * variable.value_[owner] + fluxff.value_[i] *
                  variable.value_[neighbour] + fluxvf.value_[i];*/

                tmp_matrix.matrix_.InsertValue(owner, neighbour, fluxff);
                tmp_matrix.matrix_.InsertValue(owner, owner, -fluxff);
                tmp_matrix.matrix_.InsertValue(neighbour, neighbour, -fluxff);
                tmp_matrix.matrix_.InsertValue(neighbour, owner, fluxff);
                tmp_matrix.sorce_[owner] = tmp_matrix.sorce_[owner] - fluxvf;
            }

            //边界面组装矩阵
            uint32_t number_of_boundaries = variable.mesh_->number_of_boundaries_;

            for (uint32_t i = 0; i < number_of_boundaries; i++)
            {
                uint32_t start_face = variable.mesh_->boundaries_[i].start_face_;
                uint32_t number_of_faces = variable.mesh_->boundaries_[i].number_of_faces_;
                std::string boundary_name = variable.mesh_->boundaries_[i].boundary_name_;
                BoundaryCondationType boundary_condation_type = variable.boundary_condation_[boundary_name].bc_type_;

                switch (boundary_condation_type)
                {
                case zgCFD::BoundaryCondationType::SPECIFIEDVALUE:
                    for (uint32_t i = start_face; i < start_face + number_of_faces; i++)
                    {
                        int owner = variable.mesh_->faces_[i].owner_;
                        uint32_t index = i + variable.mesh_->number_of_elements_ -
                            variable.mesh_->number_of_interior_faces_;
                        Scalar fluxff = gamma_f.value_[index] * variable.mesh_->faces_[i].g_diff_;
                        GeneralVector fluxvb =
                            fluxff * variable.boundary_condation_[boundary_name].bc_value_ +
                            (grad_variable_f.value_[i] & variable.mesh_->faces_[i].t_) * gamma_f.value_[i];

                        tmp_matrix.matrix_.InsertValue(owner, owner, -fluxff);
                        tmp_matrix.sorce_[owner] = tmp_matrix.sorce_[owner] - fluxvb;
                    }
                    break;
                case zgCFD::BoundaryCondationType::SPECIFIEDFLUX:
                    for (uint32_t i = start_face; i < start_face + number_of_faces; i++)
                    {
                        int owner = variable.mesh_->faces_[i].owner_;
                        GeneralVector fluxvb = variable.mesh_->faces_[i].area_ *
                            variable.boundary_condation_[boundary_name].bc_value_;
                        tmp_matrix.sorce_[owner] = tmp_matrix.sorce_[owner] - fluxvb;
                    }
                    break;
                case zgCFD::BoundaryCondationType::ZEROFLUX:
                    break;
                case zgCFD::BoundaryCondationType::SLIP:
                    for (uint32_t i = start_face; i < start_face + number_of_faces; i++)
                    {
                        int owner = variable.mesh_->faces_[i].owner_;
                        uint32_t index = i + variable.mesh_->number_of_elements_ -
                            variable.mesh_->number_of_interior_faces_;
                        Scalar fluxff = gamma_f.value_[index] * variable.mesh_->faces_[i].g_diff_;
                        GeneralVector n = variable.mesh_->faces_[i].sf_ / variable.mesh_->faces_[i].area_;
                        GeneralVector variable_b = variable.value_[owner] - (variable.value_[owner] & n) * n;
                        GeneralVector fluxvb = fluxff * variable_b + (grad_variable_f.value_[i] & variable.mesh_->faces_[i].t_) * gamma_f.value_[i];

                        tmp_matrix.matrix_.InsertValue(owner, owner, -fluxff);
                        tmp_matrix.sorce_[owner] = tmp_matrix.sorce_[owner] - fluxvb;
                    }
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
            return tmp_matrix;
        }

        FvmVectorMatrix Diffusion(const Scalar& gamma, VectorVolumeField& variable)
        {
            ScalarVolumeField gammaf(gamma, *variable.mesh_, "tmp", "tmp");
            FvmVectorMatrix tmp_matrix = Diffusion(gammaf, variable);
            return tmp_matrix;
        }

        FvmScalarMatrix Convection(const ScalarSurfaceField phi, ScalarVolumeField& variable)
        {
            FvmScalarMatrix tmp_matrix;

            tmp_matrix.sorce_.resize(variable.mesh_->number_of_elements_, 0);
            tmp_matrix.matrix_.SetColumn(variable.mesh_->number_of_elements_);
            tmp_matrix.matrix_.SetRow(variable.mesh_->number_of_elements_);

            VectorVolumeField grad_field(*variable.mesh_, "tmp", "tmp");
            VectorSurfaceField grad_field_f(*variable.mesh_, "tmp", "tmp");

            grad_field = exp::ComputeGradient(variable, ComputeGradientScheme::GAUSS);
            grad_field_f = exp::InterpolateGradient(grad_field, variable, TheInterpolationScheme::AVERAGECORRECTED);
                      
            //内部面组装矩阵  QUICK格式
            for (uint32_t i = 0; i < variable.mesh_->number_of_interior_faces_; i++)
            {
                int owner = variable.mesh_->faces_[i].owner_;
                int neighbour = variable.mesh_->faces_[i].neighbour_;

                //线性化系数
                Scalar fluxc = std::max(phi.value_[i], 0.);
                Scalar fluxf = std::max(-phi.value_[i], 0.);

                point p1 = variable.mesh_->faces_[i].centroid_;
                point p2 = variable.mesh_->elements_[owner].centroid_;
                GeneralVector d_cf(p1-p2);

                Scalar fluxv = 0.5 * phi.value_[i] * ((grad_field.value_[owner] + grad_field_f.value_[i]) & d_cf);

                tmp_matrix.matrix_.InsertValue(owner, owner, fluxc);
                tmp_matrix.matrix_.InsertValue(owner, neighbour, -fluxf);
                tmp_matrix.matrix_.InsertValue(neighbour, neighbour, fluxf);
                tmp_matrix.matrix_.InsertValue(neighbour, owner, -fluxc);

                tmp_matrix.sorce_[owner] -= fluxv;
            }

            //边界面组装矩阵
            uint32_t number_of_boundaries = variable.mesh_->number_of_boundaries_;

            for (uint32_t i = 0; i < number_of_boundaries; i++)
            {
                uint32_t start_face = variable.mesh_->boundaries_[i].start_face_;
                uint32_t number_of_faces = variable.mesh_->boundaries_[i].number_of_faces_;
                std::string boundary_name = variable.mesh_->boundaries_[i].boundary_name_;
                BoundaryCondationType boundary_condation_type = variable.boundary_condation_[boundary_name].bc_type_;

                switch (boundary_condation_type)
                {
                case zgCFD::BoundaryCondationType::SPECIFIEDVALUE:
                    for (uint32_t i = start_face; i < start_face + number_of_faces; i++)
                    {
                        int owner = variable.mesh_->faces_[i].owner_;
                        uint32_t index = i + variable.mesh_->number_of_elements_ -
                            variable.mesh_->number_of_interior_faces_;
 
                        Scalar fluxv = phi.value_[i] * variable.boundary_condation_[boundary_name].bc_value_;
                        tmp_matrix.sorce_[owner] -= fluxv;
                    }
                    break;
                case zgCFD::BoundaryCondationType::SPECIFIEDFLUX:
                    break;
                case zgCFD::BoundaryCondationType::ZEROFLUX:
                    break;
                case zgCFD::BoundaryCondationType::ZEROGRADIENT:
                    for (uint32_t i = start_face; i < start_face + number_of_faces; i++)
                    {
                        int owner = variable.mesh_->faces_[i].owner_;
                        uint32_t index = i + variable.mesh_->number_of_elements_ -
                            variable.mesh_->number_of_interior_faces_;

                        Scalar fluxc = phi.value_[i] ;

                        tmp_matrix.matrix_.InsertValue(owner, owner, fluxc);
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
            
            return tmp_matrix;
        }

        FvmVectorMatrix Convection(const ScalarSurfaceField phi, VectorVolumeField& variable)
        {
            FvmVectorMatrix tmp_matrix;

            tmp_matrix.sorce_.resize(variable.mesh_->number_of_elements_, GeneralVector(0, 0, 0));
            tmp_matrix.matrix_.SetColumn(variable.mesh_->number_of_elements_);
            tmp_matrix.matrix_.SetRow(variable.mesh_->number_of_elements_);

            TensorVolumeField grad_field(*variable.mesh_, "tmp", "tmp");
            TensorSurfaceField grad_field_f(*variable.mesh_, "tmp", "tmp");

            grad_field = exp::ComputeGradient(variable, ComputeGradientScheme::GAUSS);
            grad_field_f = exp::InterpolateGradient(grad_field, variable, TheInterpolationScheme::AVERAGECORRECTED);

            //内部面组装矩阵  QUICK格式
            for (uint32_t i = 0; i < variable.mesh_->number_of_interior_faces_; i++)
            {
                int owner = variable.mesh_->faces_[i].owner_;
                int neighbour = variable.mesh_->faces_[i].neighbour_;

                //线性化系数
                Scalar fluxc = std::max(phi.value_[i], 0.);
                Scalar fluxf = std::max(-phi.value_[i], 0.);

                point p1 = variable.mesh_->faces_[i].centroid_;
                point p2 = variable.mesh_->elements_[owner].centroid_;
                GeneralVector d_cf(p1 - p2);

                GeneralVector fluxv = 0.5 * phi.value_[i] * ((grad_field.value_[owner] + grad_field_f.value_[i]) & d_cf);

                tmp_matrix.matrix_.InsertValue(owner, owner, fluxc);
                tmp_matrix.matrix_.InsertValue(owner, neighbour, -fluxf);
                tmp_matrix.matrix_.InsertValue(neighbour, neighbour, fluxf);
                tmp_matrix.matrix_.InsertValue(neighbour, owner, -fluxc);

                tmp_matrix.sorce_[owner] = tmp_matrix.sorce_[owner] - fluxv;
            }

            //边界面组装矩阵
            uint32_t number_of_boundaries = variable.mesh_->number_of_boundaries_;

            for (uint32_t i = 0; i < number_of_boundaries; i++)
            {
                uint32_t start_face = variable.mesh_->boundaries_[i].start_face_;
                uint32_t number_of_faces = variable.mesh_->boundaries_[i].number_of_faces_;
                std::string boundary_name = variable.mesh_->boundaries_[i].boundary_name_;
                BoundaryCondationType boundary_condation_type = variable.boundary_condation_[boundary_name].bc_type_;

                switch (boundary_condation_type)
                {
                case zgCFD::BoundaryCondationType::SPECIFIEDVALUE:
                    for (uint32_t i = start_face; i < start_face + number_of_faces; i++)
                    {
                        int owner = variable.mesh_->faces_[i].owner_;
                        uint32_t index = i + variable.mesh_->number_of_elements_ -
                            variable.mesh_->number_of_interior_faces_;

                        GeneralVector fluxv = phi.value_[i] * variable.boundary_condation_[boundary_name].bc_value_;
                        tmp_matrix.sorce_[owner] = tmp_matrix.sorce_[owner] - fluxv;
                    }
                    break;
                case zgCFD::BoundaryCondationType::SPECIFIEDFLUX:
                    break;
                case zgCFD::BoundaryCondationType::ZEROFLUX:
                    break;
                case zgCFD::BoundaryCondationType::ZEROGRADIENT:
                    for (uint32_t i = start_face; i < start_face + number_of_faces; i++)
                    {
                        int owner = variable.mesh_->faces_[i].owner_;
                        uint32_t index = i + variable.mesh_->number_of_elements_ -
                            variable.mesh_->number_of_interior_faces_;

                        Scalar fluxc = phi.value_[i];

                        tmp_matrix.matrix_.InsertValue(owner, owner, fluxc);
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

            return tmp_matrix;
        }

        FvmScalarMatrix Ddt(ScalarVolumeField& variable)
        {
            FvmScalarMatrix tmp_matrix;

            tmp_matrix.sorce_.resize(variable.mesh_->number_of_elements_, 0);
            tmp_matrix.matrix_.SetColumn(variable.mesh_->number_of_elements_);
            tmp_matrix.matrix_.SetRow(variable.mesh_->number_of_elements_);

            for (uint32_t i = 0; i < variable.mesh_->number_of_elements_; i++)
            {
                
                //线性化系数
                Scalar ac_new = variable.mesh_->elements_[i].volume_ / variable.mesh_->delta_t_;
                Scalar ac_old = variable.mesh_->elements_[i].volume_ / variable.mesh_->delta_t_;

                tmp_matrix.matrix_.InsertValue(i, i, ac_new);

                tmp_matrix.sorce_[i] -= ac_old * variable.value_[i];
            }
            return tmp_matrix;
        }

        FvmVectorMatrix Ddt(VectorVolumeField& variable)
        {
            FvmVectorMatrix tmp_matrix;

            tmp_matrix.sorce_.resize(variable.mesh_->number_of_elements_, GeneralVector(0, 0, 0));
            tmp_matrix.matrix_.SetColumn(variable.mesh_->number_of_elements_);
            tmp_matrix.matrix_.SetRow(variable.mesh_->number_of_elements_);

            for (uint32_t i = 0; i < variable.mesh_->number_of_elements_; i++)
            {

                //线性化系数
                Scalar ac_new = variable.mesh_->elements_[i].volume_ / variable.mesh_->delta_t_;
                Scalar ac_old = variable.mesh_->elements_[i].volume_ / variable.mesh_->delta_t_;

                tmp_matrix.matrix_.InsertValue(i, i, ac_new);

                tmp_matrix.sorce_[i] = tmp_matrix.sorce_[i] - ac_old * variable.value_[i];
            }
            return tmp_matrix;
        }

        FvmScalarMatrix Source(ScalarVolumeField& variable)
        {
            FvmScalarMatrix tmp_matrix;

            tmp_matrix.sorce_.resize(variable.mesh_->number_of_elements_, 0);
            tmp_matrix.matrix_.SetColumn(variable.mesh_->number_of_elements_);
            tmp_matrix.matrix_.SetRow(variable.mesh_->number_of_elements_);

            for (uint32_t i = 0; i < variable.mesh_->number_of_elements_; i++)
            {

                //线性化系数
                Scalar fluxv = variable.mesh_->elements_[i].volume_ * variable.value_[i];

                tmp_matrix.sorce_[i] += fluxv;
            }
            return tmp_matrix;
        }

        FvmVectorMatrix Source(VectorVolumeField& variable)
        {
            FvmVectorMatrix tmp_matrix;

            tmp_matrix.sorce_.resize(variable.mesh_->number_of_elements_, GeneralVector(0, 0, 0));
            tmp_matrix.matrix_.SetColumn(variable.mesh_->number_of_elements_);
            tmp_matrix.matrix_.SetRow(variable.mesh_->number_of_elements_);

            for (uint32_t i = 0; i < variable.mesh_->number_of_elements_; i++)
            {

                //线性化系数
                GeneralVector fluxv = variable.mesh_->elements_[i].volume_ * variable.value_[i];

                tmp_matrix.sorce_[i] = tmp_matrix.sorce_[i] + fluxv;
            }
            return tmp_matrix;
        }

    }  // namespace imp
}  // namespace zgCFD
