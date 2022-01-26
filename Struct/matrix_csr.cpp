#include "matrix_csr.h"
#include <unordered_map>

namespace zgCFD {
	/********************构造函数************************/
	CsrMatrix::CsrMatrix(const CsrMatrix& matrix){
	
		number_of_rows_ = matrix.number_of_rows_;
		number_of_columns_ = matrix.number_of_columns_;
		values_ = matrix.values_;
		row_offfsets_ = matrix.row_offfsets_;
		column_indices_ = matrix.column_indices_;
	}

	CsrMatrix::CsrMatrix(const uint32_t rows, const uint32_t cols) {
		
		number_of_rows_ = rows;
		number_of_columns_ = cols;

		row_offfsets_.resize(rows+1, 0);
	}

	/********************成员函数************************/
	//向矩阵中插值
	void CsrMatrix::InsertValue(const uint32_t row, const uint32_t column, const Scalar value){
	
		if (this->number_of_rows_ < row || this->number_of_columns_ < column)
		{
			//throw matrix_csr_exception("输入的行数/列数超出了矩阵的范围！");
			std::cerr << "输入的行数/列数超出了矩阵的范围！" << std::endl;
			exit(-1);
		}

		if (abs(value - 0) < 0.0000001)
		{
			return;
		}

		int tmp_row_offfset = row_offfsets_[row];
		bool tmp_is_inserted = false;
		bool tmp_is_added = false;

		while (tmp_row_offfset < row_offfsets_[row + 1] && (!tmp_is_inserted) && (!tmp_is_added))
		{

			if (column < column_indices_[tmp_row_offfset])
			{
				column_indices_.insert(column_indices_.begin() + tmp_row_offfset, column);
				values_.insert(values_.begin() + tmp_row_offfset, value);
				tmp_is_inserted = true;
			}
			else if (column == column_indices_[tmp_row_offfset])
			{
				values_[tmp_row_offfset] += value;
				tmp_is_added = true;
			}
			tmp_row_offfset++;
		}

		if ((!tmp_is_inserted) && (!tmp_is_added))
		{
			column_indices_.insert(column_indices_.begin() + tmp_row_offfset, column);
			values_.insert(values_.begin() + tmp_row_offfset, value);
		}

		if (!tmp_is_added)
		{
			for (int i = row + 1; i < row_offfsets_.size(); i++)
			{
				row_offfsets_[i] += 1;
			}
		}
	}

	//转置
	CsrMatrix CsrMatrix::Transpose() const
	{
	
		CsrMatrix tmp_matrix(this->number_of_columns_, this->number_of_rows_);

		std::vector<std::vector<uint32_t>> tmp_rows_vectors;
		std::vector<std::vector<Scalar>> tmp_values;

		tmp_rows_vectors.resize(number_of_columns_);
		tmp_values.resize(number_of_columns_);

		for (uint32_t i = 0; i < this->number_of_rows_; i++)
		{
			for (uint32_t j = this->row_offfsets_[i]; j < this->row_offfsets_[i+1]; j++)
			{
				uint32_t tmp_column = this->column_indices_[j];
				tmp_rows_vectors[tmp_column].push_back(i);
				tmp_values[tmp_column].push_back(this->values_[j]);
			}
		}

		for (uint32_t i = 0; i < number_of_columns_; i++)
		{
			tmp_matrix.column_indices_.insert(tmp_matrix.column_indices_.end(), tmp_rows_vectors[i].begin(), tmp_rows_vectors[i].end());
			tmp_matrix.values_.insert(tmp_matrix.values_.end(), tmp_values[i].begin(), tmp_values[i].end());
			tmp_matrix.row_offfsets_[i + 1] = tmp_matrix.row_offfsets_[i] + tmp_rows_vectors[i].size();
		}

		return tmp_matrix;
	}

	//获取row行col列的值
	Scalar CsrMatrix::GetValue(const uint32_t row, const uint32_t column){

		uint32_t tmp_row_offfset = row_offfsets_[row];

		if (row_offfsets_[row] != row_offfsets_[row + 1])
		{

			while (tmp_row_offfset < row_offfsets_[row + 1] )
			{

				if (column < column_indices_[tmp_row_offfset])
				{
					return 0.0;
				}
				else if (column == column_indices_[tmp_row_offfset])
				{
					return values_[tmp_row_offfset];
				
				}
				tmp_row_offfset++;
			}
		}
		else
		{
			return 0.0;
		}
	}

