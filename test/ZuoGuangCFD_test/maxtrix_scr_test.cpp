#include "pch.h"
#include "matrix_csr.h"

TEST(matrix_csr, equal) {
	zgCFD::CsrMatrix matrix_csr_test_a(4, 4);
	zgCFD::CsrMatrix matrix_csr_test_b(4, 4);
	zgCFD::CsrMatrix matrix_csr_test_c(4, 4);
	
	matrix_csr_test_a.InsertValue(1, 1, 2);
	matrix_csr_test_a.InsertValue(2, 2, 3);

	matrix_csr_test_b.InsertValue(1, 1, 2);
	matrix_csr_test_b.InsertValue(2, 2, 3);

	matrix_csr_test_c.InsertValue(1, 1, 4);
	matrix_csr_test_c.InsertValue(2, 2, 4);

	EXPECT_TRUE(matrix_csr_test_a == matrix_csr_test_b);
	EXPECT_FALSE(matrix_csr_test_a == matrix_csr_test_c);
}

TEST(matrix_csr, add) {
	zgCFD::CsrMatrix matrix_csr_test_a(4, 4);
	zgCFD::CsrMatrix matrix_csr_test_b(4, 4);
	zgCFD::CsrMatrix matrix_csr_test_c(4, 4);

	matrix_csr_test_a.InsertValue(1, 1, 2);
	matrix_csr_test_a.InsertValue(2, 2, 3);

	matrix_csr_test_b.InsertValue(1, 1, 2);
	matrix_csr_test_b.InsertValue(2, 2, 3);

	matrix_csr_test_c.InsertValue(1, 1, 4);
	matrix_csr_test_c.InsertValue(2, 2, 6);

	EXPECT_TRUE((matrix_csr_test_a + matrix_csr_test_b) == matrix_csr_test_c);
}

TEST(matrix_csr, subtract) {
	zgCFD::CsrMatrix matrix_csr_test_a(4, 4);
	zgCFD::CsrMatrix matrix_csr_test_b(4, 4);
	zgCFD::CsrMatrix matrix_csr_test_c(4, 4);

	matrix_csr_test_a.InsertValue(1, 1, 9);
	matrix_csr_test_a.InsertValue(2, 2, 3);

	matrix_csr_test_b.InsertValue(1, 1, 2);
	matrix_csr_test_b.InsertValue(2, 2, 7);

	matrix_csr_test_c.InsertValue(1, 1, 7);
	matrix_csr_test_c.InsertValue(2, 2, -4);

	EXPECT_TRUE((matrix_csr_test_a - matrix_csr_test_b) == matrix_csr_test_c);
}

