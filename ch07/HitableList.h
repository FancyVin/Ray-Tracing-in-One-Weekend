#pragma once  
#include "Hitable.h"

/*�����ж��б������������Ƿ񱻹���ײ����ÿ���ж�һ����
���б�ײ������ײ����Ϣ������hit_record�ṹ���С�
���ǿ��Կ���rec�ǿ��ܱ�д��εģ����ձ����ֵ�Ǻ�һ�ε�ֵ��
Ҳ����������Ч��ֵ�Ǻ�һ�ε�ֵ��Ҳ������۲��������������Чײ��
������Чײ�㡱�����ڵ������壬��ɸѡ��һ���ֲ���Чײ�㣻���ڶ�����壬�����е���������Եľֲ���Чײ��ɸѡ������һ��������Чײ�㣩��
��Ϊ����������������ײ���˶��ٸ�����������ٸ�ײ�㣬�����ܿ�����ֻ�������������ײ��
�����ǰײ���ڷ�Χ�ڣ��򽫵�ǰײ��ľ�������Ϊ��Χ�����ֵ��Ҳ���Ǻ���ֻ���Ǳȸ�ײ�������ײ�㡣
�����ǣ��ҵ���ײ����Խ��Խ���ģ������ҵ������ײ�㡣*/
class HitableList : public Hitable
{
public:
	HitableList(){}
	HitableList(Hitable **l, int n) { list = l; list_size = n; }
	virtual bool hit(const Ray& r, float tmin, float tmax, hit_record& rec) const;
	Hitable **list;
	int list_size;
};

bool HitableList::hit(const Ray& r, float t_min, float t_max, hit_record& rec)const
{
	hit_record temp_rec;
	bool hit_anything = false;
	double closest_so_far = t_max;
	for (int i = 0; i < list_size; i++)
	{
		if (list[i]->hit(r, t_min, closest_so_far, temp_rec))
		{
			hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}
	return hit_anything;
}

