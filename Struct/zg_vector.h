#pragma once
#include "zg_scalar.h"
#include <math.h>
#include <ostream>

namespace zgCFD {
    template<class Type>
    class Vector
    {
    public:
        enum components { X, Y, Z };

        Type v_[3];

        // * * * * * * * * * * * * * * * ���캯�� * * * * * * * * * * * * * * * //
        Vector();
        Vector(const Type& x, const Type& y, const Type& z);
        Vector(const Vector& vec);

        // * * * * * * * * * * * * * * * ��Ա���� * * * * * * * * * * * * * * * //
		const Type& x() const;
		const Type& y() const;
		const Type& z() const;

        Type& x();
        Type& y();
        Type& z();

		//ģ��
        Scalar Magnitude();

		//���
		friend std::ostream& operator<<(std::ostream& output, Vector<Type>& v);

        //����
        template<class Type2>
        bool operator ==(const Vector<Type2>& v);
    };

#include "zg_vectorl.h"
 }

