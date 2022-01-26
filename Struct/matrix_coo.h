#pragma once
#include<vector>

namespace zgCFD {
	class matrix_coo
	{
	public:
		/********************���캯��************************/
		matrix_coo();

		/********************��Ա����************************/

		/**********************������************************/
		matrix_coo operator +(const matrix_coo mat);
		matrix_coo operator -(const matrix_coo mat);

		/********************��Ա����************************/
		std::vector<int> values_;//�����洢�����еķ���Ԫ�ص�ֵ
		std::vector<int> index_of_rows_;// ��¼�˵�i��Ԫ�ص�������
		std::vector<int> index_of_cols_;//��¼�˵�i��Ԫ�ص�������

		int rows_;//��������
		int cols_;//��������
		int number_of_values_;//����Ԫ�ص�����
	};
}

