#pragma once
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "ZuoGuangCFD/mesh/mesh.h"
#include "ZuoGuangCFD/struct/base_data_structure.h"
#include "ZuoGuangCFD/struct/enums.h"

namespace zgCFD
{
    template <class Type, int vol_or_surface>
    class BaseField
    {
      protected:
        struct BcTypeValue
        {
            BoundaryCondationType bc_type_;
            Type bc_value_;
        };

      public:
        //构造函数
        BaseField(){};

        ~BaseField(){};

        //赋值
        //BaseField& operator=(const BaseField& field);

        //输出
        template <class Type, int vol_or_surface>
        friend std::ostream& operator<<(std::ostream& output, BaseField<Type, vol_or_surface>& field);

        //等于
        bool operator==(const BaseField& field);

      protected:
        //初始化边界条件
        void InitializeBondaryCondation(const Mesh& mesh);
        //设置边界条件
        void SetBondaryCondation(std::string name, BoundaryCondationType bctype, Type value);

      public:
        //用于控制场的读取，输出等等
        std::map<std::string, std::string> control_;
        //边界条件
        std::map<std::string, BcTypeValue> boundary_condation_;

        const Mesh* mesh_;

        std::vector<Type> value_;
        std::vector<Type> value_old_;
        std::vector<Type> value_old_old_;
    };

    template <class Type, int vol_or_surface>
    bool BaseField<Type, vol_or_surface>::operator==(const BaseField& field)
    {
        for (int i = 0; i < this->value_.size(); i++)
        {
            if (!(this->value_[i] == field.value_[i]))
                return false;
        }

        return true;
    }

    template <class Type, int vol_or_surface>
    void BaseField<Type, vol_or_surface>::InitializeBondaryCondation(const Mesh& mesh)
    {
        uint32_t number_of_boundaries = mesh.number_of_boundaries_;

        for (uint32_t i = 0; i < number_of_boundaries; ++i)
        {
            BcTypeValue bc;

            bc.bc_type_ = BoundaryCondationType::SPECIFIEDVALUE;

            std::string name = mesh.boundaries_[i].boundary_name_;

            boundary_condation_[name] = bc;
        }
    }

    template <class Type, int vol_or_surface>
    void BaseField<Type, vol_or_surface>::SetBondaryCondation(std::string name, BoundaryCondationType bctype,
                                                              Type value)
    {
        if (boundary_condation_.find(name) != boundary_condation_.end())
        {
            BcTypeValue bc;
            bc.bc_type_ = bctype;
            bc.bc_value_ = value;

            boundary_condation_[name] = bc;
        }
        else
        {
            std::cout << "没有名为" << name << "的边界！" << std::endl;
        }
    }

    //#include "base_fieldl.h"
}  // namespace zgCFD
