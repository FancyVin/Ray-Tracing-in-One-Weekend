#pragma once  
#include "Ray.h"

//ײ���㴦��Ϣ
struct hit_record
{
	//���߲���t
	float t;
	//ײ����λ������p
	Vec3 p;
	//ײ���㴦������N
	Vec3 normal;
};

//�����ܱ�����ײ��������ĸ���
class Hitable
{
public:
	//hit()�ڴ˱�����Ϊ�麯������hitableΪ�����ࡣ������������б���ʵ�����麯��
	virtual bool hit(const Ray& r, float t_min, float t_max, hit_record& rec) const = 0;
};
