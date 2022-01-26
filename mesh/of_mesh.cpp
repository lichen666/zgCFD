#include "of_mesh.h"
#include <iostream>
#include <sstream>
#include <cmath>

namespace zgCFD {

	OfMesh::OfMesh(const std::string& case_directory) {
		case_directory_ = case_directory;
		ReadPointsFile();
		ReadFacesFile();
		ReadOwnerFile();			
		ReadNeighbourFile();
		ReadBoundaryFile();

		number_of_interior_faces_ = number_of_neighbours_;

		SetElements();

		number_of_boundary_faces_ = number_of_faces_ - number_of_interior_faces_;

		SetPointConnectinities();
		ProcessOFMesh();
		SetFaceWeightingFactor();
	}

	//读取点文件
	void OfMesh::ReadPointsFile(){

		points_file_.open(case_directory_ + "\\points", std::ios::in);

		if (!points_file_.is_open())
		{
			std::cout << "points文件打开失败" << std::endl;
			return;
		}

		char tmp_char_;
		std::string tmp_string_;

		//过滤掉前面的文件头及空行
		for (int i = 0; i < 18; i++)
		{
			std::getline(points_file_, tmp_string_);
		}
		//读取点的数量
		points_file_ >> number_of_points_;

		//定义点容器大小
		points_.resize(number_of_points_);

		//过滤掉空行及"("
		for (int i = 0; i < 2; i++)
		{
			std::getline(points_file_, tmp_string_);
		}
		for (int i = 0; i < number_of_points_; i++) {

			point tmp_point_;

			points_file_ >> tmp_char_;//"("
			points_file_ >> tmp_point_.x_;
			points_file_ >> tmp_point_.y_;
			points_file_ >> tmp_point_.z_;

			tmp_point_.index_ = i;

			points_[i] = tmp_point_;

			std::getline(points_file_, tmp_string_);//")"及行末
		}

		points_file_.close();
	}

	//读取面文件
	void OfMesh::ReadFacesFile(){

		faces_file_.open(case_directory_ + "\\faces", std::ios::in);

		if (!faces_file_.is_open())
		{
			std::cout << "faces文件打开失败" << std::endl;
			return;
		}

		char tmp_char_;
		std::string tmp_string_;
		int tmp_index_of_points_;
		

		//过滤掉前面的文件头及空行
		for (int i = 0; i < 18; i++)
		{
			std::getline(faces_file_, tmp_string_);
		}
		//读取面的数量
		faces_file_ >> number_of_faces_;

		//定义点容器大小
		faces_.resize(number_of_faces_);

		//过滤掉空行及"("
		for (int i = 0; i < 2; i++)
		{
			std::getline(faces_file_, tmp_string_);
		}

		for (int i = 0; i < number_of_faces_; i++) {

			int tmp_num_of_points_;
			face tmp_face_;

			faces_file_ >> tmp_num_of_points_;
			tmp_face_.points_.resize(tmp_num_of_points_);

			faces_file_ >> tmp_char_;//"("

			for (int j = 0; j < tmp_num_of_points_; j++) {
				faces_file_ >> tmp_index_of_points_;
				tmp_face_.points_[j] = tmp_index_of_points_;
			}

			tmp_face_.index_ = i;

			faces_[i] = tmp_face_;

			faces_file_ >> tmp_char_;//")"
		}

		faces_file_.close();
	}

	//读取owner文件
	void OfMesh::ReadOwnerFile(){
	
		owner_file_.open(case_directory_ + "\\owner", std::ios::in);

		if (!owner_file_.is_open())
		{
			std::cout << "owner文件打开失败" << std::endl;
			return;
		}

		std::string tmp_string_;
		int tmp_max_of_index = 0;

		//过滤掉前面的文件头及空行
		for (int i = 0; i < 19; i++)
		{
			std::getline(owner_file_, tmp_string_);
		}
		//读取owner的数量
		owner_file_ >> number_of_owners_;

		owner_file_ >> tmp_string_;//"("

		for (int i = 0; i < number_of_owners_; i++)
		{
			int tmp_index_of_owner_;
			owner_file_ >> tmp_index_of_owner_;
			faces_[i].owner_ = tmp_index_of_owner_;
			if (tmp_max_of_index < tmp_index_of_owner_) tmp_max_of_index = tmp_index_of_owner_;
		}

		//网格的数量
		number_of_elements_ = tmp_max_of_index + 1;

		owner_file_.close();
	}

