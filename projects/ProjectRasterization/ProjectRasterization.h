#pragma once
#ifndef _PROJECT_RASTERIZATION_
#define _PROJECT_RASTERIZATION_
#include "../Project.h"
#include <vector>
#define MAX_POINT 42000
#define POINT_SIZE 7
#define WIN_W 105
#define WIN_H 105

using namespace std;

struct Line {
	int A, B, C;
	Line(int x1, int y1, int x2, int y2) {
		A = y2 - y1;
		B = x1 - x2;
		C = x2 * y1 - x1 * y2;
	}
};

class ProjectRasterization : public Project {
private:
	const char* vs_path;
	const char* fs_path;
	Shader myShader;
	int show_what;
	int radius;
	int size;
	bool show_Rasterization;
	//顶点信息
	float vertices[MAX_POINT];
	//绘制点的数量
	int counter;
	//三角形顶点坐标输入
	int vertex1[2];
	int vertex2[2];
	int vertex3[2];

public:
	ProjectRasterization();
	void init();
	void draw();
	void render();
	void clear();
	void centerlize_line(Line &L, int x, int y);
	void Bresenham_line(int x0, int y0, int x1, int y1);
	void Bresenham_circle(int r);
	void draw_circle_8(int x, int y);
	void Triangle_Rasterization(int x0, int y0, int x1, int y1, int x2, int y2);

	void updateCameraView(glm::mat4 cameraView_);
	void updateCameraPos(glm::vec3 cameraPos_);
};

#endif