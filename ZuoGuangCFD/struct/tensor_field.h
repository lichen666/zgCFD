#pragma once
#include "ZuoGuangCFD/struct/base_field.h"

namespace zgCFD
{
    template <int vol_or_surface>
    class TensorField : public BaseField<Tensor<Scalar>, vol_or_surface>
    {
      public:
        //构造函数

        TensorField(const Mesh& mesh, const std::string& field_name, const std::string& field_path,
                    const std::string& is_write = "no", const std::string& is_read = "no");
        TensorField(const Tensor<Scalar>& value, const Mesh& mesh, const std::string& field_name,
                    const std::string& field_path, const std::string& is_write = "no",
                    const std::string& is_read = "no");
        TensorField(const TensorField& field, const std::string& field_name, const std::string& field_path,
                    const std::string& is_write = "no", const std::string& is_read = "no");

      private:
        void ReadField(const std::string& field_name, const std::string& field_path);
    };

#include "tensor_fieldl.h"

    template <int vol_or_surface>
    TensorField<vol_or_surface>::TensorField(const Mesh& mesh, const std::string& field_name,
                                             const std::string& field_path, const std::string& is_write,
                                             const std::string& is_read)
    {
        // vol_or_surface == 0表示体积场；vol_or_surface == 1表示面积场
        if (vol_or_surface == 0)
        {
            this->value_.resize(mesh.elements_.size() + mesh.number_of_boundary_faces_,
                                Tensor<Scalar>(0, 0, 0, 0, 0, 0, 0, 0, 0));
        }
        else
        {
            this->value_.resize(mesh.faces_.size(), Tensor<Scalar>(0, 0, 0, 0, 0, 0, 0, 0, 0));
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
    TensorField<vol_or_surface>::TensorField(const Tensor<Scalar>& value, const Mesh& mesh,
                                             const std::string& field_name, const std::string& field_path,
                                             const std::string& is_write, const std::string& is_read)
    {
        // vol_or_surface == 0表示体积场；vol_or_surface == 1表示面积场
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
    TensorField<vol_or_surface>::TensorField(const TensorField<vol_or_surface>& field, const std::string& field_name,
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
    void TensorField<vol_or_surface>::ReadField(const std::string& field_name, const std::string& field_path)
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

            if (tmp_string == "tensor")
            {
                tmp_field_file >> tmp_num;
                tmp_field_file >> tmp_char;  //"("

                this->value_.resize(tmp_num);

                for (int i = 0; i < tmp_num; i++)
                {
                    tmp_field_file >> tmp_char;

                    tmp_field_file >> this->value_[i].xx();
                    tmp_field_file >> this->value_[i].xy();
                    tmp_field_file >> this->value_[i].xz();
                    tmp_field_file >> this->value_[i].yx();
                    tmp_field_file >> this->value_[i].yy();
                    tmp_field_file >> this->value_[i].yz();
                    tmp_field_file >> this->value_[i].zx();
                    tmp_field_file >> this->value_[i].zy();
                    tmp_field_file >> this->value_[i].zz();

                    tmp_field_file >> tmp_char;
                }

                tmp_field_file >> tmp_string;  //"};"
            }
            else
            {
                std::cerr << field_path + "\\" + field_name << "文件内容错误，应为'tensor'。" << std::endl;
                exit(-1);
            }
        }
        else if (tmp_string == "uniform")
        {
            tmp_field_file >> tmp_char;

            Tensor<Scalar> tmp_tensor;

            tmp_field_file >> tmp_tensor.xx();
            tmp_field_file >> tmp_tensor.xy();
            tmp_field_file >> tmp_tensor.xz();
            tmp_field_file >> tmp_tensor.yx();
            tmp_field_file >> tmp_tensor.yy();
            tmp_field_file >> tmp_tensor.yz();
            tmp_field_file >> tmp_tensor.zx();
            tmp_field_file >> tmp_tensor.zy();
            tmp_field_file >> tmp_tensor.zz();

            tmp_field_file >> tmp_string;

            for (int i = 0; i < this->value_.size(); i++)
            {
                this->value_[i] = tmp_tensor;
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