	//获取非零元素的个数
	uint32_t CsrMatrix::number_of_values(){
	
		return values_.size();
	}

	/********************操作符************************/	
	//加号
	CsrMatrix CsrMatrix::operator +(const CsrMatrix& matrix) const 
	{
	
		if (this->number_of_rows_ != matrix.number_of_columns_ || this->number_of_columns_ != matrix.number_of_columns_)
		{
			std::cerr << "行数列数不相同的矩阵不能相加！" << std::endl;
			exit(-1);
		}

		CsrMatrix tmp_matrix(this->number_of_rows_, this->number_of_columns_);

		//遍历矩阵的行
		for (int l = 1; l < this->row_offfsets_.size(); l++)
		{
			//将this矩阵（左边的矩阵）的行的值插入新矩阵中
			tmp_matrix.values_.insert(tmp_matrix.values_.end(), this->values_.begin() + this->row_offfsets_[l - 1], this->values_.begin() + this->row_offfsets_[l]);
			tmp_matrix.column_indices_.insert(tmp_matrix.column_indices_.end(), this->column_indices_.begin() + this->row_offfsets_[l - 1], this->column_indices_.begin() + this->row_offfsets_[l]);

			int tmp_search = tmp_matrix.column_indices_.size() - this->row_offfsets_[l] + this->row_offfsets_[l - 1];

			for (int r = matrix.row_offfsets_[l - 1]; r < matrix.row_offfsets_[l]; r++)
			{
				int tmp_search_end = tmp_matrix.column_indices_.size();
				bool tmp_is_found = false;
				while (tmp_search < tmp_search_end && !tmp_is_found)
				{
					if (matrix.column_indices_[r] < tmp_matrix.column_indices_[tmp_search])
					{
						tmp_matrix.values_.insert(tmp_matrix.values_.begin() + tmp_search, matrix.values_[r]);
						tmp_matrix.column_indices_.insert(tmp_matrix.column_indices_.begin() + tmp_search, matrix.column_indices_[r]);
						tmp_is_found = true;
					}
					else if (matrix.column_indices_[r] == tmp_matrix.column_indices_[tmp_search])
					{
						tmp_matrix.values_[tmp_search] = tmp_matrix.values_[tmp_search] + matrix.values_[r];
						tmp_is_found = true;
					}
					else tmp_search++;
				}

				if (!tmp_is_found)
				{
					for (int i = r; i < matrix.row_offfsets_[l]; i++)
					{
						tmp_matrix.values_.push_back(matrix.values_[i]);
						tmp_matrix.column_indices_.push_back(matrix.column_indices_[i]);
					}

					break;
				}
			}

			tmp_matrix.row_offfsets_[l] = tmp_matrix.column_indices_.size();
		}

		return tmp_matrix;
	}
	
	//负号
	CsrMatrix CsrMatrix::operator -() 
	{
		
		CsrMatrix tmp_matrix(*this);

		for (int i = 0; i < tmp_matrix.values_.size(); i++)
		{
			tmp_matrix.values_[i] = -tmp_matrix.values_[i];
		}

		return tmp_matrix;
	}