TEST(matrix_csr, transpose) {
	zgCFD::CsrMatrix matrix_csr_test_a(4, 4);
	zgCFD::CsrMatrix matrix_csr_test_b(4, 4);
	zgCFD::CsrMatrix matrix_csr_test_c(4, 4);

	matrix_csr_test_a.InsertValue(0, 0, 1);
	matrix_csr_test_a.InsertValue(0, 1, 2);
	matrix_csr_test_a.InsertValue(0, 2, 3);
	matrix_csr_test_a.InsertValue(0, 3, 4);
	matrix_csr_test_a.InsertValue(1, 0, 6);
	matrix_csr_test_a.InsertValue(1, 1, 7);
	matrix_csr_test_a.InsertValue(1, 2, 3);
	matrix_csr_test_a.InsertValue(1, 3, 2);
	matrix_csr_test_a.InsertValue(2, 0, 4);
	matrix_csr_test_a.InsertValue(2, 1, 4);
	matrix_csr_test_a.InsertValue(2, 2, 3);
	matrix_csr_test_a.InsertValue(2, 3, 5);
	matrix_csr_test_a.InsertValue(3, 0, 2);
	matrix_csr_test_a.InsertValue(3, 1, 2);
	matrix_csr_test_a.InsertValue(3, 2, 1);
	matrix_csr_test_a.InsertValue(3, 3, 1);

	matrix_csr_test_b.InsertValue(0, 0, 1);
	matrix_csr_test_b.InsertValue(0, 1, 6);
	matrix_csr_test_b.InsertValue(0, 2, 4);
	matrix_csr_test_b.InsertValue(0, 3, 2);
	matrix_csr_test_b.InsertValue(1, 0, 2);
	matrix_csr_test_b.InsertValue(1, 1, 7);
	matrix_csr_test_b.InsertValue(1, 2, 4);
	matrix_csr_test_b.InsertValue(1, 3, 2);
	matrix_csr_test_b.InsertValue(2, 0, 3);
	matrix_csr_test_b.InsertValue(2, 1, 3);
	matrix_csr_test_b.InsertValue(2, 2, 3);
	matrix_csr_test_b.InsertValue(2, 3, 1);
	matrix_csr_test_b.InsertValue(3, 0, 4);
	matrix_csr_test_b.InsertValue(3, 1, 2);
	matrix_csr_test_b.InsertValue(3, 2, 5);
	matrix_csr_test_b.InsertValue(3, 3, 1);


	matrix_csr_test_c = matrix_csr_test_a.Transpose();

	EXPECT_TRUE(matrix_csr_test_c == matrix_csr_test_b);
}

TEST(matrix_csr, number_of_values) {
	zgCFD::CsrMatrix matrix_csr_test_a(4, 4);
	zgCFD::CsrMatrix matrix_csr_test_b(4, 4);
	zgCFD::CsrMatrix matrix_csr_test_c(4, 4);

	matrix_csr_test_a.InsertValue(0, 0, 1);
	matrix_csr_test_a.InsertValue(0, 1, 2);
	matrix_csr_test_a.InsertValue(0, 2, 3);
	matrix_csr_test_a.InsertValue(0, 3, 4);
	matrix_csr_test_a.InsertValue(1, 0, 6);
	matrix_csr_test_a.InsertValue(1, 1, 7);
	matrix_csr_test_a.InsertValue(1, 2, 3);
	matrix_csr_test_a.InsertValue(1, 3, 2);
	matrix_csr_test_a.InsertValue(2, 0, 4);
	matrix_csr_test_a.InsertValue(2, 1, 4);
	matrix_csr_test_a.InsertValue(2, 2, 3);
	matrix_csr_test_a.InsertValue(2, 3, 5);
	matrix_csr_test_a.InsertValue(3, 0, 2);
	matrix_csr_test_a.InsertValue(3, 1, 2);
	matrix_csr_test_a.InsertValue(3, 2, 1);
	matrix_csr_test_a.InsertValue(3, 3, 1);

	matrix_csr_test_b.InsertValue(0, 0, 0);
	matrix_csr_test_b.InsertValue(0, 1, 6);
	matrix_csr_test_b.InsertValue(0, 2, 0);
	matrix_csr_test_b.InsertValue(0, 3, 2);
	matrix_csr_test_b.InsertValue(1, 0, 2);
	matrix_csr_test_b.InsertValue(1, 1, 0);
	matrix_csr_test_b.InsertValue(1, 2, 4);
	matrix_csr_test_b.InsertValue(1, 3, 2);
	matrix_csr_test_b.InsertValue(2, 0, 3);
	matrix_csr_test_b.InsertValue(2, 1, 3);
	matrix_csr_test_b.InsertValue(2, 2, 3);
	matrix_csr_test_b.InsertValue(2, 3, 1);
	matrix_csr_test_b.InsertValue(3, 0, 4);
	matrix_csr_test_b.InsertValue(3, 1, 2);
	matrix_csr_test_b.InsertValue(3, 2, 5);
	matrix_csr_test_b.InsertValue(3, 3, 1);

	matrix_csr_test_c.InsertValue(3, 3, 1);

	EXPECT_EQ(16, matrix_csr_test_a.number_of_values());
	EXPECT_EQ(13, matrix_csr_test_b.number_of_values());
	EXPECT_EQ(1, matrix_csr_test_c.number_of_values());
}

