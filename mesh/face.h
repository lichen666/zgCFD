#pragma once
#include <vector>
#include "zg_scalar.h"
#include "zg_vector.h"
#include "point.h"

namespace zgCFD {
	class face
	{
	public:
		face();
		face(const int& index);
		int index_;
		int owner_;
		int neighbour_;

		Scalar area_;//���
		Vector<Scalar> sf_;//������
		point centroid_;//����
		Vector<Scalar> cn_;//���owner������ָ��neighbour�����ĵ�����
		Vector<Scalar> e_cn_;//���owner������ָ��neighbour�����ĵĵ�λ����

		Scalar gf_;//���ֵ�ļ���Ȩ������
		
		//TODO ��ʱ��֪����ɶ�ã������ٿ�
		Scalar g_diff_;//�漸����ɢϵ��
		Vector<Scalar> t_;//owner��Ԫ��neighbour��Ԫ���ĵľ�������
		Scalar wall_dist_;//��������Ԫ���ĵ�����Ĵ�ֱ���룬ĳЩ����ģ�Ϳ����õ�

		std::vector<int> points_;
	};
}