	//读取neighbour文件
	void OfMesh::ReadNeighbourFile(){
	
		neighbour_file_.open(case_directory_ + "\\neighbour", std::ios::in);

		if (!neighbour_file_.is_open())
		{
			std::cout << "neighbour文件打开失败" << std::endl;
			return;
		}

		std::string tmp_string_;

		//过滤掉前面的文件头及空行
		for (int i = 0; i < 19; i++)
		{
			std::getline(neighbour_file_, tmp_string_);
		}
		//读取neighbour的数量
		neighbour_file_ >> number_of_neighbours_;

		neighbour_file_ >> tmp_string_;//"("

		//内部面的数量和neighbour的数量相等
		number_of_interior_faces_ = number_of_neighbours_;

		for (int i = 0; i < number_of_neighbours_; i++)
		{
			int tmp_index_of_neighbour_;
			neighbour_file_ >> tmp_index_of_neighbour_;
			faces_[i].neighbour_ = tmp_index_of_neighbour_;
		}

		neighbour_file_.close();
	}

	//读取边界
	void OfMesh::ReadBoundaryFile(){
	
		boundary_file_.open(case_directory_ + "\\boundary", std::ios::in);

		if (!boundary_file_.is_open())
		{
			std::cout << "boundary文件打开失败" << std::endl;
			return;
		}

		char tmp_char_;
		std::string tmp_string_;
		std::stringstream tmp_sstream_;

		//过滤掉前面的文件头及空行
		for (int i = 0; i < 17; i++)
		{
			std::getline(boundary_file_, tmp_string_);
		}
		//读取边界的数量
		boundary_file_ >> number_of_boundaries_;
		boundaries_.resize(number_of_boundaries_);

		//过滤掉空行及"("
		for (int i = 0; i < 2; i++)
		{
			std::getline(boundary_file_, tmp_string_);
		}

		for (int i = 0; i < number_of_boundaries_; i++) {

			boundary tmp_boundary_;
			boundary_file_ >> tmp_boundary_.boundary_name_;

			boundary_file_ >> tmp_string_;//"{"

			boundary_file_ >> tmp_string_;//"type"
			tmp_string_.clear();
			boundary_file_ >> tmp_char_;
			do
			{
				tmp_string_ += tmp_char_;
				boundary_file_ >> tmp_char_;
			} while (tmp_char_ != ';');
			tmp_sstream_ << tmp_string_ << " ";
			tmp_sstream_ >> tmp_boundary_.type_of_boundary_;

			boundary_file_ >> tmp_string_;//"inGroups"
			if (tmp_string_ == "inGroups")
			{

				tmp_string_.clear();
				boundary_file_ >> tmp_char_;
				do
				{
					tmp_string_ += tmp_char_;
					boundary_file_ >> tmp_char_;
				} while (tmp_char_ != ';');

				boundary_file_ >> tmp_string_;//"nFaces"
				tmp_string_.clear();
				boundary_file_ >> tmp_char_;
				do
				{
					tmp_string_ += tmp_char_;
					boundary_file_ >> tmp_char_;
				} while (tmp_char_ != ';');
				tmp_sstream_ << tmp_string_ << " ";
				tmp_sstream_ >> tmp_boundary_.number_of_faces_;
			}
			else//"nFaces"
			{

				tmp_string_.clear();
				boundary_file_ >> tmp_char_;
				do
				{
					tmp_string_ += tmp_char_;
					boundary_file_ >> tmp_char_;
				} while (tmp_char_ != ';');
				tmp_sstream_ << tmp_string_ << " ";
				tmp_sstream_ >> tmp_boundary_.number_of_faces_;
			}

			boundary_file_ >> tmp_string_;//"startFace"
			tmp_string_.clear();
			boundary_file_ >> tmp_char_;
			do
			{
				tmp_string_ += tmp_char_;
				boundary_file_ >> tmp_char_;
			} while (tmp_char_ != ';');
			tmp_sstream_ << tmp_string_ << " ";
			tmp_sstream_ >> tmp_boundary_.start_face_;

			boundary_file_ >> tmp_string_;//"}"

			tmp_boundary_.index_ = i;

			boundaries_[i] = tmp_boundary_;
		}

		faces_file_.close();
	}

