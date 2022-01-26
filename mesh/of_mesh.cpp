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

	//��ȡ���ļ�
	void OfMesh::ReadPointsFile(){

		points_file_.open(case_directory_ + "\\points", std::ios::in);

		if (!points_file_.is_open())
		{
			std::cout << "points�ļ���ʧ��" << std::endl;
			return;
		}

		char tmp_char_;
		std::string tmp_string_;

		//���˵�ǰ����ļ�ͷ������
		for (int i = 0; i < 18; i++)
		{
			std::getline(points_file_, tmp_string_);
		}
		//��ȡ�������
		points_file_ >> number_of_points_;

		//�����������С
		points_.resize(number_of_points_);

		//���˵����м�"("
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

			std::getline(points_file_, tmp_string_);//")"����ĩ
		}

		points_file_.close();
	}

	//��ȡ���ļ�
	void OfMesh::ReadFacesFile(){

		faces_file_.open(case_directory_ + "\\faces", std::ios::in);

		if (!faces_file_.is_open())
		{
			std::cout << "faces�ļ���ʧ��" << std::endl;
			return;
		}

		char tmp_char_;
		std::string tmp_string_;
		int tmp_index_of_points_;
		

		//���˵�ǰ����ļ�ͷ������
		for (int i = 0; i < 18; i++)
		{
			std::getline(faces_file_, tmp_string_);
		}
		//��ȡ�������
		faces_file_ >> number_of_faces_;

		//�����������С
		faces_.resize(number_of_faces_);

		//���˵����м�"("
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

	//��ȡowner�ļ�
	void OfMesh::ReadOwnerFile(){
	
		owner_file_.open(case_directory_ + "\\owner", std::ios::in);

		if (!owner_file_.is_open())
		{
			std::cout << "owner�ļ���ʧ��" << std::endl;
			return;
		}

		std::string tmp_string_;
		int tmp_max_of_index = 0;

		//���˵�ǰ����ļ�ͷ������
		for (int i = 0; i < 19; i++)
		{
			std::getline(owner_file_, tmp_string_);
		}
		//��ȡowner������
		owner_file_ >> number_of_owners_;

		owner_file_ >> tmp_string_;//"("

		for (int i = 0; i < number_of_owners_; i++)
		{
			int tmp_index_of_owner_;
			owner_file_ >> tmp_index_of_owner_;
			faces_[i].owner_ = tmp_index_of_owner_;
			if (tmp_max_of_index < tmp_index_of_owner_) tmp_max_of_index = tmp_index_of_owner_;
		}

		//���������
		number_of_elements_ = tmp_max_of_index + 1;

		owner_file_.close();
	}

	//��ȡneighbour�ļ�
	void OfMesh::ReadNeighbourFile(){
	
		neighbour_file_.open(case_directory_ + "\\neighbour", std::ios::in);

		if (!neighbour_file_.is_open())
		{
			std::cout << "neighbour�ļ���ʧ��" << std::endl;
			return;
		}

		std::string tmp_string_;

		//���˵�ǰ����ļ�ͷ������
		for (int i = 0; i < 19; i++)
		{
			std::getline(neighbour_file_, tmp_string_);
		}
		//��ȡneighbour������
		neighbour_file_ >> number_of_neighbours_;

		neighbour_file_ >> tmp_string_;//"("

		//�ڲ����������neighbour���������
		number_of_interior_faces_ = number_of_neighbours_;

		for (int i = 0; i < number_of_neighbours_; i++)
		{
			int tmp_index_of_neighbour_;
			neighbour_file_ >> tmp_index_of_neighbour_;
			faces_[i].neighbour_ = tmp_index_of_neighbour_;
		}

		neighbour_file_.close();
	}

	//��ȡ�߽�
	void OfMesh::ReadBoundaryFile(){
	
		boundary_file_.open(case_directory_ + "\\boundary", std::ios::in);

		if (!boundary_file_.is_open())
		{
			std::cout << "boundary�ļ���ʧ��" << std::endl;
			return;
		}

		char tmp_char_;
		std::string tmp_string_;
		std::stringstream tmp_sstream_;

		//���˵�ǰ����ļ�ͷ������
		for (int i = 0; i < 17; i++)
		{
			std::getline(boundary_file_, tmp_string_);
		}
		//��ȡ�߽������
		boundary_file_ >> number_of_boundaries_;
		boundaries_.resize(number_of_boundaries_);

		//���˵����м�"("
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

	//��������
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

	//���ýڵ��������
	void OfMesh::SetPointConnectinities() {

		std::vector<int> tmp_points_of_face_;
		std::vector<int> tmp_faces_of_element_;

		//����ÿ�����Ӧ����
		for (int i = 0; i < number_of_faces_; i++)
		{
			tmp_points_of_face_ = faces_[i].points_;
			for (std::vector<int>::iterator it = tmp_points_of_face_.begin(); it != tmp_points_of_face_.end(); it++)
			{
				points_[*it].faces_.push_back(i);
			}
		}

		bool tmp_point_is_repeated_ = false;

		//����ÿ�����Ӧ�������Լ�ÿ�������Ӧ�ĵ�
		for (int i = 0; i < number_of_elements_; i++)
		{
			tmp_faces_of_element_ = elements_[i].faces_;
			for (std::vector<int>::iterator it_face = tmp_faces_of_element_.begin(); it_face != tmp_faces_of_element_.end(); it_face++)
			{
				tmp_points_of_face_ = faces_[*it_face].points_;
				for (std::vector<int>::iterator it_point = tmp_points_of_face_.begin(); it_point != tmp_points_of_face_.end(); it_point++)
				{
					for (std::vector<int>::iterator it = elements_[i].points_.begin();  it != elements_[i].points_.end(); it++)//����Ƿ����ظ��ĵ�
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

	//���������ļ����õ���Ԫ������ģ���Ԫ������������������������
	void OfMesh::ProcessOFMesh() {
		//��
		for (int i = 0; i < number_of_faces_; i++)
		{
			point tmp_centre_of_face(0., 0., 0.);//��������
			point tmp_centroid_of_face(0., 0., 0.);//����

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

			//����С�����ε�������
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

					point tmp_cemtroid_of_triangle((tmp_point1.x_ + tmp_point2.x_ + tmp_point3.x_) / 3, (tmp_point1.y_ + tmp_point2.y_ + tmp_point3.y_) / 3, (tmp_point1.z_ + tmp_point2.z_ + tmp_point3.z_) / 3);//С�����ε�����
					
					
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
		}//����Ϣ�������

		//��
		for (int i = 0; i < number_of_elements_; i++) {

			point tmp_centre_of_element(0., 0., 0.);//��������

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

			point tmp_centroid_of_element(0., 0., 0.);//����
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
���Ȩ�����ӻ����������ĵ㵽��Ĵ�ֱ������в�ֵ��
g_f = ( d_Cf * e ) /( (d_Cf * e ) + ( d_fF * e) )

���У����ڷ��������������������С���������������������ɳ�����
Ŀǰֻд�˳��ɳڣ����ȶ�
\***********************************************************/
	//�������Ȩ������
	void OfMesh::SetFaceWeightingFactor() {
		//�ڲ����Ȩ������
		for (int i = 0; i < number_of_interior_faces_; i++)
		{
			face tmp_face;
			tmp_face = faces_[i];
			
			Vector<Scalar> tmp_nf = tmp_face.sf_ / tmp_face.area_;//��ĵ�λ������

			element element_1 = elements_[tmp_face.owner_];
			element element_2 = elements_[tmp_face.neighbour_];

			Vector<Scalar> tmp_cn = element_2.centroid_ - element_1.centroid_;
			Vector<Scalar> tmp_e_cn = (tmp_cn / tmp_cn.Magnitude());

			faces_[i].cn_ = tmp_cn;
			faces_[i].e_cn_ = tmp_e_cn;

			//���ɳ�����
			Vector<Scalar> tmp_e = (tmp_e_cn * ((tmp_face.sf_ & tmp_face.sf_) / (tmp_e_cn & tmp_face.sf_)));
			//������
			faces_[i].t_ = tmp_face.sf_ - tmp_e;

			//��ɢϵ��
			faces_[i].g_diff_ = tmp_e.Magnitude() / tmp_cn.Magnitude();

			Vector<Scalar> tmp_cf = tmp_face.centroid_ - element_1.centroid_;
			Vector<Scalar> tmp_ff = element_2.centroid_ - tmp_face.centroid_;

			//��ֵ����
			faces_[i].gf_ = (tmp_cf & tmp_nf) / ((tmp_cf & tmp_nf) + (tmp_ff & tmp_nf));
			faces_[i].wall_dist_ = 0;
		}
		
		//�߽����Ȩ������
		for (int i = number_of_interior_faces_; i < number_of_faces_; i++)
		{
			face tmp_face;
			tmp_face = faces_[i];

			Vector<Scalar> tmp_nf = tmp_face.sf_ / tmp_face.area_;//��ĵ�λ������

			element element_1 = elements_[tmp_face.owner_];

			Vector<Scalar> tmp_cn = tmp_face.centroid_ - element_1.centroid_;
			Vector<Scalar> tmp_e_cn = (tmp_cn / tmp_cn.Magnitude());

			faces_[i].cn_ = tmp_cn;
			faces_[i].e_cn_ = tmp_e_cn;

			//���ɳ�����
			Vector<Scalar> tmp_e = (tmp_e_cn * ((tmp_face.sf_ & tmp_face.sf_) / (tmp_e_cn & tmp_face.sf_)));

			//��ɢϵ��
			faces_[i].g_diff_ = tmp_e.Magnitude() / tmp_cn.Magnitude();
			//����������
			faces_[i].t_ = tmp_face.sf_ - tmp_e;

			//��ֵ����
			faces_[i].gf_ = 1;
			faces_[i].wall_dist_ = (tmp_cn & tmp_face.sf_) / tmp_face.sf_.Magnitude();
		}
	}

}