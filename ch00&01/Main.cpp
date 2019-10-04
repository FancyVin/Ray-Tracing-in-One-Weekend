//*************************************
// book1(RT in 1 weekend) ch0&1
// �����ԭ�飬������¹��ܣ�
// *(ch00&01)����Ϊpng��ʽ
// *(ch00&01)��Ⱦ����

//*************************************

// from https://github.com/nothings/stb.git
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <iostream>

int main() {
	// width, height, channels of image
	int nx = 1920; // width
	int ny = 1080; // height
	int channels = 3;
	// �洢ͼ������
	unsigned char *data = new unsigned char[nx*ny*channels];
	
	// ѭ������ͼ��nx*ny�е�ÿ������
	for (int j = ny - 1; j >= 0; j--) {
		for (int i = 0; i < nx; i++) {
			float r = float(i) / float(nx);
			float g = float(j) / float(ny);
			float b = 0.2f;
			int ir = int(255.99*r);
			int ig = int(255.99*g);
			int ib = int(255.99*b);
			// data[y*width*channels + x*channels + index]
			data[(ny - j - 1)*nx*3 + 3 * i] = ir;
			data[(ny - j - 1)*nx*3 + 3 * i + 1] = ig;
			data[(ny - j - 1)*nx*3 + 3 * i + 2] = ib;
		}
		// ������Ⱦ����
		std::cout << (ny - j) / float(ny) * 100.0f << "%\n";
	}
	// д��pngͼƬ
	stbi_write_png("ch00&01.png", nx, ny, channels, data, 0);

	std::cout << "Completed.\n";
	system("PAUSE");
}