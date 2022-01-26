#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include "../mesh/mesh.h"
#include "base_data_structure.h"

namespace zgCFD {
	template<class Type,int vol_or_surface>
	class BaseField
	{
	public:
		//���캯��
		BaseField() {};

		~BaseField() {};

		//���ڿ��Ƴ��Ķ�ȡ������ȵ�
		std::unordered_map<std::string, std::string> control_;

		//��ֵ
		BaseField& operator = (const BaseField & field);

		//���
		template<class Type, int vol_or_surface>
		friend std::ostream& operator<<(std::ostream& output, BaseField<Type, vol_or_surface>& field);

		//����
		bool operator == (const BaseField& field);

		std::vector<Type> value_;
		std::vector<Type> value_old_;
		std::vector<Type> value_old_old_;

	};

	template<class Type, int vol_or_surface>
	bool BaseField<Type, vol_or_surface>::operator == (const BaseField& field){
	
		for (int i = 0; i < this->value_.size(); i++)
		{
			if (!(this->value_[i] == field.value_[i])) return false;
		}

		return true;
	}

//#include "base_fieldl.h"
}


