#include "pch.h"
#include "ZuoGuangCFD/mesh/of_mesh.h"
#include <fstream>

class of_mesh_test :public testing::Test {
public:
    static void SetUpTestCase() {
        test_mesh_ = new zgCFD::OfMesh("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\of_mesh_test\\elbow\\constant\\polyMesh");
    }
    static void TearDownTestCase() {
        delete test_mesh_;
        test_mesh_ = NULL;
    }

    static zgCFD::OfMesh* test_mesh_;
};

zgCFD::OfMesh* of_mesh_test::test_mesh_ = NULL;

TEST_F(of_mesh_test, points) {

    std::fstream  points_file_centroid_test;
    std::fstream  points_file_elements_test;
    std::fstream  points_file_faces_test;

    points_file_centroid_test.open("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\of_mesh_test\\point_centroid.txt", std::ios::in);
    points_file_elements_test.open("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\of_mesh_test\\point_elements.txt", std::ios::in);
    points_file_faces_test.open("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\of_mesh_test\\point_faces.txt", std::ios::in);

    if (!points_file_centroid_test.is_open())
    {
        std::cout << "points_file_centroid_test" << std::endl;
        return;
    }
    if (!points_file_elements_test.is_open())
    {
        std::cout << "points_file_elements_test" << std::endl;
        return;
    }
    if (!points_file_faces_test.is_open())
    {
        std::cout << "points_file_faces_test文件打开失败" << std::endl;
        return;
    }

    EXPECT_EQ(1074, of_mesh_test::test_mesh_->number_of_points_);

    double tmp_double_test;
    int tmp_int_test;

    for (int i = 0; i < 1074; i++)
    {
        points_file_centroid_test >> tmp_int_test;
        points_file_centroid_test >> tmp_double_test;
        EXPECT_GT(0.0001,abs(tmp_double_test-of_mesh_test::test_mesh_->points_[i].x_)) << "第" << i << "个点出了问题";
        points_file_centroid_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->points_[i].y_)) << "第" << i << "个点出了问题";
        points_file_centroid_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->points_[i].z_)) << "第" << i << "个点出了问题";
    }

    int tmp_int_2_test;
    for (int i = 0; i < 1074; i++)
    {
        points_file_elements_test >> tmp_int_test;
        for (int j = 0; j < of_mesh_test::test_mesh_->points_[tmp_int_test].elements_.size(); j++)
        {
            points_file_elements_test >> tmp_int_2_test;
            EXPECT_EQ(tmp_int_2_test-1, of_mesh_test::test_mesh_->points_[tmp_int_test].elements_[j]) << "第" << i << "个点的第" << j << "个element出了问题";
        }
    }

    for (int i = 0; i < 482; i++)
    {
        points_file_faces_test >> tmp_int_test;
        for (int j = 0; j < of_mesh_test::test_mesh_->points_[tmp_int_test].faces_.size(); j++)
        {
            points_file_faces_test >> tmp_int_2_test;
            EXPECT_EQ(tmp_int_2_test - 1, of_mesh_test::test_mesh_->points_[tmp_int_test].faces_[j]) << "第" << tmp_int_test << "个点的第" << j << "个face出了问题";
        }
    }

    points_file_centroid_test.close();
    points_file_elements_test.close();
    points_file_faces_test.close();
}

