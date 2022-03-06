#pragma once
#include "ZuoGuangCFD/struct/base_field.h"
namespace zgCFD
{
    template <int vol_or_surface>
    class VectorField : public BaseField<Vector<Scalar>, vol_or_surface>
    {
      public:
        //���캯��

        VectorField(const Mesh& mesh, const std::string& field_name, const std::string& field_path,
                    const std::string& is_write = "no", const std::string& is_read = "no");
        VectorField(const Vector<Scalar>& value, const Mesh& mesh, const std::string& field_name,
                    const std::string& field_path, const std::string& is_write = "no",
                    const std::string& is_read = "no");
        VectorField(const VectorField& field, const std::string& field_name, const std::string& field_path,
                    const std::string& is_write = "no", const std::string& is_read = "no");

      private:
        void ReadField(const std::string& field_name, const std::string& field_path);
    };

#include "vector_fieldl.h"

    template <int vol_or_surface>
    VectorField<vol_or_surface>::VectorField(const Mesh& mesh, const std::string& field_name,
                                             const std::string& field_path, const std::string& is_write,
                                             const std::string& is_read)
    {
        // vol_or_surface == 0��ʾ�������vol_or_surface == 1��ʾ�����
        if (vol_or_surface == 0)
        {
            this->value_.resize(mesh.elements_.size() + mesh.number_of_boundary_faces_, Vector<Scalar>(0, 0, 0));
        }
        else
        {
            this->value_.resize(mesh.faces_.size(), Vector<Scalar>(0, 0, 0));
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
    VectorField<vol_or_surface>::VectorField(const Vector<Scalar>& value, const Mesh& mesh,
                                             const std::string& field_name, const std::string& field_path,
                                             const std::string& is_write, const std::string& is_read)
    {
        // vol_or_surface == 0��ʾ�������vol_or_surface == 1��ʾ�����
        if (vol_or_surface == 0)
        {
            this->value_.resize(mesh.elements_.size() + mesh.number_of_boundary_faces_, value);
        }
        else
        {
            this->value_.resize(mesh.faces_.size(), value);
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
        this->value_ = field.value_;

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
    void VectorField<vol_or_surface>::ReadField(const std::string& field_name, const std::string& field_path)
    {
        std::fstream tmp_field_file(field_path + "\\" + field_name, std::ios::in);

        if (!tmp_field_file.is_open())
        {
            std::cerr << "�������ļ���" << field_path + "\\" + field_name << std::endl;
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

                this->value_.resize(tmp_num);

                for (int i = 0; i < tmp_num; i++)
                {
                    tmp_field_file >> tmp_char;

                    tmp_field_file >> this->value_[i].x();
                    tmp_field_file >> this->value_[i].y();
                    tmp_field_file >> this->value_[i].z();

                    tmp_field_file >> tmp_char;
                }

                tmp_field_file >> tmp_string;  //"};"
            }
            else
            {
                std::cerr << field_path + "\\" + field_name << "�ļ����ݴ���ӦΪ'vector'��" << std::endl;
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

            for (int i = 0; i < this->value_.size(); i++)
            {
                this->value_[i] = tmp_vector;
            }
        }
        else
        {
            std::cerr << field_path + "\\" + field_name << "�ļ����ݴ���ӦΪ'nonuniform'��'uniform'��" << std::endl;
            exit(-1);
        }

        tmp_field_file.close();
    }
}  // namespace zgCFD
