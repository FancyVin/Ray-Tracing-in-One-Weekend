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

Hitable *RandomScene() {
	int n = 500;
	Hitable **list = new Hitable *[n + 1];
	/*����һ������n+1��Ԫ�ص����飬�����ÿ��Ԫ����ָ��hitable�����ָ�롣Ȼ�������ָ�븳ֵ��list�����ԣ�list��ָ���ָ�롣*/
	list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5)));
	/*�ȴ���һ�������ڣ�0��-1000��0���뾶Ϊ1000�ĳ��������򣬽���ָ�뱣����list�ĵ�һ��Ԫ���С�*/
	int i = 1;
	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			/*����forѭ���л������11+11��*(11+11)=484�����С��*/
			float choose_mat = (rand() % (100) / (float)(100));
			/*����һ����0��1�������������Ϊ����С����ϵķ�ֵ*/
			Vec3 center(a + 0.9*(rand() % (100) / (float)(100)), 0.2,b + 0.9*(rand() % (100) / (float)(100)));
			/*�� a+0.9*(rand()%(100)/(float)(100))�����[-11,11]������-11��11��֮����������������[-11,11)֮���22��������ʹ�����ĵ�x,z�����ǣ�-11��11��֮��������*/
			if ((center - Vec3(4, 0.2, 0)).length() > 0.9) {
				/*����С���λ�ú���ǰ��Ĵ����λ��̫����*/
				if (choose_mat < 0.8) {     //diffuse  
											/*���Ϸ�ֵС��0.8��������Ϊ�����������������˥��ϵ��x,y,z���ǣ�0��1��֮����������ƽ��*/
					list[i++] = new Sphere(center, 0.2,
						new Lambertian(Vec3(
						(rand() % (100) / (float)(100))*(rand() % (100) / (float)(100)),
							(rand() % (100) / (float)(100))*(rand() % (100) / (float)(100)),
							(rand() % (100) / (float)(100))*(rand() % (100) / (float)(100)))));
				}
				else if (choose_mat < 0.95) {
					/*���Ϸ�ֵ���ڵ���0.8С��0.95��������Ϊ���淴���򣬾��淴�����˥��ϵ��x,y,z��ģ��ϵ�����ǣ�0��1��֮����������һ�ٳ���2*/
					list[i++] = new Sphere(center, 0.2,
						new Metal(Vec3(0.5*(1 + (rand() % (100) / (float)(100))),
							0.5*(1 + (rand() % (100) / (float)(100))),
							0.5*(1 + (rand() % (100) / (float)(100)))),
							0.5*(1 + (rand() % (100) / (float)(100)))));
				}
				else {
					/*���Ϸ�ֵ���ڵ���0.95��������Ϊ������*/
					list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
				}
			}
		}
	}

	list[i++] = new Sphere(Vec3(0, 1, 0), 1.0, new Dielectric(1.5));
	list[i++] = new Sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(Vec3(0.4, 0.2, 0.1)));
	list[i++] = new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));
	/*������������*/
	return new HitableList(list, i);
}

int main()
{
	ofstream outfile;
	outfile.open("ch12_HighImage.ppm");

	int nx = 2000;
	int ny = 1000;
	//��������
	int ns = 100;
	outfile << "P3\n" << nx << " " << ny << "\n255\n";


	Hitable *world = RandomScene();

	Vec3 lookfrom(13.0f, 2.0f, 3.0f);
	Vec3 lookat(0.0f, 0.0f, 0.0f);
	float dist_to_focus = 10.0f;
	float aperture = 0.1f;

	Camera cam(lookfrom, lookat , Vec3(0.0f, 1.0f, 0.0f), 20, float(nx) / float(ny), aperture, dist_to_focus);

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