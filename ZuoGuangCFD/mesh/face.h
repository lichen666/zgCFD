#pragma once
#include <vector>

#include "ZuoGuangCFD/mesh/point.h"
#include "ZuoGuangCFD/struct/zg_scalar.h"
#include "ZuoGuangCFD/struct/zg_vector.h"

namespace zgCFD
{
    class face
    {
      public:
        face();
        face(const int& index);
        int index_;
        int owner_;
        int neighbour_;

        Scalar area_;          //面积
        Vector<Scalar> sf_;    //法向量
        point centroid_;       //面心
        Vector<Scalar> cn_;    //面的owner的形心指向neighbour的形心的向量
        Vector<Scalar> e_cn_;  //面的owner的形心指向neighbour的形心的单位向量

        Scalar gf_;  //面插值的几何权重因子

        // TODO 暂时不知道有啥用，后面再看
        Scalar g_diff_;     //面几何扩散系数
        Vector<Scalar> t_;  // owner单元和neighbour单元形心的距离向量
        Scalar wall_dist_;  //面所属单元形心到壁面的垂直距离

        std::vector<int> points_;
    };
}  // namespace zgCFD
