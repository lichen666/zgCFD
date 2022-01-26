#pragma once
#include <vector>
#include "zg_scalar.h"
#include <iostream>
//#include "matrix_csr_exception.h"

namespace zgCFD {
/***********************************************************\
Compressed Sparse Row��CSR��ϡ�����Ĵ洢��ʽ:
���ھ���
[ 11  12      14                 ]
[     22  23      25             ]
[ 31      33  34                 ]
[     42          45  46         ]
[                 55             ]
[                 65  66  67     ]
[                 75      77  78 ]
[                         87  88 ]

���Դ���Ϊ:
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

	//csr��ʽ�洢�ľ���
	class CsrMatrix
	{
	public:
		/********************���캯��************************/

		CsrMatrix(const CsrMatrix& matrix);
		CsrMatrix(const uint32_t rows, const uint32_t cols);

		/********************��Ա����************************/
		//������в�ֵ
		void InsertValue(const uint32_t row, const uint32_t column, const Scalar value);
		//ת��
		CsrMatrix Transpose() const;
		//��ȡrow��col�е�ֵ
		Scalar GetValue(const uint32_t row, const uint32_t column);
		//��ȡ����Ԫ�ص�����
		uint32_t number_of_values();

		/**********************������************************/
		//�Ӻ�
		CsrMatrix operator +(const CsrMatrix& matrix) const;
		//����
		CsrMatrix operator -(const CsrMatrix& matrix) const;
		//����
		CsrMatrix operator -();
		//�������
		CsrMatrix operator ^(const CsrMatrix matrix) const;
		//�Ⱥ�
		bool operator ==(const CsrMatrix matrix) const;

		/**********************���************************/

		friend std::ostream& operator<<(std::ostream& output, const CsrMatrix& matrix);

		/********************��Ա����************************/
		std::vector<Scalar> values_;//�����洢�����еķ���Ԫ�ص�ֵ
		std::vector<uint32_t> row_offfsets_;// ��i��Ԫ�ؼ�¼��ǰi-1�а����ķ���Ԫ�ص�����
		std::vector<uint32_t> column_indices_;//��i��Ԫ�ؼ�¼�˵�i��Ԫ�ص�����

		uint32_t number_of_rows_;//��������
		uint32_t number_of_columns_;//��������

	};
}