TEST(matrix_csr, multiply) {
	zgCFD::CsrMatrix matrix_csr_test_a(4, 4);
	zgCFD::CsrMatrix matrix_csr_test_b(4, 4);
	zgCFD::CsrMatrix matrix_csr_test_c(4, 4);

	matrix_csr_test_a.InsertValue(0, 0, 1);
	matrix_csr_test_a.InsertValue(0, 1, 3);
	matrix_csr_test_a.InsertValue(0, 2, 6);
	matrix_csr_test_a.InsertValue(0, 3, 9);
	matrix_csr_test_a.InsertValue(1, 0, 6);
	matrix_csr_test_a.InsertValue(1, 1, 9);
	matrix_csr_test_a.InsertValue(1, 2, 8);
	matrix_csr_test_a.InsertValue(1, 3, 7);
	matrix_csr_test_a.InsertValue(2, 0, 3);
	matrix_csr_test_a.InsertValue(2, 1, 6);
	matrix_csr_test_a.InsertValue(2, 2, 5);
	matrix_csr_test_a.InsertValue(2, 3, 4);
	matrix_csr_test_a.InsertValue(3, 0, 5);
	matrix_csr_test_a.InsertValue(3, 1, 5);
	matrix_csr_test_a.InsertValue(3, 2, 2);
	matrix_csr_test_a.InsertValue(3, 3, 0);

	matrix_csr_test_b.InsertValue(0, 0, 2);
	matrix_csr_test_b.InsertValue(0, 1, 3);
	matrix_csr_test_b.InsertValue(0, 2, 6);
	matrix_csr_test_b.InsertValue(0, 3, 9);
	matrix_csr_test_b.InsertValue(1, 0, 55);
	matrix_csr_test_b.InsertValue(1, 1, 100);
	matrix_csr_test_b.InsertValue(1, 2, 2);
	matrix_csr_test_b.InsertValue(1, 3, 3);
	matrix_csr_test_b.InsertValue(2, 0, 66);
	matrix_csr_test_b.InsertValue(2, 1, 55);
	matrix_csr_test_b.InsertValue(2, 2, 22);
	matrix_csr_test_b.InsertValue(2, 3, 33);
	matrix_csr_test_b.InsertValue(3, 0, 1);
	matrix_csr_test_b.InsertValue(3, 1, 7);
	matrix_csr_test_b.InsertValue(3, 2, 2);
	matrix_csr_test_b.InsertValue(3, 3, 3);

	matrix_csr_test_c.InsertValue(0, 0, 572);
	matrix_csr_test_c.InsertValue(0, 1, 696);
	matrix_csr_test_c.InsertValue(0, 2, 162);
	matrix_csr_test_c.InsertValue(0, 3, 243);
	matrix_csr_test_c.InsertValue(1, 0, 1042);
	matrix_csr_test_c.InsertValue(1, 1, 1407);
	matrix_csr_test_c.InsertValue(1, 2, 244);
	matrix_csr_test_c.InsertValue(1, 3, 366);
	matrix_csr_test_c.InsertValue(2, 0, 670);
	matrix_csr_test_c.InsertValue(2, 1, 912);
	matrix_csr_test_c.InsertValue(2, 2, 148);
	matrix_csr_test_c.InsertValue(2, 3, 222);
	matrix_csr_test_c.InsertValue(3, 0, 417);
	matrix_csr_test_c.InsertValue(3, 1, 625);
	matrix_csr_test_c.InsertValue(3, 2, 84);
	matrix_csr_test_c.InsertValue(3, 3, 126);

	EXPECT_TRUE((matrix_csr_test_a ^ matrix_csr_test_b) == matrix_csr_test_c);
}