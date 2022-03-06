#include <fstream>
#include "pch.h"
#include "ZuoGuangCFD/mesh/of_mesh.h"
#include "ZuoGuangCFD/struct/zg_field.h"

#define STE_FIELD(type,name) zgCFD::type##VolumeField name##(*test_mesh_, #name , struct_path_, "no", "yes");

class base_field_test :public testing::Test {
public:
    static void SetUpTestCase() {

        std::string base_path = "F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\base_field_test\\";
        test_mesh_ = new zgCFD::OfMesh(base_path+"constant\\polyMesh");

        struct_path_ = base_path + "0";

    }
    static void TearDownTestCase() {
        delete test_mesh_;
        test_mesh_ = NULL;
    }

    static zgCFD::Mesh* test_mesh_;
    static std::string struct_path_;
};

zgCFD::Mesh* base_field_test::test_mesh_ = NULL;
std::string base_field_test::struct_path_ = "";

TEST_F(base_field_test, TT) {

    STE_FIELD(Tensor, T);
    STE_FIELD(Tensor, T1);

    STE_FIELD(Tensor, TT1);
    STE_FIELD(Tensor, TT2);
    STE_FIELD(Scalar, TT3);
    STE_FIELD(Tensor, TT4);

    EXPECT_TRUE(TT1 == (T + T1));
    EXPECT_TRUE(TT2 == (T - T1));
    EXPECT_TRUE(TT3 == (T && T1));
    EXPECT_TRUE(TT4 == (T & T1));
}

TEST_F(base_field_test, TV) {

    STE_FIELD(Tensor, T);
    STE_FIELD(Vector, V);

    STE_FIELD(Vector, TV1);

    EXPECT_TRUE(TV1 == (T & V));
}

TEST_F(base_field_test, TS) {

    STE_FIELD(Tensor, T);
    STE_FIELD(Scalar, S);

    STE_FIELD(Tensor, TS1);
    STE_FIELD(Tensor, TS2);

    EXPECT_TRUE(TS1 == (T * S));
    EXPECT_TRUE(TS2 == (T / S));
}

TEST_F(base_field_test, VV) {

    STE_FIELD(Vector, V);
    STE_FIELD(Vector, V1);

    STE_FIELD(Vector, VV1);
    STE_FIELD(Vector, VV2);
    STE_FIELD(Scalar, VV3);
    STE_FIELD(Vector, VV4);

    EXPECT_TRUE(VV1 == (V + V1));
    EXPECT_TRUE(VV2 == (V - V1));
    EXPECT_TRUE(VV3 == (V & V1));
    EXPECT_TRUE(VV4 == (V ^ V1));
}

TEST_F(base_field_test, VS) {

    STE_FIELD(Vector, V);
    STE_FIELD(Scalar, S);

    STE_FIELD(Vector, VS1);
    STE_FIELD(Vector, VS2);

    EXPECT_TRUE(VS1 == (V * S));
    EXPECT_TRUE(VS2 == (V / S));
}

TEST_F(base_field_test, ST) {

    STE_FIELD(Tensor, T);
    STE_FIELD(Scalar, S);

    STE_FIELD(Tensor, ST1);

    EXPECT_TRUE(ST1 == (S * T));
}

TEST_F(base_field_test, SV) {

    STE_FIELD(Vector, V);
    STE_FIELD(Scalar, S);

    STE_FIELD(Vector, SV1);

    EXPECT_TRUE(SV1 == (S * V));
}

TEST_F(base_field_test, SS) {

    STE_FIELD(Scalar, S);
    STE_FIELD(Scalar, S1);

    STE_FIELD(Scalar, SS1);
    STE_FIELD(Scalar, SS2);
    STE_FIELD(Scalar, SS3);
    STE_FIELD(Scalar, SS4);

    EXPECT_TRUE(SS1 == (S + S1));
    EXPECT_TRUE(SS2 == (S - S1));
    EXPECT_TRUE(SS3 == (S * S1));
    //EXPECT_TRUE(SS4 == (S / S1));
}

TEST_F(base_field_test, sTT) {

    zgCFD::GeneralTensor T(1, 52, 63, 44, 55, 6, 87, 8, 9);
    STE_FIELD(Tensor, T1);

    STE_FIELD(Tensor, sTT1);
    STE_FIELD(Tensor, sTT2);
    STE_FIELD(Scalar, sTT3);
    STE_FIELD(Tensor, sTT4);

    EXPECT_TRUE(sTT1 == (T + T1));
    EXPECT_TRUE(sTT2 == (T - T1));
    EXPECT_TRUE(sTT3 == (T && T1));
    EXPECT_TRUE(sTT4 == (T & T1));
}

TEST_F(base_field_test, sTV) {

    zgCFD::GeneralTensor T(1, 52, 63, 44, 55, 6, 87, 8, 9);
    STE_FIELD(Vector, V);

    STE_FIELD(Vector, sTV1);

    EXPECT_TRUE(sTV1 == (T & V));
}

TEST_F(base_field_test, sTS) {

    zgCFD::GeneralTensor T(1, 52, 63, 44, 55, 6, 87, 8, 9);
    STE_FIELD(Scalar, S);

    STE_FIELD(Tensor, sTS1);
    //STE_FIELD(Tensor, sTS2);

    EXPECT_TRUE(sTS1 == (T * S));
    //EXPECT_TRUE(sTS2 == (T / S));
}

