// @author ������ YinDou https://github.com/douysu
//
// @description ���ļ�
//
// @version1 20200311 �����߶�

#include "tgaimage.h"

using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);

// ����һ ���ճ��ȵ����ӻ���
//void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color)
//{
//	for (float t = 0.; t < 1; t += .01f)
//	{
//		int x = (int)((double)x0 * (1. - t) + (double)x1 * t);
//		int y = (int)((double)y0 * (1. - t) + (double)y1 * t);
//		image.set(x, y, color);
//	}
//}

// ���Զ� ����x���ӻ��ƣ�����y
//void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color)
//{
//	for (int x = x0; x <= x1; x++)
//	{
//		float t = (x - x0) / (float)(x1 - x0); // ����ռ��
//		int y = (int)(y0 * (1. - t) + y1 * (double)t); // ����Y
//		image.set(x, y, color);
//	}
//}

// ������ 
//void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color)
//{
//	bool steep = false;
//	
//	// ���б�� > 1������
//	if (abs(x0 - x1) < abs(y0 - y1))
//	{
//		swap(x0, y0);
//		swap(x1, y1);
//		steep = true;
//	}
//	// ��� x0 > x1
//	if (x0 > x1)
//	{
//		swap(x0, x1);
//		swap(y0, y1);
//	}
//
//	for (int x = 0; x < x1; x++)
//	{
//		float t = (x - x0) / (float)(x1 - x0);
//		int y = (int)(y0 * (1. - t) + y1 * (double)t);
//		if (steep)
//			image.set(y, x, color);
//		else
//			image.set(x, y, color);
//	}
//}

// ������
//void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color)
//{
//	bool steep = false;
//	if (abs(x0 - x1) < abs(y0 - y1))
//	{
//		swap(x0, y0);
//		swap(x1, y1);
//		steep = true;
//	}
//
//	if (x0 > x1)
//	{
//		swap(x0, x1);
//		swap(y0, y1);
//	}
//
//	int dx = x1 - x0;
//	int dy = y1 - y0;
//	float derror = abs(dy / float(dx));
//	float error = 0;
//
//	int  y = y0;
//	for (int x = x0; x < x1; x++)
//	{
//		if (steep)
//			image.set(y, x, color);
//		else
//			image.set(x, y, color);
//
//		error += derror; // xÿ���ӣ�ҲҪ���Ӷ�Ӧ��ֵ
//		if (error > .5)
//		{
//			y += (y1 > y0 ? 1 : -1);
//			error -= 1;
//		}
//	}
//}

// ������
void line(int x0, int y0, int x1, int y1, TGAImage& image, TGAColor color)
{
	bool steep = false;
	if (abs(x0 - x1) < abs(y0 - y1))
	{
		swap(x0, y0);
		swap(x1, y1);
		steep = true;
	}

	if (x0 > x1)
	{
		swap(x0, x1);
		swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = abs(dy) * 2;
	int error2 = 0;
	int y = y0;
	for (int x = x0; x <= x1; x++)
	{
		if (steep)
			image.set(y, x, color);
		else
			image.set(x, y, color);

		error2 += derror2;
		if (error2 > dx)
		{
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

int main(int argc, char** argv)
{
	TGAImage image(100, 100, TGAImage::RGB);

	line(13, 20, 80, 40, image, white);
	line(20, 13, 40, 80, image, red);
	line(80, 40, 13, 20, image, red);

	image.flip_vertically();
	image.write_tga_file("output.tga");

	return 0;
}