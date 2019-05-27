#pragma once
#ifndef _PROJECT_BEZIER_
#define _PROJECT_BEZIER_
#include "../Project.h"
#include <vector>
#include <math.h>

using namespace std;

class ProjectBezier : public Project {
private:
	unsigned int VBO;
	unsigned int controlVAO;
	unsigned int assistVAO;
	unsigned int bezierVAO;

	const char* vs_path;
	const char* fs_path;
	Shader myShader;
	// 顶点信息
	float vertices[MAX_POINT];
	int verticesNum;
	// 阶乘
	int factorial[13];
	vector<glm::vec2> controlPoints;
	int controlNum;
	float controlVertices[100];
	int assistNum;
	float assistVertices[100];
	float frame;

public:
	ProjectBezier(GLFWwindow* window_);
	GLFWwindow* window;
	void draw();
	void render();
	int getFactorial(int n);
	void addControlPoint(glm::vec2 p);
	void deleteControlPoint();

	// Bezier算法
	void createBezierLine();
	float Bernstein(int i, int n, float t);
	void createAssistLines(float t);
	void drawAssistLines(int count);

	void updateCameraView(glm::mat4 cameraView_);
	void updateCameraPos(glm::vec3 cameraPos_);
};

#endif