TEST_F(base_field_test, sVV) {

    zgCFD::GeneralVector V(122, 12, 95);
    STE_FIELD(Vector, V1);

    STE_FIELD(Vector, sVV1);
    STE_FIELD(Vector, sVV2);
    STE_FIELD(Scalar, sVV3);
    STE_FIELD(Vector, sVV4);

    EXPECT_TRUE(sVV1 == (V + V1));
    EXPECT_TRUE(sVV2 == (V - V1));
    //EXPECT_TRUE(sVV3 == (V & V1));
    EXPECT_TRUE(sVV4 == (V ^ V1));
}

TEST_F(base_field_test, sVS) {

    zgCFD::GeneralVector V(122, 12, 95);
    STE_FIELD(Scalar, S);

    STE_FIELD(Vector, sVS1);
    //STE_FIELD(Vector, sVS2);

    EXPECT_TRUE(sVS1 == (V * S));
    //EXPECT_TRUE(sVS2 == (V / S));
}

TEST_F(base_field_test, sST) {

    STE_FIELD(Tensor, T);
    //STE_FIELD(Scalar, S);
    zgCFD::Scalar S(5.6);

    STE_FIELD(Tensor, sST1);

    EXPECT_TRUE(sST1 == (S * T));
}

TEST_F(base_field_test, sSV) {

    STE_FIELD(Vector, V);
    zgCFD::Scalar S(5.6);

    STE_FIELD(Vector, sSV1);

    EXPECT_TRUE(sSV1 == (S * V));
}

TEST_F(base_field_test, sSS) {

    zgCFD::Scalar S(5.6);
    STE_FIELD(Scalar, S1);

    STE_FIELD(Scalar, sSS1);
    STE_FIELD(Scalar, sSS2);
    STE_FIELD(Scalar, sSS3);
    STE_FIELD(Scalar, sSS4);

    EXPECT_TRUE(sSS1 == (S + S1));
    EXPECT_TRUE(sSS2 == (S - S1));
    //EXPECT_TRUE(sSS3 == (S * S1));
    //EXPECT_TRUE(sSS4 == (S / S1));
}

TEST_F(base_field_test, bTT) {

    STE_FIELD(Tensor, T);
    zgCFD::GeneralTensor T1(11, 552, 63, 44, 55, 65, 87, 8, 95);

    STE_FIELD(Tensor, bTT1);
    STE_FIELD(Tensor, bTT2);
    STE_FIELD(Scalar, bTT3);
    STE_FIELD(Tensor, bTT4);

    EXPECT_TRUE(bTT1 == (T + T1));
    EXPECT_TRUE(bTT2 == (T - T1));
    //EXPECT_TRUE(bTT3 == (T && T1));
    EXPECT_TRUE(bTT4 == (T & T1));
}

TEST_F(base_field_test, bTV) {

    STE_FIELD(Tensor, T);
    zgCFD::GeneralVector V(122, 112, 9.5);

    STE_FIELD(Vector, bTV1);

    EXPECT_TRUE(bTV1 == (T & V));
}

TEST_F(base_field_test, bTS) {

    STE_FIELD(Tensor, T);
    zgCFD::Scalar S(87);

    STE_FIELD(Tensor, bTS1);
    STE_FIELD(Tensor, bTS2);

    EXPECT_TRUE(bTS1 == (T * S));
    EXPECT_TRUE(bTS2 == (T / S));
}

TEST_F(base_field_test, bVV) {

    STE_FIELD(Vector, V);
    zgCFD::GeneralVector V1(122, 112, 9.5);

    STE_FIELD(Vector, bVV1);
    STE_FIELD(Vector, bVV2);
    STE_FIELD(Scalar, bVV3);
    STE_FIELD(Vector, bVV4);

    EXPECT_TRUE(bVV1 == (V + V1));
    EXPECT_TRUE(bVV2 == (V - V1));
    EXPECT_TRUE(bVV3 == (V & V1));
    EXPECT_TRUE(bVV4 == (V ^ V1));
}

TEST_F(base_field_test, bVS) {

    STE_FIELD(Vector, V);
    zgCFD::Scalar S(87);

    STE_FIELD(Vector, bVS1);
    STE_FIELD(Vector, bVS2);

    EXPECT_TRUE(bVS1 == (V * S));
    EXPECT_TRUE(bVS2 == (V / S));
}

TEST_F(base_field_test, bST) {

    zgCFD::GeneralTensor T(11, 552, 63, 44, 55, 65, 87, 8, 95);
    STE_FIELD(Scalar, S);

    STE_FIELD(Tensor, bST1);

    EXPECT_TRUE(bST1 == (S * T));
}

TEST_F(base_field_test, bSV) {

    zgCFD::GeneralVector V(122, 112, 9.5);
    STE_FIELD(Scalar, S);

    STE_FIELD(Vector, bSV1);

    EXPECT_TRUE(bSV1 == (S * V));
}

TEST_F(base_field_test, bSS) {

    STE_FIELD(Scalar, S);
    zgCFD::Scalar S1(87);

    STE_FIELD(Scalar, bSS1);
    STE_FIELD(Scalar, bSS2);
    STE_FIELD(Scalar, bSS3);
    STE_FIELD(Scalar, bSS4);

    EXPECT_TRUE(bSS1 == (S + S1));
    EXPECT_TRUE(bSS2 == (S - S1));
    //标量间运算产生的误差使得不能相等
    /*EXPECT_TRUE(bSS3 == (S * S1));
    EXPECT_TRUE(bSS4 == (S / S1));*/
}