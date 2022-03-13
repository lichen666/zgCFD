#pragma once
#include "ZuoGuangCFD/struct/base_field.h"
namespace zgCFD
{
    template <int vol_or_surface>
    class VectorField : public BaseField<Vector<Scalar>, vol_or_surface>
    {
      public:
        //构造函数

        VectorField(Mesh& mesh, const std::string& field_name, const std::string& field_path,
                    const std::string& is_write = "no", const std::string& is_read = "no");
        VectorField(const Vector<Scalar>& value, Mesh& mesh, const std::string& field_name,
                    const std::string& field_path, const std::string& is_write = "no",
                    const std::string& is_read = "no");
        VectorField(const VectorField& field, const std::string& field_name, const std::string& field_path,
                    const std::string& is_write = "no", const std::string& is_read = "no");

        void AdaptToBondary();

      private:
        void ReadField(const std::string& field_name, const std::string& field_path);
    };

#include "vector_fieldl.h"

    template <int vol_or_surface>
    VectorField<vol_or_surface>::VectorField(Mesh& mesh, const std::string& field_name, const std::string& field_path,
                                             const std::string& is_write, const std::string& is_read)
    {
        // vol_or_surface == 0表示体积场；vol_or_surface == 1表示面积场
        if (vol_or_surface == 0)
        {
            this->interior_value_.resize(mesh.number_of_elements_, GeneralVector(0, 0, 0));
            this->boundary_value_.resize(mesh.number_of_boundary_faces_, GeneralVector(0, 0, 0));
        }
        else
        {
            this->interior_value_.resize(mesh.number_of_interior_faces_, GeneralVector(0, 0, 0));
            this->boundary_value_.resize(mesh.number_of_boundary_faces_, GeneralVector(0, 0, 0));
        }

        this->mesh_ = &mesh;

        this->InitializeBondaryCondation(mesh);

        this->control_["name"] = field_name;
        this->control_["path"] = field_path;
        this->control_["input"] = is_read;
        this->control_["output"] = is_write;

        if (is_read == "yes")
        {
            ReadField(field_name, field_path);
        }
        else if (is_read == "if_exist")
        {
            std::string tmp_file_path;
            tmp_file_path = field_path + "\\" + field_name;
            struct stat buffer;
            if (stat(tmp_file_path.c_str(), &buffer) == 0)
            {
                ReadField(field_name, field_path);
            }
        }
    }

    template <int vol_or_surface>
    VectorField<vol_or_surface>::VectorField(const Vector<Scalar>& value, Mesh& mesh, const std::string& field_name,
                                             const std::string& field_path, const std::string& is_write,
                                             const std::string& is_read)
    {
        // vol_or_surface == 0表示体积场；vol_or_surface == 1表示面积场
        if (vol_or_surface == 0)
        {
            this->interior_value_.resize(mesh.number_of_elements_, value);
            this->boundary_value_.resize(mesh.number_of_boundary_faces_, value);
        }
        else
        {
            this->interior_value_.resize(mesh.number_of_interior_faces_, value);
            this->boundary_value_.resize(mesh.number_of_boundary_faces_, value);
        }

        this->mesh_ = &mesh;

        this->InitializeBondaryCondation(mesh);

        this->control_["name"] = field_name;
        this->control_["path"] = field_path;
        this->control_["input"] = is_read;
        this->control_["output"] = is_write;

        if (is_read == "yes")
        {
            ReadField(field_name, field_path);
        }
        else if (is_read == "if_exist")
        {
            std::string tmp_file_path;
            tmp_file_path = field_path + "\\" + field_name;
            struct stat buffer;
            if (stat(tmp_file_path.c_str(), &buffer) == 0)
            {
                ReadField(field_name, field_path);
            }
        }
    }

    template <int vol_or_surface>
    VectorField<vol_or_surface>::VectorField(const VectorField<vol_or_surface>& field, const std::string& field_name,
                                             const std::string& field_path, const std::string& is_write,
                                             const std::string& is_read)
    {
        this->interior_value_ = field.interior_value_;
        this->boundary_value_ = field.boundary_value_;

        this->mesh_ = field.mesh_;

        this->InitializeBondaryCondation(*field.mesh_);

        this->control_["name"] = field_name;
        this->control_["path"] = field_path;
        this->control_["input"] = is_read;
        this->control_["output"] = is_write;

        if (is_read == "yes")
        {
            ReadField(field_name, field_path);
        }
        else if (is_read == "if_exist")
        {
            std::string tmp_file_path;
            tmp_file_path = field_path + "\\" + field_name;
            struct stat buffer;
            if (stat(tmp_file_path.c_str(), &buffer) == 0)
            {
                ReadField(field_name, field_path);
            }
        }
    }

