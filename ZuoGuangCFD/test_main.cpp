#include <iostream>
#include "Eigen/Eigen"
#include "Eigen/IterativeLinearSolvers"
#include "Eigen/Core"

#include <ZuoGuangCFD/finite_volume_method/implicit_calculus.h>
#include <ZuoGuangCFD/solver/solver.h>
#include <ZuoGuangCFD/struct/zg_field.h>
#include <ZuoGuangCFD/mesh/of_mesh.h>
#include "ZuoGuangCFD/struct/fvm_matrix.h"
#include "ZuoGuangCFD/algorithm/piso.h"

using namespace zgCFD;
using namespace Eigen;
using namespace std;

void a(int& b) {
    int* p;
    p = &b;
    *p = 3;
}

void printvm(FvmVectorMatrix &u_eqn) {
    std::vector<Scalar> row;
    std::vector<Scalar> col;
    std::vector<Scalar> value;

    row.resize(u_eqn.matrix_.column_indices_.size(), -1);

    col.assign(u_eqn.matrix_.column_indices_.begin(), u_eqn.matrix_.column_indices_.end());
    value.assign(u_eqn.matrix_.values_.begin(), u_eqn.matrix_.values_.end());

    uint32_t num = 0;
    for (uint32_t i = 0; i < u_eqn.matrix_.row_offfsets_.size() - 1; i++)
    {
        for (uint32_t j = 0; j < u_eqn.matrix_.row_offfsets_[i + 1] - u_eqn.matrix_.row_offfsets_[i]; j++)
        {
            row[num] = i;
            num++;
        }
    }

    std::vector< Eigen::Triplet<Scalar> > triplet_list; //三元组列表
    triplet_list.resize(row.size());

    for (uint32_t i = 0; i < row.size(); i++)
    {
        triplet_list[i] = Eigen::Triplet<Scalar>(row[i], col[i], value[i]);
    }

    Eigen::SparseMatrix <Scalar> eigen_matrix(u_eqn.matrix_.number_of_rows_, u_eqn.matrix_.number_of_columns_);

    eigen_matrix.setFromTriplets(triplet_list.begin(), triplet_list.end());

    cout << eigen_matrix;
    cout << endl;
    for (auto &i:u_eqn.sorce_)
    {
        cout << i.x() << " " << i.y() << " " << i.z() << "\n";
    }
}

void printsm(FvmScalarMatrix& p_eqn) {
    std::vector<Scalar> row;
    std::vector<Scalar> col;
    std::vector<Scalar> value;

    row.resize(p_eqn.matrix_.column_indices_.size(), -1);

    col.assign(p_eqn.matrix_.column_indices_.begin(), p_eqn.matrix_.column_indices_.end());
    value.assign(p_eqn.matrix_.values_.begin(), p_eqn.matrix_.values_.end());

    uint32_t num = 0;
    for (uint32_t i = 0; i < p_eqn.matrix_.row_offfsets_.size() - 1; i++)
    {
        for (uint32_t j = 0; j < p_eqn.matrix_.row_offfsets_[i + 1] - p_eqn.matrix_.row_offfsets_[i]; j++)
        {
            row[num] = i;
            num++;
        }
    }

    std::vector< Eigen::Triplet<Scalar> > triplet_list; //三元组列表
    triplet_list.resize(row.size());

    for (uint32_t i = 0; i < row.size(); i++)
    {
        triplet_list[i] = Eigen::Triplet<Scalar>(row[i], col[i], value[i]);
    }

    Eigen::SparseMatrix <Scalar> eigen_matrix(p_eqn.matrix_.number_of_rows_, p_eqn.matrix_.number_of_columns_);

    eigen_matrix.setFromTriplets(triplet_list.begin(), triplet_list.end());

    cout << eigen_matrix;
    cout << endl;
    for (auto& i : p_eqn.sorce_)
    {
        cout << i<< "\n";
    }
}