	//减号
	CsrMatrix CsrMatrix::operator -(const CsrMatrix& matrix) const
	{

		if (this->number_of_rows_ != matrix.number_of_columns_ || this->number_of_columns_ != matrix.number_of_columns_)
		{
			std::cerr << "行数列数不相同的矩阵不能相减！" << std::endl;
			exit(-1);
		}

		CsrMatrix tmp_matrix(this->number_of_rows_, this->number_of_columns_);

		//遍历矩阵的行
		for (int l = 1; l < this->row_offfsets_.size(); l++)
		{
			//将this矩阵（左边的矩阵）的行的值插入新矩阵中
			tmp_matrix.values_.insert(tmp_matrix.values_.end(), this->values_.begin() + this->row_offfsets_[l - 1], this->values_.begin() + this->row_offfsets_[l]);
			tmp_matrix.column_indices_.insert(tmp_matrix.column_indices_.end(), this->column_indices_.begin() + this->row_offfsets_[l - 1], this->column_indices_.begin() + this->row_offfsets_[l]);

			int tmp_search = tmp_matrix.column_indices_.size() - this->row_offfsets_[l] + this->row_offfsets_[l - 1];

			for (int r = matrix.row_offfsets_[l - 1]; r < matrix.row_offfsets_[l]; r++)
			{
				int tmp_search_end = tmp_matrix.column_indices_.size();
				bool tmp_is_found = false;
				while (tmp_search < tmp_search_end && !tmp_is_found)
				{
					if (matrix.column_indices_[r] < tmp_matrix.column_indices_[tmp_search])
					{
						tmp_matrix.values_.insert(values_.begin() + tmp_search, -matrix.values_[r]);
						tmp_matrix.column_indices_.insert(tmp_matrix.column_indices_.begin() + tmp_search, matrix.column_indices_[r]);
						tmp_is_found = true;
					}
					else if (matrix.column_indices_[r] == tmp_matrix.column_indices_[tmp_search])
					{
						tmp_matrix.values_[tmp_search] = tmp_matrix.values_[tmp_search] - matrix.values_[r];
						tmp_is_found = true;
					}
					else tmp_search++;
				}

				if (!tmp_is_found)
				{
					for (int i = r; i < matrix.row_offfsets_[l]; i++)
					{
						tmp_matrix.values_.push_back(-matrix.values_[i]);
						tmp_matrix.column_indices_.push_back(matrix.column_indices_[i]);
					}

					break;
				}
			}

			tmp_matrix.row_offfsets_[l] = tmp_matrix.column_indices_.size();
		}

		return tmp_matrix;
	}

	//矩阵相乘
	CsrMatrix CsrMatrix::operator ^(const CsrMatrix matrix) const {
	
		if (this->number_of_columns_ != matrix.number_of_rows_)
		{
			std::cerr << "相乘矩阵的维度错误！" << std::endl;
			exit(-1);
		}

		CsrMatrix tmp_matrix(this->number_of_columns_, this->number_of_columns_);
		CsrMatrix tmp_matrix_t = matrix.Transpose();

		for (uint32_t row = 0; row < this->number_of_rows_; row++)
		{
			// 左矩阵中的非零值列
			std::unordered_map<uint32_t, int> tmp_column_values;

			for (uint32_t i = this->row_offfsets_[row]; i < this->row_offfsets_[row+1]; i++)
			{
				tmp_column_values[this->column_indices_[i]] = this->values_[i];
			}

			// Multiply saved values with corresponding values of the transposed right matrix.
			for (uint32_t col = 0; col < tmp_matrix_t.number_of_rows_; col++)
			{
				int tmp_multiplication_result = 0;

				for (uint32_t i = tmp_matrix_t.row_offfsets_[col]; i < tmp_matrix_t.row_offfsets_[col+1]; i++)
				{
					uint32_t column = tmp_matrix_t.column_indices_[i];

					if (tmp_column_values.find(column) != tmp_column_values.end())
						tmp_multiplication_result += tmp_column_values[column] * tmp_matrix_t.values_[i];
				}

				if (tmp_multiplication_result != 0)
				{
					tmp_matrix.InsertValue(row, col, tmp_multiplication_result);
				}
			}
		}

		return tmp_matrix;
	}

	//等号
	bool CsrMatrix::operator ==(const CsrMatrix matrix) const {
		if (this->number_of_rows_ != matrix.number_of_columns_ || this->number_of_columns_ != matrix.number_of_columns_)
		{
			return false;
		}

		if (this->column_indices_ == matrix.column_indices_ && this->row_offfsets_ == matrix.row_offfsets_ && this->values_ == matrix.values_)
		{
			return true;
		}

		return false;
	}

	/**********************输出************************/

	std::ostream& operator<<(std::ostream& output, CsrMatrix& matrix) {

		output << "row_offfsets:" << std::endl;
		for (std::vector<uint32_t>::iterator it = matrix.row_offfsets_.begin(); it != matrix.row_offfsets_.end(); it++)
		{
			output << *it << " ";
		}
		output << std::endl;

		output << "column_indices:" << std::endl;
		for (std::vector<uint32_t>::iterator it = matrix.column_indices_.begin(); it != matrix.column_indices_.end(); it++)
		{
			output << *it << " ";
		}
		output << std::endl;

		output << "values:" << std::endl;
		for (std::vector<Scalar>::iterator it = matrix.values_.begin(); it != matrix.values_.end(); it++)
		{
			output << *it << " ";
		}
		output << std::endl;

		return output;
	}
}