    template <int vol_or_surface>
    inline void VectorField<vol_or_surface>::AdaptToBondary()
    {
        if (vol_or_surface == 0)
        {
            for (uint32_t i = 0; i < this->mesh_->number_of_boundaries_; i++)
            {
                uint32_t start_face = this->mesh_->boundaries_[i].start_face_;
                uint32_t number_of_faces = this->mesh_->boundaries_[i].number_of_faces_;

                std::string boundary_name = this->mesh_->boundaries_[i].boundary_name_;
                switch (this->boundary_condation_[boundary_name].bc_type_)
                {
                    case BoundaryCondationType::SPECIFIEDVALUE:
                        for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                        {
                            uint32_t index = j - this->mesh_->number_of_interior_faces_;

                            this->boundary_value_[index] = this->boundary_condation_[boundary_name].bc_value_;
                        }
                        break;
                    case BoundaryCondationType::ZEROGRADIENT:
                        for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                        {
                            uint32_t index = j - this->mesh_->number_of_interior_faces_;

                            uint32_t owner = this->mesh_->faces_[j].owner_;

                            this->boundary_value_[index] = this->interior_value_[owner];
                        }
                        break;
                    case BoundaryCondationType::INLET:
                        for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                        {
                            uint32_t index = j - this->mesh_->number_of_interior_faces_;

                            uint32_t owner = this->mesh_->faces_[j].owner_;

                            this->boundary_value_[index] = this->interior_value_[owner];
                        }
                        break;
                    case BoundaryCondationType::OUTLET:
                        for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                        {
                            uint32_t index = j - this->mesh_->number_of_interior_faces_;

                            uint32_t owner = this->mesh_->faces_[j].owner_;

                            this->boundary_value_[index] = this->interior_value_[owner];
                        }
                        break;
                    case BoundaryCondationType::EMPTY:
                        for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                        {
                            uint32_t index = j - this->mesh_->number_of_interior_faces_;

                            this->boundary_value_[index] = GeneralVector(0, 0, 0);
                        }
                        break;
                    case BoundaryCondationType::SLIP:
                        for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                        {
                            uint32_t index = j - this->mesh_->number_of_interior_faces_;

                            uint32_t owner = this->mesh_->faces_[j].owner_;

                            this->boundary_value_[index] = this->interior_value_[owner];
                        }
                        break;
                    case BoundaryCondationType::NOSLIP:
                        for (uint32_t j = start_face; j < start_face + number_of_faces; j++)
                        {
                            uint32_t index = j - this->mesh_->number_of_interior_faces_;

                            this->boundary_value_[index] = GeneralVector(0, 0, 0);
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }

    template <int vol_or_surface>
    void VectorField<vol_or_surface>::ReadField(const std::string& field_name, const std::string& field_path)
    {
        std::fstream tmp_field_file(field_path + "\\" + field_name, std::ios::in);

        if (!tmp_field_file.is_open())
        {
            std::cerr << "不存在文件：" << field_path + "\\" + field_name << std::endl;
            exit(-1);
        }

        char tmp_char;
        std::string tmp_string;
        Scalar tmp_num;

        while (true)
        {
            tmp_field_file >> tmp_string;
            if (tmp_string == "internalField")
            {
                break;
            }
        }
        // tmp_field_file >> tmp_string;//internalField
        tmp_field_file >> tmp_string;

        if (tmp_string == "nonuniform")
        {
            tmp_field_file >> tmp_string;

            tmp_string = tmp_string.substr(tmp_string.find("<") + 1, tmp_string.find(">") - tmp_string.find("<") - 1);

            if (tmp_string == "vector")
            {
                tmp_field_file >> tmp_num;
                tmp_field_file >> tmp_char;  //"("

                this->interior_value_.resize(tmp_num);

                for (int i = 0; i < tmp_num; i++)
                {
                    tmp_field_file >> tmp_char;

                    tmp_field_file >> this->interior_value_[i].x();
                    tmp_field_file >> this->interior_value_[i].y();
                    tmp_field_file >> this->interior_value_[i].z();

                    tmp_field_file >> tmp_char;
                }

                tmp_field_file >> tmp_string;  //"};"
            }
            else
            {
                std::cerr << field_path + "\\" + field_name << "文件内容错误，应为'vector'。" << std::endl;
                exit(-1);
            }
        }
        else if (tmp_string == "uniform")
        {
            tmp_field_file >> tmp_char;

            Vector<Scalar> tmp_vector;

            tmp_field_file >> tmp_vector.x();
            tmp_field_file >> tmp_vector.y();
            tmp_field_file >> tmp_vector.z();

            tmp_field_file >> tmp_string;

            for (int i = 0; i < this->interior_value_.size(); i++)
            {
                this->interior_value_[i] = tmp_vector;
            }
        }
        else
        {
            std::cerr << field_path + "\\" + field_name << "文件内容错误，应为'nonuniform'或'uniform'。" << std::endl;
            exit(-1);
        }

        tmp_field_file.close();
    }
}  // namespace zgCFD