void printVector(std::vector<int>& v) {

    for (std::vector<int>::iterator it = v.begin(); it != v.end(); it++) {
        cout << *it << " ";
    }
    cout <<endl;
}

void test1() {
    zgCFD::ScalarVolumeField* p_;
    zgCFD::VectorVolumeField* v_;
    zgCFD::Mesh* mesh_;

    string base_path = "F:\\027.CFDsoftware\\test\\testCase\\constant\\polyMesh";
    mesh_ = new zgCFD::OfMesh(base_path);

    p_ = new ScalarVolumeField(*mesh_, "p", "F:\\027.CFDsoftware\\test\\testCase\\0", "no", "yes");
    v_ = new VectorVolumeField(*mesh_, "U", "F:\\027.CFDsoftware\\test\\testCase\\0", "no", "yes");

    v_->boundary_condation_["movingWall"].bc_value_.x() = 2;
    v_->boundary_condation_["movingWall"].bc_value_.y() = 4;
    v_->boundary_condation_["movingWall"].bc_value_.z() = 0;
    v_->boundary_condation_["movingWall"].bc_type_ = BoundaryCondationType::SPECIFIEDVALUE;

    v_->boundary_condation_["fixedWalls"].bc_value_.x() = 2;
    v_->boundary_condation_["fixedWalls"].bc_value_.y() = 4;
    v_->boundary_condation_["fixedWalls"].bc_value_.z() = 0;
    v_->boundary_condation_["fixedWalls"].bc_type_ = BoundaryCondationType::SPECIFIEDVALUE;

    /*v_->boundary_condation_["fixedWalls"].bc_type_ = BoundaryCondationType::ZEROGRADIENT;*/

    v_->boundary_condation_["frontAndBack"].bc_type_ = BoundaryCondationType::EMPTY;
    v_->AdaptToBondary();


    p_->boundary_condation_["movingWall"].bc_type_ = BoundaryCondationType::ZEROGRADIENT;
    p_->boundary_condation_["fixedWalls"].bc_type_ = BoundaryCondationType::ZEROGRADIENT;
    p_->boundary_condation_["frontAndBack"].bc_type_ = BoundaryCondationType::EMPTY;
    p_->boundary_condation_["movingWall"].bc_type_ = BoundaryCondationType::SPECIFIEDVALUE;
    p_->boundary_condation_["movingWall"].bc_value_ = 18;
    p_->boundary_condation_["fixedWalls"].bc_type_ = BoundaryCondationType::SPECIFIEDVALUE;
    p_->boundary_condation_["fixedWalls"].bc_value_ = 20;
    p_->boundary_condation_["frontAndBack"].bc_type_ = BoundaryCondationType::EMPTY;
    p_->AdaptToBondary();

    VectorSurfaceField sf(*(v_->mesh_), "tmp", "tmp");
    for (uint32_t i = 0; i < v_->mesh_->number_of_interior_faces_; i++)
    {
        sf.interior_value_[i] = v_->mesh_->faces_[i].sf_;
    }
    for (uint32_t i = v_->mesh_->number_of_interior_faces_; i < v_->mesh_->number_of_faces_; i++)
    {
        uint32_t index = i - v_->mesh_->number_of_interior_faces_;
        sf.boundary_value_[index] = v_->mesh_->faces_[i].sf_;
    }

    ScalarSurfaceField flux = (exp::Interpolate(*(v_), TheInterpolationScheme::AVERAGE) & sf);
    VectorSurfaceField vf = exp::Interpolate(*(v_), TheInterpolationScheme::AVERAGE);

    v_->mesh_->delta_t_ = 0.01;

    FvmVectorMatrix u_eqn;
    u_eqn.SetVarible(v_);

    Scalar nu = 0.01;
    //u_eqn = imp::Diffusion(nu, *v_);

    u_eqn = ((imp::Ddt(*v_) + imp::Convection(flux, *v_)) - imp::Diffusion(nu, *v_));

    std::vector<Scalar> row;
    std::vector<Scalar> col;
    std::vector<Scalar> value;

    row.resize(u_eqn.matrix_.column_indices_.size(), -1);

    col.assign(u_eqn.matrix_.column_indices_.begin(), u_eqn.matrix_.column_indices_.end());
    value.assign(u_eqn.matrix_.values_.begin(), u_eqn.matrix_.values_.end());

    uint32_t num = 0;
    for (uint32_t i = 0; i < u_eqn.matrix_.row_offfsets_.size() - 1; i++)
    {
        for (uint32_t j = 0; j < u_eqn.matrix_.row_offfsets_[i + 1] - u_eqn.matrix_.row_offfsets_[i]; j++)
        {
            row[num] = i;
            num++;
        }
    }

    std::vector< Eigen::Triplet<Scalar> > triplet_list; //三元组列表
    triplet_list.resize(row.size());

    for (uint32_t i = 0; i < row.size(); i++)
    {
        triplet_list[i] = Eigen::Triplet<Scalar>(row[i], col[i], value[i]);
    }

    Eigen::SparseMatrix <Scalar> eigen_matrix(u_eqn.matrix_.number_of_rows_, u_eqn.matrix_.number_of_columns_);

    eigen_matrix.setFromTriplets(triplet_list.begin(), triplet_list.end());

    cout << eigen_matrix;

    FvmScalarMatrix p_eqn;
    p_eqn.SetVarible(p_);

    p_eqn = imp::Diffusion(nu, *p_);

    std::vector<Scalar> rowp;
    std::vector<Scalar> colp;
    std::vector<Scalar> valuep;

    rowp.resize(p_eqn.matrix_.column_indices_.size(), -1);

    colp.assign(p_eqn.matrix_.column_indices_.begin(), p_eqn.matrix_.column_indices_.end());
    valuep.assign(p_eqn.matrix_.values_.begin(), p_eqn.matrix_.values_.end());

    uint32_t nump = 0;
    for (uint32_t i = 0; i < p_eqn.matrix_.row_offfsets_.size() - 1; i++)
    {
        for (uint32_t j = 0; j < p_eqn.matrix_.row_offfsets_[i + 1] - p_eqn.matrix_.row_offfsets_[i]; j++)
        {
            rowp[nump] = i;
            nump++;
        }
    }

    std::vector< Eigen::Triplet<Scalar> > triplet_listp; //三元组列表
    triplet_listp.resize(rowp.size());

    for (uint32_t i = 0; i < rowp.size(); i++)
    {
        triplet_listp[i] = Eigen::Triplet<Scalar>(rowp[i], colp[i], valuep[i]);
    }

    Eigen::SparseMatrix <Scalar> eigen_matrixp(p_eqn.matrix_.number_of_rows_, p_eqn.matrix_.number_of_columns_);

    eigen_matrixp.setFromTriplets(triplet_listp.begin(), triplet_listp.end());

    cout << eigen_matrixp;

    //Piso piso(0, 0.01, 1, 0.01, 2, 0.002, v_, p_);

    //piso.run();}
}


