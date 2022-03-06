#pragma once
#include <vector>
#include "ZuoGuangCFD/struct/zg_scalar.h"
#include <iostream>
//#include "matrix_csr_exception.h"

namespace zgCFD {
/***********************************************************\
Compressed Sparse Row（CSR）稀疏矩阵的存储格式:
对于矩阵
[ 11  12      14                 ]
[     22  23      25             ]
[ 31      33  34                 ]
[     42          45  46         ]
[                 55             ]
[                 65  66  67     ]
[                 75      77  78 ]
[                         87  88 ]

可以储存为:
   Index   0    1    2    3    4    5    6    7    8    9   10
   -----------------------------------------------------------
   Val    11   12   14   22   23   25   31   33   34   42   45
   RowPtr  0    3    6    9   12   13   16   19   21    -    -
   Col     0    1    3    1    2    4    0    2    3    1    4

   Index  11   12   13   14   15   16   17   18   19   20   21
   ------------------------------------------------------------
   Val    46   55   65   66   67   75   77   78   87   88    -
   RowPtr  -    -    -    -    -    -    -    -    -    -    -
   Col     5    4    4    5    6    4    6    7    6    7    -

\***********************************************************/
	class CsrMatrix;
	std::ostream& operator<<(std::ostream& output, CsrMatrix& matrix);

	//csr格式存储的矩阵
	class CsrMatrix
	{
	public:

		CsrMatrix(const CsrMatrix& matrix);
		CsrMatrix(const uint32_t rows, const uint32_t cols);

		//向矩阵中插值
		void InsertValue(const uint32_t row, const uint32_t column, const Scalar value);
		//转置
		CsrMatrix Transpose() const;
		//获取row行col列的值
		Scalar GetValue(const uint32_t row, const uint32_t column);
		//获取非零元素的数量
		uint32_t GetNumberOfValues();

		void SetColumn(uint32_t column);
		void SetRow(uint32_t row);

		//加号
		CsrMatrix operator +(const CsrMatrix& matrix) const;
		//减号
		CsrMatrix operator -(const CsrMatrix& matrix) const;
		//负号
		CsrMatrix operator -();
		//矩阵相乘
		CsrMatrix operator ^(const CsrMatrix matrix) const;
		//等号
		bool operator ==(const CsrMatrix matrix) const;


		friend std::ostream& operator<<(std::ostream& output, const CsrMatrix& matrix);

		std::vector<Scalar> values_;//用来存储矩阵中的非零元素的值
		std::vector<uint32_t> row_offfsets_;// 第i个元素记录了前i-1行包含的非零元素的数量
		std::vector<uint32_t> column_indices_;//第i个元素记录了第i个元素的列数

		uint32_t number_of_rows_;//矩阵行数
		uint32_t number_of_columns_;//矩阵列数

	};
}
