//*************************************
// book1(RT in 1 weekend) ch5
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

#include <iostream>
#include "ray.h"

float hit_sphere(const vec3& center, float radius, const ray& r) {
	vec3 A_C = r.origin() - center;
	vec3 B = r.direction();
	float a = dot(B, B);
	float b = 2.0 * dot(A_C, B);
	float c = dot(A_C, A_C) - radius * radius;
	float discriminant = b * b - 4 * a*c;
	if (discriminant < 0) { // û�н���
		return -1.0;
	}
	else {// �н��㣬������С���Ǹ�t����������Ľ��㣩
		return (-b - sqrt(discriminant)) / (2.0 * a);
	} 
}

vec3 color(const ray& r) {
	// ��� ray r ��������Ļ������ؽ���� t
	float t = hit_sphere(vec3(0, 0, -1), 0.5, r);
	if (t > 0.0) {
		// ���㽻�㴦�ķ��ߣ����ѷ��߽��п��ӻ�
		vec3 N = unit_vector(r.point_at_parameter(t) - vec3(0, 0, -1)); // P - C
		return 0.5*vec3(N.x() + 1, N.y() + 1, N.z() + 1);
	}
	// ���򣬼��㱳��ɫ������
	vec3 unit_direction = unit_vector(r.direction());
	t = (0.5 * unit_direction.y() + 1.0);
	return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

int main() {
	// width, height, channels of image
	int nx = 2000; // width
	int ny = 1000; // height
	int channels = 3;
	// �洢ͼ������
	unsigned char *data = new unsigned char[nx*ny*channels];

	// camera���
	vec3 lower_left_corner(-2.0, -1.0, -1.0); // ���½�
	vec3 horizontal(4.0, 0.0, 0.0); // ����
	vec3 vertical(0.0, 2.0, 0.0); // ����
	vec3 origin(0.0, 0.0, 0.0); // eye
	
	// ѭ������ͼ��nx*ny�е�ÿ������
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			// ȷ�� ray r
			float u = float(i) / float(nx);
			float v = float(j) / float(ny);
			ray r(origin, lower_left_corner + u * horizontal + v * vertical);

			// ���� ray r ���ص���ɫ
			vec3 col = color(r);

			// д��ͼ������ data[y*width*channels + x*channels + index]
			data[(ny - j - 1)*nx*3 + 3 * i + 0] = int(255.99*col[0]);
			data[(ny - j - 1)*nx*3 + 3 * i + 1] = int(255.99*col[1]);
			data[(ny - j - 1)*nx*3 + 3 * i + 2] = int(255.99*col[2]);
		}
		// print��Ⱦ����
		std::cout << (ny - j) / float(ny) * 100.0f << "%\n";
	}
	// д��pngͼƬ
	stbi_write_png("ch05-1.png", nx, ny, channels, data, 0);

	std::cout << "Completed.\n";
	system("PAUSE");
	return 0;
}