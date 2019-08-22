//*************************************
// book1(RT in 1 weekend) ch11
// �����ԭ�� ������¹��ܣ�
// -(ch00&01)����Ϊpng��ʽ
// -(ch00&01)��Ⱦ����
// -(ch04)ͼ��ĳ����Ҫ�� horizontal/vertical һ�£������Ť�����Ρ��������ѡ���޸�ͼ�񳤿�� h/v ��ֵ����������ѡ���޸�ͼ�񳤿�
//*************************************

// stbͼ��� from https://github.com/nothings/stb.git
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"


vec3 color(const ray& r, hitable *world, int depth) {
	hit_record rec;
	if (world->hit(r, 0.001, FLT_MAX, rec)) {
		ray scattered;
		vec3 attenuation;
		if (depth < 50 && rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
			return attenuation * color(scattered, world, depth + 1);
		}
		else {
			return vec3(0, 0, 0);
		}
	} 
	else {
		// ���򣬼��㱳��ɫ������
		vec3 unit_direction = unit_vector(r.direction());
		float t = (0.5 * unit_direction.y() + 1.0);
		return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
	}
}

int main() {
	// width, height, channels of image
	int nx = 2000; // width
	int ny = 1000; // height
	int channels = 3;
	int ns = 100; // sample count

	// �洢ͼ������
	unsigned char *data = new unsigned char[nx*ny*channels];
	
	// �������
	hitable *list[5];
	list[0] = new sphere(vec3(0, 0, -1), 0.5, new lambertian(vec3(0.8,0.3,0.3)));
	list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0)));
	list[2] = new sphere(vec3(1, 0, -1), 0.5, new metal(vec3(0.8, 0.6, 0.2), 0.3));
	list[3] = new sphere(vec3(-1, 0, -1), 0.5, new dielectric(1.5));
	list[4] = new sphere(vec3(-1, 0, -1), -0.45, new dielectric(1.5));
	hitable *world = new hitable_list(list, 5);
	vec3 lookfrom(3, 3, 2);
	vec3 lookat(0, 0, -1);
	float dist_to_focus = (lookfrom - lookat).length();
	float aperture = 2.0;
	camera cam(lookfrom, lookat, vec3(0,1,0), 20, float(nx)/float(ny), aperture, dist_to_focus);

	// ѭ������ͼ��nx*ny�е�ÿ������
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			vec3 col(0,0,0);
			// ���� ns ��
			for (int k = 0; k < ns; k++) {
				float u = float(i + (rand() % 100 / float(100))) / float(nx);
				float v = float(j + (rand() % 100 / float(100))) / float(ny);
				// ȷ�� ray r
				ray r = cam.get_ray(u, v);
				// �ۼ� ray r ���볡�� world �󣬷��ص���ɫ
				col += color(r, world, 0);
			}
			col /= float(ns);
			// gammar ����
			col = vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));
			// д��ͼ������ data[y*width*channels + x*channels + index]
			data[(ny - j - 1)*nx*3 + 3 * i + 0] = int(255.99*col[0]);
			data[(ny - j - 1)*nx*3 + 3 * i + 1] = int(255.99*col[1]);
			data[(ny - j - 1)*nx*3 + 3 * i + 2] = int(255.99*col[2]);
		}
		// print��Ⱦ����
		std::cout << (ny - j) / float(ny) * 100.0f << "%\n";
	}
	// д��pngͼƬ
	stbi_write_png("ch11.png", nx, ny, channels, data, 0);

	std::cout << "Completed.\n";
	system("PAUSE");
	return 0;
}