TEST_F(of_mesh_test, faces) {

    std::fstream  faces_file_area_test;
    std::fstream  faces_file_centroid_test;
    std::fstream  faces_file_cnecn_test;
    std::fstream  faces_file_gdiff_test;
    std::fstream  faces_file_gf_test;
    std::fstream  faces_file_on_test;
    std::fstream  faces_file_points_test;
    std::fstream  faces_file_sf_test;
    std::fstream  faces_file_t_test;

    faces_file_area_test.open("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\of_mesh_test\\faces_area.txt", std::ios::in);
    faces_file_centroid_test.open("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\of_mesh_test\\faces_centroid.txt", std::ios::in);
    faces_file_cnecn_test.open("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\of_mesh_test\\faces_cn_ecn.txt", std::ios::in);
    faces_file_gdiff_test.open("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\of_mesh_test\\faces_gdiff.txt", std::ios::in);
    faces_file_gf_test.open("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\of_mesh_test\\faces_gf.txt", std::ios::in);
    faces_file_on_test.open("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\of_mesh_test\\faces_on.txt", std::ios::in);
    faces_file_points_test.open("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\of_mesh_test\\faces_points.txt", std::ios::in);
    faces_file_sf_test.open("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\of_mesh_test\\faces_sf.txt", std::ios::in);
    faces_file_t_test.open("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\of_mesh_test\\faces_t.txt", std::ios::in);

    if (!faces_file_area_test.is_open())
    {
        std::cout << "faces_file_area_test" << std::endl;
        return;
    }
    if (!faces_file_centroid_test.is_open())
    {
        std::cout << "faces_file_centroid_test" << std::endl;
        return;
    }
    if (!faces_file_cnecn_test.is_open())
    {
        std::cout << "faces_file_cnecn_test" << std::endl;
        return;
    }
    if (!faces_file_gdiff_test.is_open())
    {
        std::cout << "faces_file_gdiff_test" << std::endl;
        return;
    }
    if (!faces_file_gf_test.is_open())
    {
        std::cout << "faces_file_gf_test" << std::endl;
        return;
    }
    if (!faces_file_on_test.is_open())
    {
        std::cout << "faces_file_on_test" << std::endl;
        return;
    }
    if (!faces_file_sf_test.is_open())
    {
        std::cout << "faces_file_sf_test" << std::endl;
        return;
    }
    if (!faces_file_t_test.is_open())
    {
        std::cout << "faces_file_t_test" << std::endl;
        return;
    }

    EXPECT_EQ(3290, of_mesh_test::test_mesh_->number_of_faces_);

    double tmp_double_test;
    int tmp_int_test;

    for (int i = 0; i < 3290; i++)
    {
        faces_file_area_test >> tmp_int_test;
        faces_file_area_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].area_)) << "第" << i << "个面积出了问题";
    }

    for (int i = 0; i < 3290; i++)
    {
        faces_file_centroid_test >> tmp_int_test;
        faces_file_centroid_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].centroid_.x_)) << "第" << i << "个面的坐标出了问题";
        faces_file_centroid_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].centroid_.y_)) << "第" << i << "个面的坐标出了问题";
        faces_file_centroid_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].centroid_.z_)) << "第" << i << "个面的坐标出了问题";
    }

    for (int i = 0; i < 3290; i++)
    {
        faces_file_cnecn_test >> tmp_int_test;
        faces_file_cnecn_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].cn_.x())) << "第" << i << "个面的xcn出了问题";
        faces_file_cnecn_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].cn_.y())) << "第" << i << "个面的ycn出了问题";
        faces_file_cnecn_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].cn_.z())) << "第" << i << "个面的zcn出了问题";

        faces_file_cnecn_test >> tmp_int_test;
        faces_file_cnecn_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].e_cn_.x())) << "第" << i << "个面的xecn出了问题";
        faces_file_cnecn_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].e_cn_.y())) << "第" << i << "个面的yecn出了问题";
        faces_file_cnecn_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].e_cn_.z())) << "第" << i << "个面的zecn出了问题";
    }

    for (int i = 0; i < 3290; i++)
    {
        faces_file_gdiff_test >> tmp_int_test;
        faces_file_gdiff_test >> tmp_double_test;
        EXPECT_GT(1, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].g_diff_)) << "第" << i << "个面g_diff_出了问题";
    }

    for (int i = 0; i < 3290; i++)
    {
        faces_file_gf_test >> tmp_int_test;
        faces_file_gf_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].gf_)) << "第" << i << "个面gf_出了问题";
    }

    for (int i = 0; i < 3290; i++)
    {
        faces_file_gf_test >> tmp_int_test;
        faces_file_gf_test >> tmp_int_test;
        EXPECT_GT(tmp_int_test, of_mesh_test::test_mesh_->faces_[i].owner_) << "第" << i << "个面owner出了问题";
        faces_file_gf_test >> tmp_int_test;
        EXPECT_GT(tmp_int_test, of_mesh_test::test_mesh_->faces_[i].neighbour_) << "第" << i << "个面neighbour出了问题";
    }

    int tmp_int_2_test;
    for (int i = 0; i < 3290; i++)
    {
        faces_file_points_test >> tmp_int_test;
        for (int j = 0; j < of_mesh_test::test_mesh_->faces_[tmp_int_test].points_.size(); j++)
        {
            faces_file_points_test >> tmp_int_2_test;
            EXPECT_EQ(tmp_int_2_test - 1, of_mesh_test::test_mesh_->faces_[tmp_int_test].points_[j]) << "第" << tmp_int_test << "个面的第" << j << "个point出了问题";
        }
    }

    for (int i = 0; i < 3290; i++)
    {
        faces_file_sf_test >> tmp_int_test;
        faces_file_sf_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].sf_.x())) << "第" << i << "个面的sf_.x出了问题";
        faces_file_sf_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].sf_.y())) << "第" << i << "个面的sf_.y出了问题";
        faces_file_sf_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].sf_.z())) << "第" << i << "个面的sf_.z出了问题";
    }

    for (int i = 0; i < 3290; i++)
    {
        faces_file_t_test >> tmp_int_test;
        faces_file_t_test >> tmp_double_test;
        EXPECT_GT(1.1, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].t_.x())) << "第" << i << "个面的t_.x出了问题";
        faces_file_t_test >> tmp_double_test;
        EXPECT_GT(1.1, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].t_.y())) << "第" << i << "个面的t_.y出了问题";
        faces_file_t_test >> tmp_double_test;
        EXPECT_GT(1.1, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].t_.z())) << "第" << i << "个面的t_.z出了问题";
    }

    faces_file_area_test.close();
    faces_file_centroid_test.close();
    faces_file_cnecn_test.close();
    faces_file_gdiff_test.close();
    faces_file_gf_test.close();
    faces_file_on_test.close();
    faces_file_points_test.close();
    faces_file_sf_test.close();
    faces_file_t_test.close();
}

