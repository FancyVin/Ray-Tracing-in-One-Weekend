#include <iostream>
#include <fstream>
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include <random>

using namespace std;

Vec3 Color(const Ray& r, Hitable *world)
{
	hit_record rec;
	if (world->hit(r, 0.0, FLT_MAX, rec))
	{
		return 0.5f*Vec3(rec.normal.x() + 1.0f, rec.normal.y() + 1.0f, rec.normal.z() + 1.0f);
	}
	else
	{
		//���Ʊ���
		Vec3 unit_direction = unit_vector(r.direction());
		float t = 0.5f*(unit_direction.y() + 1.0f);
		//(1-t)*��ɫ+t*��ɫ�������һ�����׵Ľ���
		return (1.0f - t)*Vec3(1.0f, 1.0f, 1.0f) + t*Vec3(0.5f, 0.7f, 1.0f);
	}
}

int main()
{

	ofstream outfile;
	outfile.open("ch6Image.ppm");

	int nx = 200;
	int ny = 100;
	//��������
	int ns = 100;
	outfile << "P3\n" << nx << " " << ny << "\n255\n";

	Hitable *list[2];
	list[0] = new Sphere(Vec3(0.0f, 0.0f, -1.0f), 0.5f);
	list[1] = new Sphere(Vec3(0.0f, -100.5f, -1.0f), 100.0f);
	Hitable *world = new HitableList(list, 2);

	Camera cam;

	//���������
	default_random_engine reng;
	uniform_real_distribution<float> uni_dist(0.0f, 1.0f);

	for (int j = ny - 1; j >= 0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			Vec3 col(0.0f, 0.0f, 0.0f);
			//ÿ���������ns��
			for (int s = 0; s < ns; s++)
			{
				float u = float(i + uni_dist(reng)) / float(nx);
				float v = float(j + uni_dist(reng)) / float(ny);
				Ray r = cam.getRay(u,v);
				//Vec3 p = r.point_at_parameter(2.0);
				//��������(u,v)��(u+1,v+1)������ɫֵ�ۼ�
				col += Color(r, world);
			}
			//����������ɫ��ֵ
			col /= float(ns);

			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			outfile << ir << " " << ig << " " << ib << "\n";
		}
	}
	outfile.close();
	return 0;
}