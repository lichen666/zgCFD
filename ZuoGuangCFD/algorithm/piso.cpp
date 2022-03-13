#include "ZuoGuangCFD/algorithm/piso.h"
namespace zgCFD
{
    Piso::Piso(const Scalar& start_time, const Scalar& delta_t, const Scalar& end_time, const Scalar& output_time,
               const uint32_t& number_of_correctors, const Scalar& nu, VectorVolumeField* v, ScalarVolumeField* p)
    {
        this->start_time_ = start_time;
        this->delta_t_ = delta_t;
        this->end_time_ = end_time;
        this->number_of_correctors = number_of_correctors;
        this->nu_ = nu;
        this->v_ = v;
        this->p_ = p;
        this->current_time_ = start_time;
        this->output_time_ = output_time;
        this->current_output_time_ = output_time;
    }
    Piso::~Piso() {}
    void Piso::run()
    {
        std::cout << "计算开始" << std::endl;

        VectorSurfaceField sf(*(this->v_->mesh_), "tmp", "tmp");
        for (uint32_t i = 0; i < this->v_->mesh_->number_of_interior_faces_; i++)
        {
            sf.interior_value_[i] = this->v_->mesh_->faces_[i].sf_;
        }
        for (uint32_t i = this->v_->mesh_->number_of_interior_faces_; i < this->v_->mesh_->number_of_faces_; i++)
        {
            uint32_t index = i - this->v_->mesh_->number_of_interior_faces_;
            sf.boundary_value_[index] = this->v_->mesh_->faces_[i].sf_;
        }

        ScalarSurfaceField flux = (exp::Interpolate(*(this->v_), TheInterpolationScheme::AVERAGE) & sf);

        uint32_t count = 0;
        while (this->IsRunnig())
        {
            this->current_time_ += this->delta_t_;
            v_->mesh_->delta_t_ = this->delta_t_;
            std::cout << "时刻：" << this->current_time_ << std::endl;

            FvmVectorMatrix u_eqn;
            u_eqn.SetVarible(this->v_);

            u_eqn = ((imp::Ddt(*v_) + imp::Convection(flux, *v_)) - imp::Diffusion(nu_, *v_));
            // Solver().Solve(u_eqn);

            for (uint32_t i = 0; i < number_of_correctors; i++)
            {
                ScalarVolumeField rau(*(this->v_->mesh_), "tmp", "tmp");

                uint32_t number_of_boundaries = rau.mesh_->number_of_boundaries_;

                for (uint32_t i = 0; i < number_of_boundaries; i++)
                {
                    std::string boundary_name = rau.mesh_->boundaries_[i].boundary_name_;
                    BoundaryCondationType boundary_condation_type = v_->boundary_condation_[boundary_name].bc_type_;
                    if (boundary_condation_type == BoundaryCondationType::EMPTY)
                    {
                        rau.boundary_condation_[boundary_name].bc_type_ = BoundaryCondationType::EMPTY;
                    }
                }

                for (uint32_t i = 0; i < this->v_->mesh_->number_of_elements_; i++)
                {
                    rau.interior_value_[i] = 1 / u_eqn.matrix_.GetValue(i, i);
                }

                rau.AdaptToBondary();

                VectorVolumeField HbyA(*(this->v_->mesh_), "tmp", "tmp");
                VectorVolumeField H(*(this->v_->mesh_), "tmp", "tmp");

                for (uint32_t i = 0; i < this->v_->mesh_->number_of_elements_; i++)
                {
                    for (size_t j = u_eqn.matrix_.row_offfsets_[i]; j < u_eqn.matrix_.row_offfsets_[i + 1]; j++)
                    {
                        uint32_t column = u_eqn.matrix_.column_indices_[j];
                        if (i != column)
                        {
                            H.interior_value_[i] = H.interior_value_[i] + u_eqn.matrix_.GetValue(i, column) *
                                                                              this->v_->interior_value_[column];
                        }
                    }
                    H.interior_value_[i] = u_eqn.sorce_[i] - H.interior_value_[i];
                    HbyA.interior_value_[i] = H.interior_value_[i] * rau.interior_value_[i];
                }

                HbyA.boundary_condation_ = v_->boundary_condation_;
                HbyA.AdaptToBondary();

                ScalarSurfaceField phi_HbyA(*(this->v_->mesh_), "tmp", "tmp");

                phi_HbyA = (exp::Interpolate(HbyA, TheInterpolationScheme::AVERAGE) & sf);

                FvmScalarMatrix p_eqn;
                p_eqn.SetVarible(this->p_);

                ScalarVolumeField c_phi_HbyA(*(this->v_->mesh_), "tmp", "tmp");

                c_phi_HbyA = exp::Convection(phi_HbyA);

                for (uint32_t i = 0; i < c_phi_HbyA.interior_value_.size(); i++)
                {
                    c_phi_HbyA.interior_value_[i] =
                        c_phi_HbyA.interior_value_[i] / c_phi_HbyA.mesh_->elements_[i].volume_;
                }

                p_eqn = (imp::Diffusion(rau, *p_) - imp::Source(c_phi_HbyA));

                Solver().Solve(p_eqn);

                VectorVolumeField grad_p = exp::ComputeGradient(*p_, ComputeGradientScheme::GAUSS);

                VectorVolumeField u_new = (HbyA - (rau * grad_p));

                v_->interior_value_old_old_.assign(v_->interior_value_old_.begin(), v_->interior_value_old_.end());
                v_->interior_value_old_.assign(v_->interior_value_.begin(), v_->interior_value_.end());

                v_->interior_value_.assign(u_new.interior_value_.begin(), u_new.interior_value_.end());
                v_->AdaptToBondary();
                flux = (exp::Interpolate(*(this->v_), TheInterpolationScheme::AVERAGE) & sf);
            }

            if (current_time_ > current_output_time_)
            {
                std::string name = v_->mesh_->case_directory_ + "\\out." + std::to_string(count) + ".vtu";
                count++;
                current_output_time_ += output_time_;

                OutputVTU outvtk(name, p_, v_, p_->mesh_);
                outvtk.Write();
                count++;
                std::string path = v_->mesh_->case_directory_ + "\\..\\..\\" + std::to_string(count);
                _mkdir(const_cast<char*>(path.c_str()));

                OutPutOf outof(path, std::to_string(current_output_time_), p_, v_, p_->mesh_);
                outof.Write();

                current_output_time_ += output_time_;
            }
        }
    }
    bool Piso::IsRunnig()
    {
        if (this->current_time_ < this->end_time_)
        {
            return true;
        }
        return false;
    }
}  // namespace zgCFD