	//设置网格
	void OfMesh::SetElements() {

		for (int i = 0; i < number_of_elements_; i++)
		{
			elements_.push_back(element(i));
		}
		elements_.resize(number_of_elements_);

		int tmp_index_of_faces_owner_;
		int tmp_index_of_faces_neighbour_;

		for (int i = 0; i < number_of_interior_faces_; i++)
		{
			tmp_index_of_faces_owner_ = faces_[i].owner_;
			tmp_index_of_faces_neighbour_ = faces_[i].neighbour_;

			elements_[tmp_index_of_faces_owner_].faces_.push_back(i);
			elements_[tmp_index_of_faces_neighbour_].faces_.push_back(i);

			elements_[tmp_index_of_faces_owner_].face_sign_.push_back(1);
			elements_[tmp_index_of_faces_neighbour_].face_sign_.push_back(-1);

			elements_[tmp_index_of_faces_owner_].neighbours_.push_back(tmp_index_of_faces_neighbour_);
			elements_[tmp_index_of_faces_neighbour_].neighbours_.push_back(tmp_index_of_faces_owner_);
		}

		for (int i = number_of_interior_faces_; i < number_of_faces_; i++)
		{
			tmp_index_of_faces_owner_ = faces_[i].owner_;

			elements_[tmp_index_of_faces_owner_].faces_.push_back(i);
			elements_[tmp_index_of_faces_owner_].face_sign_.push_back(1);
		}

		for (int i = 0; i < number_of_elements_; i++)
		{
			elements_[i].number_of_neighbours_ = elements_[i].neighbours_.size();
		}
	}

	//设置节点的连接性
	void OfMesh::SetPointConnectinities() {

		std::vector<int> tmp_points_of_face_;
		std::vector<int> tmp_faces_of_element_;

		//设置每个点对应的面
		for (int i = 0; i < number_of_faces_; i++)
		{
			tmp_points_of_face_ = faces_[i].points_;
			for (std::vector<int>::iterator it = tmp_points_of_face_.begin(); it != tmp_points_of_face_.end(); it++)
			{
				points_[*it].faces_.push_back(i);
			}
		}

		bool tmp_point_is_repeated_ = false;

		//设置每个点对应的网格以及每个网格对应的点
		for (int i = 0; i < number_of_elements_; i++)
		{
			tmp_faces_of_element_ = elements_[i].faces_;
			for (std::vector<int>::iterator it_face = tmp_faces_of_element_.begin(); it_face != tmp_faces_of_element_.end(); it_face++)
			{
				tmp_points_of_face_ = faces_[*it_face].points_;
				for (std::vector<int>::iterator it_point = tmp_points_of_face_.begin(); it_point != tmp_points_of_face_.end(); it_point++)
				{
					for (std::vector<int>::iterator it = elements_[i].points_.begin();  it != elements_[i].points_.end(); it++)//检查是否有重复的点
					{
						if (*it== *it_point)
						{
							tmp_point_is_repeated_ = true;
							break;
						}
					}

					if (tmp_point_is_repeated_==false)
					{
						elements_[i].points_.push_back(*it_point);
						points_[*it_point].elements_.push_back(i);
					}

					tmp_point_is_repeated_ = false;
				}
			}
		}

	}

