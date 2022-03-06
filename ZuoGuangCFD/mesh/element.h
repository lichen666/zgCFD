#pragma once
#include <vector>
#include "ZuoGuangCFD/struct/zg_scalar.h"
#include "ZuoGuangCFD/mesh/point.h"

namespace zgCFD {
	//������
	class element
	{
	public:

		//���캯��
		element();
		element(const int& index);

		//����ֵ
		int index_;

		//�������
		Scalar volume_;
		Scalar old_volume_;

		//����
		point centroid_;

		//����������
		int number_of_neighbours_;

		//������������
		std::vector<int> neighbours_;

		//�����������
		std::vector<int> faces_;

		//��������ĵ�
		std::vector<int> points_;

		//��ʾ��Ԫ�Ǹ�����Ԫ���������Ԫ��+1�������ٽӵ�Ԫ��-1��
		std::vector<int> face_sign_;
	};
}

