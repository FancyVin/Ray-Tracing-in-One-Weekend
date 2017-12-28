#pragma once
#define _USE_MATH_DEFINES
#include "Ray.h"
#include <math.h>

//��z=0ƽ���ϲ���һ���������ԭ�㣬����С��1�������������������Ϊʲô��z=0ƽ�棬������������б�����йء����������б����Ϊview up �����vup��һ������Ϊ��0��1��0������
Vec3 RandomInUnitDisk() {
	Vec3 p;
	do {
		p = 2.0*Vec3((rand() % (100) / (float)(100)), (rand() % (100) / (float)(100)), 0) - Vec3(1, 1, 0);
	} while (dot(p, p) >= 1.0);
	return p;
}

class Camera 
{
public:
	//vfov: top to bottom in degrees
	Camera(Vec3 lookfrom, Vec3 lookat, Vec3 vup, float vfov, float aspect, float aperture, float focus_dist)
	{
		lens_radius = aperture / 2;
		float theta = vfov*M_PI / 180;
		float half_height = tan(theta / 2);
		float half_width = aspect * half_height;

		origin = lookfrom;
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);

		lower_left_corner = origin - half_width*focus_dist*u - half_height*focus_dist*v - focus_dist*w;
		horizontal = focus_dist * 2 * half_width*u;
		vertical = focus_dist * 2 * half_height*v;
	}

	Ray getRay(float s, float t) 
	{
		Vec3 rd = lens_radius * RandomInUnitDisk();
		Vec3 offset = u * rd.x() + v * rd.y();
		return Ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
	}

	Vec3 lower_left_corner;
	Vec3 origin;
	Vec3 horizontal;
	Vec3 vertical;
	Vec3 u, v, w;
	float lens_radius;
};