	//处理网格文件，得到单元面的形心，单元面面积，网格体积，网格形心
	void OfMesh::ProcessOFMesh() {
		//面
		for (int i = 0; i < number_of_faces_; i++)
		{
			point tmp_centre_of_face(0., 0., 0.);//几何中心
			point tmp_centroid_of_face(0., 0., 0.);//形心

			for (std::vector<int>::iterator it = faces_[i].points_.begin(); it != faces_[i].points_.end(); it++)
			{
				tmp_centre_of_face.x_ += points_[*it].x_;
				tmp_centre_of_face.y_ += points_[*it].y_;
				tmp_centre_of_face.z_ += points_[*it].z_;
			}

			int tmp_number_of_faces_points_ = faces_[i].points_.size();

			tmp_centre_of_face.x_ /= tmp_number_of_faces_points_;
			tmp_centre_of_face.y_ /= tmp_number_of_faces_points_;
			tmp_centre_of_face.z_ /= tmp_number_of_faces_points_;

			//构成小三角形的三个点
			point tmp_point1;
			point tmp_point2;
			point tmp_point3;
			Scalar tmp_area = 0.;
			Vector<Scalar> tmp_sf(0., 0., 0.);

			if (tmp_number_of_faces_points_ == 3)
			{
				tmp_point1 = points_[*faces_[i].points_.begin()];
				tmp_point2 = points_[*(faces_[i].points_.begin()+1)];
				tmp_point3 = points_[*(faces_[i].points_.begin()+2)];

				tmp_centroid_of_face = tmp_centre_of_face;
				tmp_sf = 0.5 * ((tmp_point2 - tmp_point1) ^ (tmp_point3 - tmp_point1));
				tmp_area = tmp_sf.Magnitude();
			}
			else
			{
				tmp_point1 = tmp_centre_of_face;
				
				Scalar tmp_area_of_triangle;
				
				for (std::vector<int>::iterator it = faces_[i].points_.begin(); it != faces_[i].points_.end(); it++)
				{
					tmp_point2 = points_[*it];
					if (it == (faces_[i].points_.end()-1))
					{
						tmp_point3 = points_[*faces_[i].points_.begin()];
					}
					else
					{
						tmp_point3 = points_[*(it + 1)];
					}

					point tmp_cemtroid_of_triangle((tmp_point1.x_ + tmp_point2.x_ + tmp_point3.x_) / 3, (tmp_point1.y_ + tmp_point2.y_ + tmp_point3.y_) / 3, (tmp_point1.z_ + tmp_point2.z_ + tmp_point3.z_) / 3);//小三角形的形心
					
					
					Vector<Scalar> tmp_sf_of_triangle = 0.5 * ((tmp_point2 - tmp_point1) ^ (tmp_point3 - tmp_point1));

					tmp_area_of_triangle = tmp_sf_of_triangle.Magnitude();

					tmp_area += tmp_area_of_triangle;
					tmp_sf = tmp_sf + tmp_sf_of_triangle;

					tmp_centroid_of_face.x_ += tmp_area_of_triangle * tmp_cemtroid_of_triangle.x_;
					tmp_centroid_of_face.y_ += tmp_area_of_triangle * tmp_cemtroid_of_triangle.y_;
					tmp_centroid_of_face.z_ += tmp_area_of_triangle * tmp_cemtroid_of_triangle.z_;
				}

				tmp_centroid_of_face.x_ /= tmp_area;
				tmp_centroid_of_face.y_ /= tmp_area;
				tmp_centroid_of_face.z_ /= tmp_area;

			}

			faces_[i].sf_ = tmp_sf;
			faces_[i].area_ = tmp_area;
			faces_[i].centroid_ = tmp_centroid_of_face;
		}//面信息处理完毕

		//体
		for (int i = 0; i < number_of_elements_; i++) {

			point tmp_centre_of_element(0., 0., 0.);//几何中心

			for (std::vector<int>::iterator it = elements_[i].faces_.begin(); it != elements_[i].faces_.end(); it++)
			{
				tmp_centre_of_element.x_ += faces_[*it].centroid_.x_;
				tmp_centre_of_element.y_ += faces_[*it].centroid_.y_;
				tmp_centre_of_element.z_ += faces_[*it].centroid_.z_;
			}

			int tmp_number_of_faces_of_element=elements_[i].faces_.size();

			tmp_centre_of_element.x_ /= tmp_number_of_faces_of_element;
			tmp_centre_of_element.y_ /= tmp_number_of_faces_of_element;
			tmp_centre_of_element.z_ /= tmp_number_of_faces_of_element;

			point tmp_centroid_of_element(0., 0., 0.);//形心
			Scalar tmp_volume = 0.;
			
			for (int i_face = 0; i_face < elements_[i].faces_.size(); i_face++)
			{
				Vector<Scalar> tmp_sf = faces_[elements_[i].faces_[i_face]].sf_ * elements_[i].face_sign_[i_face];
				Vector<Scalar> tmp_d_gf = faces_[elements_[i].faces_[i_face]].centroid_ - tmp_centre_of_element;

				Scalar tmp_volume_of_pyramid = (tmp_sf & tmp_d_gf) / 3;

				point tmp_centroid_of_pyramid;
				tmp_centroid_of_pyramid.x_ = 0.25 * tmp_centre_of_element.x_ + 0.75 * faces_[elements_[i].faces_[i_face]].centroid_.x_;
				tmp_centroid_of_pyramid.y_ = 0.25 * tmp_centre_of_element.y_ + 0.75 * faces_[elements_[i].faces_[i_face]].centroid_.y_;
				tmp_centroid_of_pyramid.z_ = 0.25 * tmp_centre_of_element.z_ + 0.75 * faces_[elements_[i].faces_[i_face]].centroid_.z_;

				tmp_volume += tmp_volume_of_pyramid;
				tmp_centroid_of_element.x_ += tmp_centroid_of_pyramid.x_ * tmp_volume_of_pyramid;
				tmp_centroid_of_element.y_ += tmp_centroid_of_pyramid.y_ * tmp_volume_of_pyramid;
				tmp_centroid_of_element.z_ += tmp_centroid_of_pyramid.z_ * tmp_volume_of_pyramid;
			}

			tmp_centroid_of_element.x_/= tmp_volume;
			tmp_centroid_of_element.y_/= tmp_volume;
			tmp_centroid_of_element.z_/= tmp_volume;

			elements_[i].volume_ = tmp_volume;
			elements_[i].old_volume_ = tmp_volume;
			elements_[i].centroid_ = tmp_centroid_of_element;
		}


	}

/***********************************************************\
面的权重因子基于网格中心点到面的垂直距离进行插值：
g_f = ( d_Cf * e ) /( (d_Cf * e ) + ( d_fF * e) )

其中，对于非正交网格的修正，有最小修正法，正交修正，超松弛三种
目前只写了超松弛，最稳定
\***********************************************************/
	//设置面的权重因子
	void OfMesh::SetFaceWeightingFactor() {
		//内部面的权重因子
		for (int i = 0; i < number_of_interior_faces_; i++)
		{
			face tmp_face;
			tmp_face = faces_[i];
			
			Vector<Scalar> tmp_nf = tmp_face.sf_ / tmp_face.area_;//面的单位法向量

			element element_1 = elements_[tmp_face.owner_];
			element element_2 = elements_[tmp_face.neighbour_];

			Vector<Scalar> tmp_cn = element_2.centroid_ - element_1.centroid_;
			Vector<Scalar> tmp_e_cn = (tmp_cn / tmp_cn.Magnitude());

			faces_[i].cn_ = tmp_cn;
			faces_[i].e_cn_ = tmp_e_cn;

			//超松弛修正
			Vector<Scalar> tmp_e = (tmp_e_cn * ((tmp_face.sf_ & tmp_face.sf_) / (tmp_e_cn & tmp_face.sf_)));
			//非正交
			faces_[i].t_ = tmp_face.sf_ - tmp_e;

			//扩散系数
			faces_[i].g_diff_ = tmp_e.Magnitude() / tmp_cn.Magnitude();

			Vector<Scalar> tmp_cf = tmp_face.centroid_ - element_1.centroid_;
			Vector<Scalar> tmp_ff = element_2.centroid_ - tmp_face.centroid_;

			//插值因子
			faces_[i].gf_ = (tmp_cf & tmp_nf) / ((tmp_cf & tmp_nf) + (tmp_ff & tmp_nf));
			faces_[i].wall_dist_ = 0;
		}
		
		//边界面的权重因子
		for (int i = number_of_interior_faces_; i < number_of_faces_; i++)
		{
			face tmp_face;
			tmp_face = faces_[i];

			Vector<Scalar> tmp_nf = tmp_face.sf_ / tmp_face.area_;//面的单位法向量

			element element_1 = elements_[tmp_face.owner_];

			Vector<Scalar> tmp_cn = tmp_face.centroid_ - element_1.centroid_;
			Vector<Scalar> tmp_e_cn = (tmp_cn / tmp_cn.Magnitude());

			faces_[i].cn_ = tmp_cn;
			faces_[i].e_cn_ = tmp_e_cn;

			//超松弛修正
			Vector<Scalar> tmp_e = (tmp_e_cn * ((tmp_face.sf_ & tmp_face.sf_) / (tmp_e_cn & tmp_face.sf_)));

			//扩散系数
			faces_[i].g_diff_ = tmp_e.Magnitude() / tmp_cn.Magnitude();
			//非正交部分
			faces_[i].t_ = tmp_face.sf_ - tmp_e;

			//插值因子
			faces_[i].gf_ = 1;
			faces_[i].wall_dist_ = (tmp_cn & tmp_face.sf_) / tmp_face.sf_.Magnitude();
		}
	}

}