#pragma once
#include <vector>
#include "ZuoGuangCFD/struct/zg_scalar.h"
#include "ZuoGuangCFD/mesh/point.h"

namespace zgCFD {
	//网格类
	class element
	{
	public:

		//构造函数
		element();
		element(const int& index);

		//索引值
		int index_;

		//网格体积
		Scalar volume_;
		Scalar old_volume_;

		//体心
		point centroid_;

		//相邻网格数
		int number_of_neighbours_;

		//相邻网格索引
		std::vector<int> neighbours_;

		//构成网格的面
		std::vector<int> faces_;

		//构成网格的点
		std::vector<int> points_;

		//表示单元是各个单元面的所属单元（+1）还是临接单元（-1）
		std::vector<int> face_sign_;
	};
}

