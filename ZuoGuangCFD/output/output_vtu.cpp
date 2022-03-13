#include "ZuoGuangCFD/output/output_vtu.h"

namespace zgCFD
{
    OutputVTU::OutputVTU(const std::string& file_name, ScalarVolumeField* p, VectorVolumeField* v, Mesh* mesh)
    {
        this->file_name_ = file_name;
        this->p_ = p;
        this->v_ = v;
        this->mesh_ = mesh;
    }
    void OutputVTU::Write()
    {
        WriteFile(file_name_);
    }
    bool OutputVTU::WriteFile(const std::string& file_name)
    {
        if (!StartWrite(file_name))
            return false;

        WriteXmlHead("<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">");
        {
            WriteXmlHead("<UnstructuredGrid>");

            std::stringstream tmp_string;
            tmp_string << "<Piece NumberOfPoints=\"" << mesh_->number_of_points_ << "\" NumberOfCells=\""
                       << mesh_->number_of_faces_ << "\">";
            WriteXmlHead(tmp_string.str());
            {
                WritePoints();
                WriteCells();
                WriteCellData();
            }
            WriteXmlEnd("</Piece>");

            WriteXmlEnd("</UnstructuredGrid>");
        }
        WriteXmlEnd("</VTKFile>");

        return true;
    }
    void OutputVTU::WritePoints()
    {
        WriteXmlHead("<Points>");
        {
            WriteXmlHead("<DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">");

            for (int i = 0; i < mesh_->number_of_points_; i++)
            {
                WriteSpaceEnter(i, 4);

                out_file_ << mesh_->points_[i].x_ << " " << mesh_->points_[i].y_ << " " << mesh_->points_[i].z_ << " ";
            }
            out_file_ << std::endl;
            WriteXmlEnd("</DataArray>");
        }
        WriteXmlEnd("</Points>");
    }
    void OutputVTU::WriteCells()
    {
        WriteXmlHead("<Cells>");
        {
            WriteXmlHead("<DataArray type=\"Int32\" Name=\"connectivity\" format=\"ascii\">");
            {
                /*for (uint32_t i = 0; i < mesh_->number_of_elements_; i++)
                {
                    WriteSpaceEnter(i, 3);
                    for (auto& point : mesh_->elements_[i].points_)
                    {
                        out_file_ << point << " ";
                    }
                }
                out_file_ << std::endl;*/

                for (uint32_t i = 0; i < mesh_->number_of_faces_; i++)
                {
                    WriteSpaceEnter(i, 5);
                    for (auto& point : mesh_->faces_[i].points_)
                    {
                        out_file_ << point << " ";
                    }
                }
                out_file_ << std::endl;
            }
            WriteXmlEnd("</DataArray>");
            WriteXmlHead("<DataArray type=\"Int32\" Name=\"offsets\" format=\"ascii\">");
            {
                /*uint32_t count = 0;
                for (uint32_t i = 0; i < mesh_->number_of_elements_; i++)
                {
                    count += mesh_->elements_[i].points_.size();
                    WriteSpaceEnter(i, 15);

                    out_file_ << count << " ";
                }
                out_file_ << std::endl;*/
                uint32_t count = 0;
                for (uint32_t i = 0; i < mesh_->number_of_faces_; i++)
                {
                    count += mesh_->faces_[i].points_.size();
                    WriteSpaceEnter(i, 15);

                    out_file_ << count << " ";
                }
                out_file_ << std::endl;
            }
            WriteXmlEnd("</DataArray>");
            // TODO 只写了六面体网格的
            WriteXmlHead("<DataArray type=\"UInt8\" Name=\"types\" format=\"ascii\">");
            {
                /*for (uint32_t i = 0; i < mesh_->number_of_elements_; i++)
                {
                    WriteSpaceEnter(i, 15);

                    out_file_ << 12 << " ";
                }
                out_file_ << std::endl;*/
                for (uint32_t i = 0; i < mesh_->number_of_faces_; i++)
                {
                    WriteSpaceEnter(i, 15);

                    out_file_ << 9 << " ";
                }
                out_file_ << std::endl;
            }
            WriteXmlEnd("</DataArray>");
        }
        WriteXmlEnd("</Cells>");
    }
    void OutputVTU::WriteCellData()
    {
        /*WriteXmlHead("<CellData>");
        {
            WriteXmlHead("<DataArray type=\"Float32\" Name=\"V\" NumberOfComponents=\"3\" format=\"ascii\">");
            {
                for (uint32_t i = 0; i < mesh_->number_of_elements_; i++)
                {
                    Scalar vx = v_->value_[i].x();
                    Scalar vy = v_->value_[i].y();
                    Scalar vz = v_->value_[i].z();

                    WriteSpaceEnter(i, 5);
                    std::stringstream ss;
                    ss << vx << " " << vy << " " << vz << " ";
                    out_file_ << ss.str();
                }
                out_file_ << std::endl;
            }
            WriteXmlEnd("</DataArray>");
            WriteXmlHead("<DataArray type=\"Float32\" Name=\"P\" format=\"ascii\">");
            {
                for (uint32_t i = 0; i < mesh_->number_of_elements_; i++)
                {
                    WriteSpaceEnter(i, 15);
                    std::stringstream ss;
                    ss << p_->value_[i] << " ";
                    out_file_ << ss.str();
                }
                out_file_ << std::endl;
            }
            WriteXmlEnd("</DataArray>");
        }
        WriteXmlEnd("</CellData>");*/
        WriteXmlHead("<CellData>");
        {
            ScalarSurfaceField ps = exp::Interpolate(*p_, TheInterpolationScheme::AVERAGE);
            VectorSurfaceField vs = exp::Interpolate(*v_, TheInterpolationScheme::AVERAGE);
            WriteXmlHead("<DataArray type=\"Float32\" Name=\"V\" NumberOfComponents=\"3\" format=\"ascii\">");
            {
                for (uint32_t i = 0; i < mesh_->number_of_interior_faces_; i++)
                {
                    Scalar vx = vs.interior_value_[i].x();
                    Scalar vy = vs.interior_value_[i].y();
                    Scalar vz = vs.interior_value_[i].z();

                    WriteSpaceEnter(i, 5);
                    std::stringstream ss;
                    ss << vx << " " << vy << " " << vz << " ";
                    out_file_ << ss.str();
                }
                out_file_ << std::endl;
                for (uint32_t i = mesh_->number_of_interior_faces_; i < mesh_->number_of_faces_; i++)
                {
                    uint32_t index = i - mesh_->number_of_interior_faces_;
                    Scalar vx = vs.boundary_value_[index].x();
                    Scalar vy = vs.boundary_value_[index].y();
                    Scalar vz = vs.boundary_value_[index].z();

                    WriteSpaceEnter(index, 5);
                    std::stringstream ss;
                    ss << vx << " " << vy << " " << vz << " ";
                    out_file_ << ss.str();
                }
                out_file_ << std::endl;
            }
            WriteXmlEnd("</DataArray>");
            WriteXmlHead("<DataArray type=\"Float32\" Name=\"P\" format=\"ascii\">");
            {
                for (uint32_t i = 0; i < mesh_->number_of_interior_faces_; i++)
                {
                    WriteSpaceEnter(i, 15);
                    std::stringstream ss;
                    ss << ps.interior_value_[i] << " ";
                    out_file_ << ss.str();
                }
                out_file_ << std::endl;
                for (uint32_t i = mesh_->number_of_interior_faces_; i < mesh_->number_of_faces_; i++)
                {
                    uint32_t index = i - mesh_->number_of_interior_faces_;
                    WriteSpaceEnter(index, 15);
                    std::stringstream ss;
                    ss << ps.boundary_value_[index] << " ";
                    out_file_ << ss.str();
                }
                out_file_ << std::endl;
            }
            WriteXmlEnd("</DataArray>");
        }
        WriteXmlEnd("</CellData>");
    }
    bool OutputVTU::StartWrite(const std::string& file_name)
    {
        out_file_.open(file_name, std::ios::out);
        if (!out_file_.is_open())
        {
            return false;
        }
        out_file_ << "<?xml version=\"1.0\"?>" << std::endl;
        format_.clear();
        return true;
    }
    bool OutputVTU::EndWrite()
    {
        out_file_.close();
        return true;
    }
    void OutputVTU::WriteXmlHead(const std::string& str)
    {
        out_file_ << format_ << str << std::endl;
        format_.append("  ");
    }
    void OutputVTU::WriteXmlEnd(const std::string& str)
    {
        format_.erase(format_.size() - 2);
        out_file_ << format_ << str << std::endl;
    }
    void OutputVTU::WriteSpaceEnter(int index, int interval)
    {
        if (index % interval == 0)
        {
            if (index == 0)
            {
                out_file_ << format_;
            }
            else
            {
                out_file_ << std::endl << format_;
            }
        }
    }
}  // namespace zgCFD
