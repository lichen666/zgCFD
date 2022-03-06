#pragma once
#include <math.h>

#include <ostream>

#include "ZuoGuangCFD/struct/zg_scalar.h"

namespace zgCFD
{
    template <class Type>
    class Vector
    {
      public:
        enum components
        {
            X,
            Y,
            Z
        };

        Type v_[3];

        // * * * * * * * * * * * * * * * 构造函数 * * * * * * * * * * * * * * * //
        Vector();
        Vector(const Type& x, const Type& y, const Type& z);
        Vector(const Vector& vec);

        // * * * * * * * * * * * * * * * 成员函数 * * * * * * * * * * * * * * * //
        const Type& x() const;
        const Type& y() const;
        const Type& z() const;

        Type& x();
        Type& y();
        Type& z();

        //模长
        Scalar Magnitude();

        void Normalize();

        //输出
        friend std::ostream& operator<<(std::ostream& output, Vector<Type>& v);

        //等于
        template <class Type2>
        bool operator==(const Vector<Type2>& v);
    };

#include "zg_vectorl.h"
}  // namespace zgCFD
