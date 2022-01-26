#include "pch.h"
#include "base_data_structure.h"

TEST(Vector,euqal){
	zgCFD::Vector<int> a(1, 1, 1);
	zgCFD::Vector<int> b(2, 2, 2);
	zgCFD::Vector<int> c(2, 2, 2);
	zgCFD::Vector<zgCFD::Scalar> d(2., 2., 2.);
	EXPECT_FALSE(a == b);
	EXPECT_TRUE(b == c);
	EXPECT_FALSE(a == d);
	EXPECT_TRUE(b == d);
}

TEST(Vector, Vector) {
	zgCFD::Vector<int> a(1, 1, 1);
	zgCFD::Vector<int> b(2, 2, 2);
	zgCFD::Vector<int> c(3, 3, 3);
	zgCFD::Vector<zgCFD::Scalar> d(4., 4., 4.);
	zgCFD::Scalar e = 4;
	zgCFD::Vector<zgCFD::Scalar> f(3., 4., 5.);
	zgCFD::Vector<zgCFD::Scalar> g(1., -2., 1.);

	EXPECT_TRUE((a + b) == c);
	EXPECT_TRUE((c - a) == b);
	EXPECT_TRUE((e * a) == d);
	EXPECT_TRUE((a * e) == d);
	EXPECT_TRUE((d / e) == a);
	EXPECT_EQ(12, a & d);
	EXPECT_TRUE((a ^ f) == g);
}

TEST(Tensor, euqal) {
	zgCFD::Tensor<int> a(1, 1, 1, 1, 1, 1, 1, 1, 1);
	zgCFD::Tensor<int> b(2, 2, 2, 2, 2, 2, 2, 2, 2);
	zgCFD::Tensor<int> c(2, 2, 2, 2, 2, 2, 2, 2, 2);
	zgCFD::Tensor<zgCFD::Scalar> d(2., 2., 2., 2., 2., 2., 2., 2., 2.);
	EXPECT_FALSE(a == b);
	EXPECT_TRUE(b == c);
	EXPECT_FALSE(a == d);
	EXPECT_TRUE(b == d);
}

TEST(Tensor, Tensor) {
	zgCFD::Tensor<int> a(1, 1, 1, 1, 1, 1, 1, 1, 1);
	zgCFD::Tensor<int> b(2, 2, 2, 2, 2, 2, 2, 2, 2);
	zgCFD::Tensor<int> c(3, 3, 3, 3, 3, 3, 3, 3, 3);
	zgCFD::Tensor<zgCFD::Scalar> d(2., 2., 2., 2., 2., 2., 2., 2., 2.);
	zgCFD::Scalar e = 3;
	zgCFD::Tensor<zgCFD::Scalar> f(5, 6, 8, 0, 557, 7, 4, 0, 0);
	zgCFD::Tensor<zgCFD::Scalar> g(57, 8, 0, 0, 577, 5, 5, 9, 0);
	zgCFD::Tensor<zgCFD::Scalar> h(325, 3574, 30, 35, 321452, 2785, 228, 32, 0);
	EXPECT_TRUE((a + b) == c);
	EXPECT_TRUE((c - b) == a);
	EXPECT_TRUE(3 * a == c);
	EXPECT_TRUE(a * 3 == c);
	EXPECT_TRUE(c / 3 == a);
	EXPECT_EQ(18, a && b);
	EXPECT_TRUE((f & g) == h);
}

TEST(Tensor, Vecor) {
	zgCFD::Tensor<int> a(2, 4, 5, 9, 8, 7, 14, 75, 5);
	zgCFD::Vector<int> b(8, 5, 77);
	zgCFD::Vector<int> c(421, 651, 872);
	EXPECT_TRUE((a & b) == c);
}