TEST_F(of_mesh_test, elements) {

    std::fstream  elements_file_centroid_test;
    std::fstream  elements_file_faces_test;
    std::fstream  elements_file_facesign_test;
    std::fstream  elements_file_neighbours_test;
    std::fstream  elements_file_points_test;
    std::fstream  elements_file_volume_test;

    elements_file_centroid_test.open("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\of_mesh_test\\elements_centroid.txt", std::ios::in);
    elements_file_faces_test.open("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\of_mesh_test\\elements_faces.txt", std::ios::in);
    elements_file_facesign_test.open("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\of_mesh_test\\elements_facesign.txt", std::ios::in);
    elements_file_neighbours_test.open("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\of_mesh_test\\elements_neighbours.txt", std::ios::in);
    elements_file_points_test.open("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\of_mesh_test\\elements_points.txt", std::ios::in);
    elements_file_volume_test.open("F:\\027.CFDsoftware\\ZuoGuangCFD\\test\\ZuoGuangCFD_test\\of_mesh_test\\elements_volume.txt", std::ios::in);

    if (!elements_file_centroid_test.is_open())
    {
        std::cout << "elements_file_centroid_test" << std::endl;
        return;
    }
    if (!elements_file_faces_test.is_open())
    {
        std::cout << "elements_file_faces_test" << std::endl;
        return;
    }
    if (!elements_file_facesign_test.is_open())
    {
        std::cout << "elements_file_facesign_test" << std::endl;
        return;
    }
    if (!elements_file_neighbours_test.is_open())
    {
        std::cout << "elements_file_neighbours_test" << std::endl;
        return;
    }
    if (!elements_file_points_test.is_open())
    {
        std::cout << "elements_file_points_test" << std::endl;
        return;
    }
    if (!elements_file_volume_test.is_open())
    {
        std::cout << "elements_file_volume_test" << std::endl;
        return;
    }

    EXPECT_EQ(918, of_mesh_test::test_mesh_->number_of_elements_);

    int tmp_int_test;
    double tmp_double_test;
    for (int i = 0; i < 918; i++)
    {
        elements_file_centroid_test >> tmp_int_test;
        elements_file_centroid_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->elements_[i].centroid_.x_)) << "第" << i << "个element的坐标出了问题";
        elements_file_centroid_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->elements_[i].centroid_.y_)) << "第" << i << "个element的坐标出了问题";
        elements_file_centroid_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->elements_[i].centroid_.z_)) << "第" << i << "个element的坐标出了问题";
    }

    int tmp_int_2_test;
    for (int i = 0; i < 918; i++)
    {
        elements_file_faces_test >> tmp_int_test;
        for (int j = 0; j < of_mesh_test::test_mesh_->elements_[tmp_int_test].faces_.size(); j++)
        {
            elements_file_faces_test >> tmp_int_2_test;
            EXPECT_EQ(tmp_int_2_test-1, of_mesh_test::test_mesh_->elements_[tmp_int_test].faces_[j]) << "第" << tmp_int_test << "个element的第"<<j<<"个面出了问题";
        }
    }

    for (int i = 0; i < 918; i++)
    {
        elements_file_facesign_test >> tmp_int_test;
        for (int j = 0; j < of_mesh_test::test_mesh_->elements_[tmp_int_test].face_sign_.size(); j++)
        {
            elements_file_facesign_test >> tmp_int_2_test;
            EXPECT_EQ(tmp_int_2_test, of_mesh_test::test_mesh_->elements_[tmp_int_test].face_sign_[j]) << "第" << tmp_int_test << "个element的第" << j << "个面符号出了问题";
        }
    }

    int tmp_int_3_test;
    for (int i = 0; i < 918; i++)
    {
        elements_file_neighbours_test >> tmp_int_test;
        elements_file_neighbours_test >> tmp_int_2_test;

        EXPECT_EQ(tmp_int_2_test, of_mesh_test::test_mesh_->elements_[tmp_int_test].number_of_neighbours_) << "第" << tmp_int_test << "个element的number_of_neighbours出了问题";

        for (int j = 0; j < of_mesh_test::test_mesh_->elements_[tmp_int_test].number_of_neighbours_; j++)
        {
            elements_file_neighbours_test >> tmp_int_3_test;
            EXPECT_EQ(tmp_int_3_test-1, of_mesh_test::test_mesh_->elements_[tmp_int_test].neighbours_[j]) << "第" << tmp_int_test << "个element的第" << j << "个neighbour出了问题";
        }
    }

    for (int i = 0; i < 918; i++)
    {
        elements_file_points_test >> tmp_int_test;
        for (int j = 0; j < of_mesh_test::test_mesh_->elements_[tmp_int_test].points_.size(); j++)
        {
            elements_file_points_test >> tmp_int_2_test;
            EXPECT_EQ(tmp_int_2_test - 1, of_mesh_test::test_mesh_->elements_[tmp_int_test].points_[j]) << "第" << tmp_int_test << "个element的第" << j << "个points出了问题";
        }
    }

    for (int i = 0; i < 918; i++)
    {
        elements_file_volume_test >> tmp_int_test;
        elements_file_volume_test >> tmp_double_test;
        EXPECT_GT(0.0001 , abs(tmp_double_test-of_mesh_test::test_mesh_->elements_[tmp_int_test].volume_)) << "第" << tmp_int_test << "个element的第体积出了问题";
    }

    elements_file_centroid_test.close();
    elements_file_faces_test.close();
    elements_file_facesign_test.close();
    elements_file_neighbours_test.close();
    elements_file_points_test.close();
    elements_file_volume_test.close();
}

TEST_F(of_mesh_test, boundaries) {

    EXPECT_EQ(6, of_mesh_test::test_mesh_->number_of_boundaries_);
}

TEST_F(of_mesh_test, interiorfaces) {

    EXPECT_EQ(1300, of_mesh_test::test_mesh_->number_of_interior_faces_);
}

TEST_F(of_mesh_test, bfaces) {

    EXPECT_EQ(1990, of_mesh_test::test_mesh_->number_of_boundary_faces_);
}