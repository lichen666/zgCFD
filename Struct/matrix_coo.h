#pragma once
#include<vector>

namespace zgCFD {
	class matrix_coo
	{
	public:
		/********************构造函数************************/
		matrix_coo();

		/********************成员函数************************/

		/**********************操作符************************/
		matrix_coo operator +(const matrix_coo mat);
		matrix_coo operator -(const matrix_coo mat);

		/********************成员变量************************/
		std::vector<int> values_;//用来存储矩阵中的非零元素的值
		std::vector<int> index_of_rows_;// 记录了第i个元素的行索引
		std::vector<int> index_of_cols_;//记录了第i个元素的列索引

		int rows_;//矩阵行数
		int cols_;//矩阵列数
		int number_of_values_;//非零元素的数量
	};
}

