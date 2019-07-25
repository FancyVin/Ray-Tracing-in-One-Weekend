#include <iostream>
#include <fstream>
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include <random>

using namespace std;

Vec3 Color(const Ray& r, Hitable *world, int depth)
{
	hit_record rec;
	if (world->hit(r, 0.001f, FLT_MAX, rec))
	{
		Ray scattered;
		Vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->Scatter(r, rec, attenuation, scattered)) {
			return attenuation*Color(scattered, world, depth + 1);
		}
		else {
			return Vec3(0.0f, 0.0f, 0.0f);
		}
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
	outfile.open("ch10Image.ppm");

	int nx = 200;
	int ny = 100;
	//��������
	int ns = 100;
	outfile << "P3\n" << nx << " " << ny << "\n255\n";

	Hitable *list[5];

	list[0] = new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(vec3(0.1, 0.2, 0.5)));

	list[1] = new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(vec3(0.8, 0.8, 0.0)));

	list[2] = new Sphere(Vec3(1, 0, -1), 0.5, new Metal(vec3(0.8, 0.6, 0.2), 0.0));

	list[3] = new Sphere(Vec3(-1, 0, -1), 0.5, new Dielectric(1.5));

	list[4] = new Sphere(Vec3(-1, 0, -1), -0.45, new Dielectric(1.5));

	Hitable *world = new HitableList(list, 5);

	Camera cam();

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
				col += Color(r, world, 0);
			}
			//����������ɫ��ֵ
			col /= float(ns);
			//gamma����
			col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			outfile << ir << " " << ig << " " << ib << "\n";
		}
	}
	outfile.close();
	return 0;
}