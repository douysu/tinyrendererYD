﻿// @author 憨豆酒 YinDou https://github.com/douysu
//
// @description 主文件
//
// @version1 20200312 填充三角形

#include <vector>
#include <cmath>

#include "tgaimage.h"
#include "model.h"
#include "geometry.h"

using namespace std;

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);

Model* model = NULL;

const int width = 200;
const int height = 200;

void line(Vec2i p0, Vec2i p1, TGAImage& image, TGAColor color)
{
	bool steep = false;
	if (abs(p0.x - p1.x) < abs(p0.y - p1.y))
	{
		swap(p0.x, p0.y);
		swap(p1.x, p1.y);
		steep = true;
	}

	if (p0.x > p1.x)
	{
		swap(p0.x, p1.x);
		swap(p0.y, p1.y);
	}

	int dx = p1.x - p0.x;
	int dy = p1.y - p0.y;
	int derror2 = abs(dy) * 2;
	int error2 = 0;
	int y = p0.y;
	for (int x = p0.x; x <= p1.x; x++)
	{
		if (steep)
			image.set(y, x, color);
		else
			image.set(x, y, color);

		error2 += derror2;
		if (error2 > dx)
		{
			y += (p1.y > p0.y ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

// 尝试1
//void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color)
//{
//	if (t0.y > t1.y)
//		swap(t0, t1);
//	if (t0.y > t2.y)
//		swap(t0, t2);
//	if (t1.y > t2.y) 
//		swap(t1, t2);
//
//	int total_height = t2.y - t0.y;
//	for (int y = t0.y; y <= t1.y; y++) {
//		int segment_height = t1.y - t0.y + 1;
//		float alpha = (float)(y - t0.y) / total_height;
//		float beta = (float)(y - t0.y) / segment_height; // be careful with divisions by zero 
//		Vec2i A = t0 + (t2 - t0) * alpha;
//		Vec2i B = t0 + (t1 - t0) * beta;
//		image.set(A.x, y, red);
//		image.set(B.x, y, green);
//	}
//}

// 尝试2
//void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) {
//	// sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
//	if (t0.y > t1.y) std::swap(t0, t1);
//	if (t0.y > t2.y) std::swap(t0, t2);
//	if (t1.y > t2.y) std::swap(t1, t2);
//	int total_height = t2.y - t0.y;
//	for (int y = t0.y; y <= t1.y; y++) {
//		int segment_height = t1.y - t0.y + 1;
//		float alpha = (float)(y - t0.y) / total_height;
//		float beta = (float)(y - t0.y) / segment_height; // be careful with divisions by zero 
//		Vec2i A = t0 + (t2 - t0) * alpha;
//		Vec2i B = t0 + (t1 - t0) * beta;
//		if (A.x > B.x) std::swap(A, B);
//		for (int j = A.x; j <= B.x; j++) {
//			image.set(j, y, color); // attention, due to int casts t0.y+i != A.y 
//		}
//	}
//	for (int y = t1.y; y <= t2.y; y++) {
//		int segment_height = t2.y - t1.y + 1;
//		float alpha = (float)(y - t0.y) / total_height;
//		float beta = (float)(y - t1.y) / segment_height; // be careful with divisions by zero 
//		Vec2i A = t0 + (t2 - t0) * alpha;
//		Vec2i B = t1 + (t2 - t1) * beta;
//		if (A.x > B.x) std::swap(A, B);
//		for (int j = A.x; j <= B.x; j++) {
//			image.set(j, y, color); // attention, due to int casts t0.y+i != A.y 
//		}
//	}
//}

// 尝试3
void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage& image, TGAColor color) {
	if (t0.y == t1.y && t0.y == t2.y) return; // I dont care about degenerate triangles 
	// sort the vertices, t0, t1, t2 lower−to−upper (bubblesort yay!) 
	if (t0.y > t1.y) std::swap(t0, t1);
	if (t0.y > t2.y) std::swap(t0, t2);
	if (t1.y > t2.y) std::swap(t1, t2);
	int total_height = t2.y - t0.y;
	for (int i = 0; i < total_height; i++) {
		bool second_half = i > t1.y - t0.y || t1.y == t0.y;
		int segment_height = second_half ? t2.y - t1.y : t1.y - t0.y;
		float alpha = (float)i / total_height;
		float beta = (float)(i - (second_half ? t1.y - t0.y : 0)) / segment_height; // be careful: with above conditions no division by zero here 
		Vec2i A = t0 + (t2 - t0) * alpha;
		Vec2i B = second_half ? t1 + (t2 - t1) * beta : t0 + (t1 - t0) * beta;
		if (A.x > B.x) std::swap(A, B);
		for (int j = A.x; j <= B.x; j++) {
			image.set(j, t0.y + i, color); // attention, due to int casts t0.y+i != A.y 
		}
	}
}

int main(int argc, char** argv)
{
	TGAImage image(width, height, TGAImage::RGB);

	Vec2i t0[3] = { Vec2i(10, 70),   Vec2i(50, 160),  Vec2i(70, 80) };
	Vec2i t1[3] = { Vec2i(180, 50),  Vec2i(150, 1),   Vec2i(70, 180) };
	Vec2i t2[3] = { Vec2i(180, 150), Vec2i(120, 160), Vec2i(130, 180) };

	triangle(t0[0], t0[1], t0[2], image, red);
	triangle(t1[0], t1[1], t1[2], image, white);
	triangle(t2[0], t2[1], t2[2], image, green);

	image.flip_vertically();
	image.write_tga_file("output.tga");
	delete model;
	return 0;
}