void test2() {
    zgCFD::ScalarVolumeField* p_;
    zgCFD::VectorVolumeField* v_;
    zgCFD::Mesh* mesh_;

    string base_path = "F:\\027.CFDsoftware\\test\\testCase\\constant\\polyMesh";
    mesh_ = new zgCFD::OfMesh(base_path);

    p_ = new ScalarVolumeField(*mesh_, "p", "F:\\027.CFDsoftware\\test\\testCase\\0", "no", "yes");
    v_ = new VectorVolumeField(*mesh_, "U", "F:\\027.CFDsoftware\\test\\testCase\\0", "no", "yes");

    v_->boundary_condation_["movingWall"].bc_value_.x() = 2;
    v_->boundary_condation_["movingWall"].bc_value_.y() = 4;
    v_->boundary_condation_["movingWall"].bc_value_.z() = 0;
    v_->boundary_condation_["movingWall"].bc_type_ = BoundaryCondationType::SPECIFIEDVALUE;

    //v_->boundary_condation_["fixedWalls"].bc_value_.x() = 2;
    //v_->boundary_condation_["fixedWalls"].bc_value_.y() = 4;
    //v_->boundary_condation_["fixedWalls"].bc_value_.z() = 0;
    v_->boundary_condation_["fixedWalls"].bc_type_ = BoundaryCondationType::ZEROGRADIENT;

    /*v_->boundary_condation_["fixedWalls"].bc_type_ = BoundaryCondationType::ZEROGRADIENT;*/

    v_->boundary_condation_["frontAndBack"].bc_type_ = BoundaryCondationType::EMPTY;
    v_->AdaptToBondary();


    p_->boundary_condation_["movingWall"].bc_type_ = BoundaryCondationType::ZEROGRADIENT;
    p_->boundary_condation_["fixedWalls"].bc_type_ = BoundaryCondationType::ZEROGRADIENT;
    p_->boundary_condation_["frontAndBack"].bc_type_ = BoundaryCondationType::EMPTY;
    p_->boundary_condation_["movingWall"].bc_type_ = BoundaryCondationType::SPECIFIEDVALUE;
    p_->boundary_condation_["movingWall"].bc_value_ = 18;
    p_->boundary_condation_["fixedWalls"].bc_type_ = BoundaryCondationType::SPECIFIEDVALUE;
    p_->boundary_condation_["fixedWalls"].bc_value_ = 20;
    p_->boundary_condation_["frontAndBack"].bc_type_ = BoundaryCondationType::EMPTY;
    p_->AdaptToBondary();

    v_->mesh_->delta_t_ = 0.01;

    std::cout << "计算开始" << std::endl;

    VectorSurfaceField sf(*(v_->mesh_), "tmp", "tmp");
    for (uint32_t i = 0; i < v_->mesh_->number_of_interior_faces_; i++)
    {
        sf.interior_value_[i] = v_->mesh_->faces_[i].sf_;
    }
    for (uint32_t i = v_->mesh_->number_of_interior_faces_; i < v_->mesh_->number_of_faces_; i++)
    {
        uint32_t index = i - v_->mesh_->number_of_interior_faces_;
        sf.boundary_value_[index] = v_->mesh_->faces_[i].sf_;
    }

    ScalarSurfaceField flux = (exp::Interpolate(*(v_), TheInterpolationScheme::AVERAGE) & sf);

    uint32_t count = 0;
    while (true)
    {
        FvmVectorMatrix u_eqn;
        u_eqn.SetVarible(v_);

        Scalar nu_ = 0.01;

        u_eqn = (
            (imp::Ddt(*v_) + imp::Convection(flux, *v_)) - imp::Diffusion(nu_, *v_)
            );
        //Solver().Solve(u_eqn);

        for (uint32_t i = 0; i < 2; i++)
        {

            ScalarVolumeField rau(*(v_->mesh_), "tmp", "tmp");

            for (uint32_t i = 0; i < v_->mesh_->number_of_elements_; i++)
            {
                //rau.interior_value_[i] = v_->mesh_->elements_[i].volume_ / u_eqn.matrix_.GetValue(i, i);
                rau.interior_value_[i] = 1 / u_eqn.matrix_.GetValue(i, i);
            }

            VectorVolumeField HbyA(*(v_->mesh_), "tmp", "tmp");
            VectorVolumeField H(*(v_->mesh_), "tmp", "tmp");

            for (uint32_t i = 0; i < v_->mesh_->number_of_elements_; i++)
            {
                for (uint32_t j = u_eqn.matrix_.row_offfsets_[i]; j < u_eqn.matrix_.row_offfsets_[i + 1]; j++)
                {
                    uint32_t column = u_eqn.matrix_.column_indices_[j];
                    if (i != column)
                    {
                        H.interior_value_[i] = H.interior_value_[i] + u_eqn.matrix_.GetValue(i, column) * v_->interior_value_[column];
                    }                    
                }
                H.interior_value_[i] = u_eqn.sorce_[i] - H.interior_value_[i];
                HbyA.interior_value_[i] = H.interior_value_[i] * rau.interior_value_[i];
            }

            for (uint32_t i = HbyA.mesh_->number_of_interior_faces_; i < HbyA.mesh_->number_of_faces_; i++)
            {
                /*uint32_t index = i - HbyA.mesh_->number_of_interior_faces_;
                HbyA.boundary_value_[index] = */
            }

            uint32_t number_of_boundaries = HbyA.mesh_->number_of_boundaries_;

            for (uint32_t i = 0; i < number_of_boundaries; i++)
            {
                uint32_t start_face = HbyA.mesh_->boundaries_[i].start_face_;
                uint32_t number_of_faces = HbyA.mesh_->boundaries_[i].number_of_faces_;
                std::string boundary_name = HbyA.mesh_->boundaries_[i].boundary_name_;
                BoundaryCondationType boundary_condation_type = v_->boundary_condation_[boundary_name].bc_type_;
                HbyA.boundary_condation_[boundary_name].bc_type_ = BoundaryCondationType::CALCULATED;

                switch (boundary_condation_type)
                {
                case zgCFD::BoundaryCondationType::SPECIFIEDVALUE:
                    for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                    {
                        uint32_t index = j - HbyA.mesh_->number_of_interior_faces_;
                        HbyA.boundary_value_[index] = v_->boundary_condation_[boundary_name].bc_value_;
                    }
                    break;
                case zgCFD::BoundaryCondationType::SPECIFIEDFLUX:
                    break;
                case zgCFD::BoundaryCondationType::ZEROFLUX:
                    break;
                case zgCFD::BoundaryCondationType::ZEROGRADIENT:
                    for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                    {
                        int owner = HbyA.mesh_->faces_[j].owner_;
                        uint32_t index = j - HbyA.mesh_->number_of_interior_faces_;

                        HbyA.boundary_value_[index] = v_->interior_value_[owner];
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
                    for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                    {
                        int owner = HbyA.mesh_->faces_[j].owner_;
                        uint32_t index = j - HbyA.mesh_->number_of_interior_faces_;

                        HbyA.boundary_value_[index] = GeneralVector(0, 0, 0);
                    }
                    break;
                default:
                    break;
                }
            }

            ScalarSurfaceField phi_HbyA(*(v_->mesh_), "tmp", "tmp");

            phi_HbyA = (exp::Interpolate(HbyA, TheInterpolationScheme::AVERAGE) & sf);

            FvmScalarMatrix p_eqn;
            p_eqn.SetVarible(p_);                       

            VectorVolumeField gradp = exp::ComputeGradient(*p_, ComputeGradientScheme::GAUSS);

            ScalarVolumeField c_phi_HbyA(*(v_->mesh_), "tmp", "tmp");

            c_phi_HbyA = exp::Convection(phi_HbyA);

            for (uint32_t i = 0; i < c_phi_HbyA.interior_value_.size(); i++)
            {
                c_phi_HbyA.interior_value_[i] = c_phi_HbyA.interior_value_[i] / c_phi_HbyA.mesh_->elements_[i].volume_;
            }

            p_eqn = (
                imp::Source(c_phi_HbyA) - imp::Diffusion(nu_, *p_)
                );

            std::vector<Scalar> rowp;
            std::vector<Scalar> colp;
            std::vector<Scalar> valuep;

            rowp.resize(p_eqn.matrix_.column_indices_.size(), -1);

            colp.assign(p_eqn.matrix_.column_indices_.begin(), p_eqn.matrix_.column_indices_.end());
            valuep.assign(p_eqn.matrix_.values_.begin(), p_eqn.matrix_.values_.end());

            uint32_t nump = 0;
            for (uint32_t i = 0; i < p_eqn.matrix_.row_offfsets_.size() - 1; i++)
            {
                for (uint32_t j = 0; j < p_eqn.matrix_.row_offfsets_[i + 1] - p_eqn.matrix_.row_offfsets_[i]; j++)
                {
                    rowp[nump] = i;
                    nump++;
                }
            }

            std::vector< Eigen::Triplet<Scalar> > triplet_listp; //三元组列表
            triplet_listp.resize(rowp.size());

            for (uint32_t i = 0; i < rowp.size(); i++)
            {
                triplet_listp[i] = Eigen::Triplet<Scalar>(rowp[i], colp[i], valuep[i]);
            }

            Eigen::SparseMatrix <Scalar> eigen_matrixp(p_eqn.matrix_.number_of_rows_, p_eqn.matrix_.number_of_columns_);

            eigen_matrixp.setFromTriplets(triplet_listp.begin(), triplet_listp.end());

            cout << eigen_matrixp;

            Solver().Solve(p_eqn);

            VectorVolumeField grad_p = exp::ComputeGradient(*p_, ComputeGradientScheme::GAUSS);

            VectorVolumeField u_new = (HbyA - (rau * grad_p));

            v_->interior_value_old_old_.assign(v_->interior_value_old_.begin(), v_->interior_value_old_.end());
            v_->interior_value_old_.assign(v_->interior_value_.begin(), v_->interior_value_.end());

            v_->interior_value_.assign(u_new.interior_value_.begin(), u_new.interior_value_.end());
            v_->AdaptToBondary();
            flux = (exp::Interpolate(*(v_), TheInterpolationScheme::AVERAGE) & sf);
        }
    }
}

void test3() {

    zgCFD::ScalarVolumeField* p_;
    zgCFD::VectorVolumeField* v_;
    zgCFD::Mesh* mesh_;

    string base_path = "F:\\027.CFDsoftware\\test\\testCase\\constant\\polyMesh";
    mesh_ = new zgCFD::OfMesh(base_path);

    p_ = new ScalarVolumeField(*mesh_, "p", "F:\\027.CFDsoftware\\test\\testCase\\0", "no", "yes");
    v_ = new VectorVolumeField(*mesh_, "U", "F:\\027.CFDsoftware\\test\\testCase\\0", "no", "yes");

    v_->boundary_condation_["movingWall"].bc_value_.x() = 2;
    v_->boundary_condation_["movingWall"].bc_value_.y() = 4;
    v_->boundary_condation_["movingWall"].bc_value_.z() = 0;
    v_->boundary_condation_["movingWall"].bc_type_ = BoundaryCondationType::SPECIFIEDVALUE;

    v_->boundary_condation_["fixedWalls"].bc_value_.x() = 2;
    v_->boundary_condation_["fixedWalls"].bc_value_.y() = 4;
    v_->boundary_condation_["fixedWalls"].bc_value_.z() = 0;
    v_->boundary_condation_["fixedWalls"].bc_type_ = BoundaryCondationType::SPECIFIEDVALUE;

    /*v_->boundary_condation_["fixedWalls"].bc_type_ = BoundaryCondationType::ZEROGRADIENT;*/

    v_->boundary_condation_["frontAndBack"].bc_type_ = BoundaryCondationType::EMPTY;
    v_->AdaptToBondary();


    p_->boundary_condation_["movingWall"].bc_type_ = BoundaryCondationType::ZEROGRADIENT;
    p_->boundary_condation_["fixedWalls"].bc_type_ = BoundaryCondationType::ZEROGRADIENT;
    p_->boundary_condation_["frontAndBack"].bc_type_ = BoundaryCondationType::EMPTY;
    p_->boundary_condation_["movingWall"].bc_type_ = BoundaryCondationType::SPECIFIEDVALUE;
    p_->boundary_condation_["movingWall"].bc_value_ = 18;
    p_->boundary_condation_["fixedWalls"].bc_type_ = BoundaryCondationType::SPECIFIEDVALUE;
    p_->boundary_condation_["fixedWalls"].bc_value_ = 20;
    p_->boundary_condation_["frontAndBack"].bc_type_ = BoundaryCondationType::EMPTY;
    p_->AdaptToBondary();

    Piso piso(0, 0.01, 1, 0.01, 2, 0.002, v_, p_);

    piso.run();
}

void test4() {
    zgCFD::ScalarVolumeField* p_;
    zgCFD::VectorVolumeField* v_;
    zgCFD::Mesh* mesh_;

    string base_path = "F:\\027.CFDsoftware\\test\\testCase\\constant\\polyMesh";
    mesh_ = new zgCFD::OfMesh(base_path);

    p_ = new ScalarVolumeField(*mesh_, "p", "F:\\027.CFDsoftware\\test\\testCase\\0", "no", "yes");
    v_ = new VectorVolumeField(*mesh_, "U", "F:\\027.CFDsoftware\\test\\testCase\\0", "no", "yes");

    v_->boundary_condation_["movingWall"].bc_value_.x() = 2;
    v_->boundary_condation_["movingWall"].bc_value_.y() = 4;
    v_->boundary_condation_["movingWall"].bc_value_.z() = 0;
    v_->boundary_condation_["movingWall"].bc_type_ = BoundaryCondationType::SPECIFIEDVALUE;

    //v_->boundary_condation_["fixedWalls"].bc_value_.x() = 2;
    //v_->boundary_condation_["fixedWalls"].bc_value_.y() = 4;
    //v_->boundary_condation_["fixedWalls"].bc_value_.z() = 0;
    //v_->boundary_condation_["fixedWalls"].bc_type_ = BoundaryCondationType::SPECIFIEDVALUE;

    v_->boundary_condation_["fixedWalls"].bc_type_ = BoundaryCondationType::ZEROGRADIENT;

    v_->boundary_condation_["frontAndBack"].bc_type_ = BoundaryCondationType::EMPTY;
    v_->AdaptToBondary();

    p_->boundary_condation_["movingWall"].bc_type_ = BoundaryCondationType::SPECIFIEDVALUE;
    p_->boundary_condation_["movingWall"].bc_value_ = 18;
    p_->boundary_condation_["fixedWalls"].bc_type_ = BoundaryCondationType::SPECIFIEDVALUE;
    p_->boundary_condation_["fixedWalls"].bc_value_ = 20;
    p_->boundary_condation_["frontAndBack"].bc_type_ = BoundaryCondationType::EMPTY;
    p_->AdaptToBondary();

    VectorSurfaceField sf(*(v_->mesh_), "tmp", "tmp");
    for (uint32_t i = 0; i < v_->mesh_->number_of_interior_faces_; i++)
    {
        sf.interior_value_[i] = v_->mesh_->faces_[i].sf_;
    }
    for (uint32_t i = v_->mesh_->number_of_interior_faces_; i < v_->mesh_->number_of_faces_; i++)
    {
        uint32_t index = i - v_->mesh_->number_of_interior_faces_;
        sf.boundary_value_[index] = v_->mesh_->faces_[i].sf_;
    }

    ScalarSurfaceField flux = (exp::Interpolate(*(v_), TheInterpolationScheme::AVERAGE) & sf);
    VectorSurfaceField vf = exp::Interpolate(*(v_), TheInterpolationScheme::AVERAGE);

    v_->mesh_->delta_t_ = 0.01;

    //测试fvm的各项
    FvmVectorMatrix u_eqn;
    u_eqn.SetVarible(v_);

    Scalar nu = 0.01;

    u_eqn = imp::Ddt(*v_);
    printvm(u_eqn);

    u_eqn = imp::Convection(flux, *v_);
    printvm(u_eqn);

    u_eqn = imp::Diffusion(nu, *v_);
    printvm(u_eqn);

    u_eqn = ((imp::Ddt(*v_) + imp::Convection(flux, *v_)) - imp::Diffusion(nu, *v_));
    printvm(u_eqn);

    ScalarVolumeField rau(*(v_->mesh_), "tmp", "tmp");

    uint32_t number_of_boundaries = rau.mesh_->number_of_boundaries_;

    for (uint32_t i = 0; i < number_of_boundaries; i++)
    {
        std::string boundary_name = rau.mesh_->boundaries_[i].boundary_name_;
        BoundaryCondationType boundary_condation_type = v_->boundary_condation_[boundary_name].bc_type_;
        if (boundary_condation_type == BoundaryCondationType::EMPTY) {
        rau.boundary_condation_[boundary_name].bc_type_ = BoundaryCondationType::EMPTY;
        }
    }

    for (uint32_t i = 0; i < v_->mesh_->number_of_elements_; i++)
    {
        //rau.interior_value_[i] = v_->mesh_->elements_[i].volume_ / u_eqn.matrix_.GetValue(i, i);
        rau.interior_value_[i] = 1 / u_eqn.matrix_.GetValue(i, i);
    }

    rau.AdaptToBondary();

    VectorVolumeField HbyA(*(v_->mesh_), "tmp", "tmp");
    VectorVolumeField H(*(v_->mesh_), "tmp", "tmp");

    for (uint32_t i = 0; i < v_->mesh_->number_of_elements_; i++)
    {
        for (uint32_t j = u_eqn.matrix_.row_offfsets_[i]; j < u_eqn.matrix_.row_offfsets_[i + 1]; j++)
        {
            uint32_t column = u_eqn.matrix_.column_indices_[j];
            if (i != column)
            {
                H.interior_value_[i] = H.interior_value_[i] + u_eqn.matrix_.GetValue(i, column) * v_->interior_value_[column];
            }
        }
        H.interior_value_[i] = u_eqn.sorce_[i] - H.interior_value_[i];
        HbyA.interior_value_[i] = H.interior_value_[i] * rau.interior_value_[i];
    }
    HbyA.boundary_condation_ = v_->boundary_condation_;
    HbyA.AdaptToBondary();

    ScalarSurfaceField phi_HbyA(*(v_->mesh_), "tmp", "tmp");

    phi_HbyA = (exp::Interpolate(HbyA, TheInterpolationScheme::AVERAGE) & sf);

    FvmScalarMatrix p_eqn;
    p_eqn.SetVarible(p_);

    ScalarVolumeField c_phi_HbyA(*(v_->mesh_), "tmp", "tmp");

    c_phi_HbyA = exp::Convection(phi_HbyA);

    for (uint32_t i = 0; i < c_phi_HbyA.interior_value_.size(); i++)
    {
        c_phi_HbyA.interior_value_[i] =
            c_phi_HbyA.interior_value_[i] / c_phi_HbyA.mesh_->elements_[i].volume_;
    }

    VectorVolumeField grad_variable = exp::ComputeGradient(*p_, ComputeGradientScheme::GAUSS);
    VectorSurfaceField grad_variable_f =
        exp::InterpolateGradient(grad_variable, *p_, TheInterpolationScheme::AVERAGE);

    ScalarSurfaceField gamma_f = exp::Interpolate(rau, TheInterpolationScheme::AVERAGE);

    p_eqn = imp::Diffusion(rau, *p_);
    printsm(p_eqn);

    p_eqn = imp::Source(c_phi_HbyA);
    printsm(p_eqn);

    p_eqn = (imp::Diffusion(nu, *p_) - imp::Source(c_phi_HbyA));
    printsm(p_eqn);
    Solver().Solve(p_eqn);

}

int main() {  

    test4();

    return 0;

}

