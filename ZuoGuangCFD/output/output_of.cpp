#include "ZuoGuangCFD/output/output_of.h"

namespace zgCFD {
	OutPutOf::OutPutOf(const std::string& file_path, const std::string& file_time, ScalarVolumeField* p, VectorVolumeField* v, Mesh* mesh)
	{
		this->file_path_ = file_path;
		this->p_ = p;
		this->v_ = v;
		this->mesh_ = mesh;
        this->file_time_ = file_time;

        p_head = R"(FoamFile 
{
    version     2.0;
    format      ascii;
    arch        "LSB;label=32;scalar=64";
    class       volScalarField;
    location    ")" + file_time_ + R"(";
    object      p;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

dimensions[0 2 - 2 0 0 0 0];

internalField   nonuniform List<scalar>
)";

        v_head = R"(FoamFile 
{
    version     2.0;
    format      ascii;
    arch        "LSB;label=32;scalar=64";
    class       volVectorField;
    location    ")" + file_time_ + R"(";
    object      U;
}
// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

dimensions[0 1 -1 0 0 0 0];

internalField   nonuniform List<vector>  
)";
	}
	void OutPutOf::Write()
	{
		WriteFile(file_path_);
	}
	bool OutPutOf::WriteFile(const std::string& file_path)
	{
        out_file_p_.open(file_path + "\\p", std::ios::out);
        if (!out_file_p_.is_open())
        {
            return false;
        }
        out_file_v_.open(file_path + "\\U", std::ios::out);
        if (!out_file_v_.is_open())
        {
            return false;
        }
        {
            WriteHead("p");

            std::stringstream tmp_string;
            tmp_string << mesh_->number_of_elements_ << "\n";
            tmp_string << "(\n";
            out_file_p_ << tmp_string.str();

            WriteP();
            out_file_p_ << ")\n";
            out_file_p_ << ";\n";
            out_file_p_ << "boundaryField\n";
            out_file_p_ << "{\n";
            WritePBoundary();
            out_file_p_ << "}";
        }
        {
            WriteHead("v");

            std::stringstream tmp_string;
            tmp_string << mesh_->number_of_elements_ << "\n";
            tmp_string << "(\n";
            out_file_v_ << tmp_string.str();

            WriteV();
            out_file_v_ << ")\n";
            out_file_v_ << ";\n";
            out_file_v_ << "boundaryField\n";
            out_file_v_ << "{\n";
            WriteVBoundary(); 
            out_file_v_ << "}";
        }

        return true;
	}
    void OutPutOf::WriteP()
    {
        for (uint32_t i = 0; i < mesh_->number_of_elements_; i++)
        {
            out_file_p_ << p_->interior_value_[i] << "\n";
        }
    }
    void OutPutOf::WriteV()
    {
        for (uint32_t i = 0; i < mesh_->number_of_elements_; i++)
        {
            out_file_v_ << "("<<v_->interior_value_[i].x()<<" " << v_->interior_value_[i].y() << " " << v_->interior_value_[i].z() << ")" << "\n";
        }
    }
    void OutPutOf::WriteVBoundary()
    {
        for (uint32_t i = 0; i < mesh_->number_of_boundaries_; i++)
        {
            std::string boundary_name = mesh_->boundaries_[i].boundary_name_;
            BoundaryCondationType boundary_condation_type = v_->boundary_condation_[boundary_name].bc_type_;

            switch (boundary_condation_type)
            {
            case zgCFD::BoundaryCondationType::SPECIFIEDVALUE:
                out_file_v_ << "\t" << boundary_name << "\n";
                out_file_v_ << "\t{\n";
                out_file_v_ << "\t\ttype\t\tfixedValue;\n";
                out_file_v_ << "\t\tvalue\t\tuniform (" << v_->boundary_condation_[boundary_name].bc_value_.x() << " " << v_->boundary_condation_[boundary_name].bc_value_.y() << " " << v_->boundary_condation_[boundary_name].bc_value_.z() << ")" << ";\n\t}\n";
                break;
            case zgCFD::BoundaryCondationType::SPECIFIEDFLUX:
                break;
            case zgCFD::BoundaryCondationType::ZEROFLUX:
                break;
            case zgCFD::BoundaryCondationType::ZEROGRADIENT:
                out_file_v_ << "\t" << boundary_name << "\n";
                out_file_v_ << "\t{\n";
                out_file_v_ << "\t\ttype\tzeroGradient;\n\t}\n";
                break;
            case zgCFD::BoundaryCondationType::SLIP:
                out_file_v_ << "\t" << boundary_name << "\n";
                out_file_v_ << "\t{\n";
                out_file_v_ << "\t\ttype\tslip;\n\t}\n";
                break;
            case zgCFD::BoundaryCondationType::NOSLIP:
                out_file_v_ << "\t" << boundary_name << "\n";
                out_file_v_ << "\t{\n";
                out_file_v_ << "\t\ttype\tnoSlip;\n\t}\n";
                break;
            case zgCFD::BoundaryCondationType::INLET:
                break;
            case zgCFD::BoundaryCondationType::OUTLET:
                break;
            case zgCFD::BoundaryCondationType::EMPTY:
                out_file_v_ << "\t" << boundary_name << "\n";
                out_file_v_ << "\t{\n";
                out_file_v_ << "\t\ttype\tempty;\n\t}\n";
                break;
            default:
                break;
            }
        }
    }
    void OutPutOf::WritePBoundary()
    {
        for (uint32_t i = 0; i < mesh_->number_of_boundaries_; i++)
        {
            std::string boundary_name = mesh_->boundaries_[i].boundary_name_;
            BoundaryCondationType boundary_condation_type = p_->boundary_condation_[boundary_name].bc_type_;

            switch (boundary_condation_type)
            {
            case zgCFD::BoundaryCondationType::SPECIFIEDVALUE:
                out_file_p_ << "\t" << boundary_name << "\n";
                out_file_p_ << "\t{\n";
                out_file_p_ << "\t\ttype\t\tfixedValue;\n";
                out_file_p_ << "\t\tvalue\t\tuniform " << p_->boundary_condation_[boundary_name].bc_value_ << ";\n\t}\n";
                break;
            case zgCFD::BoundaryCondationType::SPECIFIEDFLUX:
                break;
            case zgCFD::BoundaryCondationType::ZEROFLUX:
                break;
            case zgCFD::BoundaryCondationType::ZEROGRADIENT:
                out_file_p_ << "\t" << boundary_name << "\n";
                out_file_p_ << "\t{\n";
                out_file_p_ << "\t\ttype\tzeroGradient;\n\t}\n";
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
                out_file_p_ << "\t" << boundary_name << "\n";
                out_file_p_ << "\t{\n";
                out_file_p_ << "\t\ttype\t\empty;\n\t}\n";
                break;
            default:
                break;
            }
        }
    }
    void OutPutOf::WriteHead(const std::string& str)
    {
        if (str == "p")
        {
            out_file_p_ << p_head;
        }
        else if (str == "v")
        {
            out_file_v_ << v_head;
        }
    }
}