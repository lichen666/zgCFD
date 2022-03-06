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
        std::cout << "points_file_faces_test�ļ���ʧ��" << std::endl;
        return;
    }

    EXPECT_EQ(1074, of_mesh_test::test_mesh_->number_of_points_);

    double tmp_double_test;
    int tmp_int_test;

    for (int i = 0; i < 1074; i++)
    {
        points_file_centroid_test >> tmp_int_test;
        points_file_centroid_test >> tmp_double_test;
        EXPECT_GT(0.0001,abs(tmp_double_test-of_mesh_test::test_mesh_->points_[i].x_)) << "��" << i << "�����������";
        points_file_centroid_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->points_[i].y_)) << "��" << i << "�����������";
        points_file_centroid_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->points_[i].z_)) << "��" << i << "�����������";
    }

    int tmp_int_2_test;
    for (int i = 0; i < 1074; i++)
    {
        points_file_elements_test >> tmp_int_test;
        for (int j = 0; j < of_mesh_test::test_mesh_->points_[tmp_int_test].elements_.size(); j++)
        {
            points_file_elements_test >> tmp_int_2_test;
            EXPECT_EQ(tmp_int_2_test-1, of_mesh_test::test_mesh_->points_[tmp_int_test].elements_[j]) << "��" << i << "����ĵ�" << j << "��element��������";
        }
    }

    for (int i = 0; i < 482; i++)
    {
        points_file_faces_test >> tmp_int_test;
        for (int j = 0; j < of_mesh_test::test_mesh_->points_[tmp_int_test].faces_.size(); j++)
        {
            points_file_faces_test >> tmp_int_2_test;
            EXPECT_EQ(tmp_int_2_test - 1, of_mesh_test::test_mesh_->points_[tmp_int_test].faces_[j]) << "��" << tmp_int_test << "����ĵ�" << j << "��face��������";
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
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].area_)) << "��" << i << "�������������";
    }

    for (int i = 0; i < 3290; i++)
    {
        faces_file_centroid_test >> tmp_int_test;
        faces_file_centroid_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].centroid_.x_)) << "��" << i << "����������������";
        faces_file_centroid_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].centroid_.y_)) << "��" << i << "����������������";
        faces_file_centroid_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].centroid_.z_)) << "��" << i << "����������������";
    }

    for (int i = 0; i < 3290; i++)
    {
        faces_file_cnecn_test >> tmp_int_test;
        faces_file_cnecn_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].cn_.x())) << "��" << i << "�����xcn��������";
        faces_file_cnecn_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].cn_.y())) << "��" << i << "�����ycn��������";
        faces_file_cnecn_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].cn_.z())) << "��" << i << "�����zcn��������";

        faces_file_cnecn_test >> tmp_int_test;
        faces_file_cnecn_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].e_cn_.x())) << "��" << i << "�����xecn��������";
        faces_file_cnecn_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].e_cn_.y())) << "��" << i << "�����yecn��������";
        faces_file_cnecn_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].e_cn_.z())) << "��" << i << "�����zecn��������";
    }

    for (int i = 0; i < 3290; i++)
    {
        faces_file_gdiff_test >> tmp_int_test;
        faces_file_gdiff_test >> tmp_double_test;
        EXPECT_GT(1, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].g_diff_)) << "��" << i << "����g_diff_��������";
    }

    for (int i = 0; i < 3290; i++)
    {
        faces_file_gf_test >> tmp_int_test;
        faces_file_gf_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].gf_)) << "��" << i << "����gf_��������";
    }

    for (int i = 0; i < 3290; i++)
    {
        faces_file_gf_test >> tmp_int_test;
        faces_file_gf_test >> tmp_int_test;
        EXPECT_GT(tmp_int_test, of_mesh_test::test_mesh_->faces_[i].owner_) << "��" << i << "����owner��������";
        faces_file_gf_test >> tmp_int_test;
        EXPECT_GT(tmp_int_test, of_mesh_test::test_mesh_->faces_[i].neighbour_) << "��" << i << "����neighbour��������";
    }

    int tmp_int_2_test;
    for (int i = 0; i < 3290; i++)
    {
        faces_file_points_test >> tmp_int_test;
        for (int j = 0; j < of_mesh_test::test_mesh_->faces_[tmp_int_test].points_.size(); j++)
        {
            faces_file_points_test >> tmp_int_2_test;
            EXPECT_EQ(tmp_int_2_test - 1, of_mesh_test::test_mesh_->faces_[tmp_int_test].points_[j]) << "��" << tmp_int_test << "����ĵ�" << j << "��point��������";
        }
    }

    for (int i = 0; i < 3290; i++)
    {
        faces_file_sf_test >> tmp_int_test;
        faces_file_sf_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].sf_.x())) << "��" << i << "�����sf_.x��������";
        faces_file_sf_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].sf_.y())) << "��" << i << "�����sf_.y��������";
        faces_file_sf_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].sf_.z())) << "��" << i << "�����sf_.z��������";
    }

    for (int i = 0; i < 3290; i++)
    {
        faces_file_t_test >> tmp_int_test;
        faces_file_t_test >> tmp_double_test;
        EXPECT_GT(1.1, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].t_.x())) << "��" << i << "�����t_.x��������";
        faces_file_t_test >> tmp_double_test;
        EXPECT_GT(1.1, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].t_.y())) << "��" << i << "�����t_.y��������";
        faces_file_t_test >> tmp_double_test;
        EXPECT_GT(1.1, abs(tmp_double_test - of_mesh_test::test_mesh_->faces_[i].t_.z())) << "��" << i << "�����t_.z��������";
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
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->elements_[i].centroid_.x_)) << "��" << i << "��element�������������";
        elements_file_centroid_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->elements_[i].centroid_.y_)) << "��" << i << "��element�������������";
        elements_file_centroid_test >> tmp_double_test;
        EXPECT_GT(0.0001, abs(tmp_double_test - of_mesh_test::test_mesh_->elements_[i].centroid_.z_)) << "��" << i << "��element�������������";
    }

    int tmp_int_2_test;
    for (int i = 0; i < 918; i++)
    {
        elements_file_faces_test >> tmp_int_test;
        for (int j = 0; j < of_mesh_test::test_mesh_->elements_[tmp_int_test].faces_.size(); j++)
        {
            elements_file_faces_test >> tmp_int_2_test;
            EXPECT_EQ(tmp_int_2_test-1, of_mesh_test::test_mesh_->elements_[tmp_int_test].faces_[j]) << "��" << tmp_int_test << "��element�ĵ�"<<j<<"�����������";
        }
    }

    for (int i = 0; i < 918; i++)
    {
        elements_file_facesign_test >> tmp_int_test;
        for (int j = 0; j < of_mesh_test::test_mesh_->elements_[tmp_int_test].face_sign_.size(); j++)
        {
            elements_file_facesign_test >> tmp_int_2_test;
            EXPECT_EQ(tmp_int_2_test, of_mesh_test::test_mesh_->elements_[tmp_int_test].face_sign_[j]) << "��" << tmp_int_test << "��element�ĵ�" << j << "������ų�������";
        }
    }

    int tmp_int_3_test;
    for (int i = 0; i < 918; i++)
    {
        elements_file_neighbours_test >> tmp_int_test;
        elements_file_neighbours_test >> tmp_int_2_test;

        EXPECT_EQ(tmp_int_2_test, of_mesh_test::test_mesh_->elements_[tmp_int_test].number_of_neighbours_) << "��" << tmp_int_test << "��element��number_of_neighbours��������";

        for (int j = 0; j < of_mesh_test::test_mesh_->elements_[tmp_int_test].number_of_neighbours_; j++)
        {
            elements_file_neighbours_test >> tmp_int_3_test;
            EXPECT_EQ(tmp_int_3_test-1, of_mesh_test::test_mesh_->elements_[tmp_int_test].neighbours_[j]) << "��" << tmp_int_test << "��element�ĵ�" << j << "��neighbour��������";
        }
    }

    for (int i = 0; i < 918; i++)
    {
        elements_file_points_test >> tmp_int_test;
        for (int j = 0; j < of_mesh_test::test_mesh_->elements_[tmp_int_test].points_.size(); j++)
        {
            elements_file_points_test >> tmp_int_2_test;
            EXPECT_EQ(tmp_int_2_test - 1, of_mesh_test::test_mesh_->elements_[tmp_int_test].points_[j]) << "��" << tmp_int_test << "��element�ĵ�" << j << "��points��������";
        }
    }

    for (int i = 0; i < 918; i++)
    {
        elements_file_volume_test >> tmp_int_test;
        elements_file_volume_test >> tmp_double_test;
        EXPECT_GT(0.0001 , abs(tmp_double_test-of_mesh_test::test_mesh_->elements_[tmp_int_test].volume_)) << "��" << tmp_int_test << "��element�ĵ������������";
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