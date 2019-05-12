#pragma once
#ifndef _PROJECT_TRIANGLE_
#define _PROJECT_TRIANGLE_
#include "../Project.h"

using namespace std;

class ProjectTriangle : public Project {
private:
	const char* vs_path;
	const char* fs_path;
	Shader myShader;
	ImVec4 myColor;

	float vertices[120] = {
		//first triangle
		-0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,	//left down
		-0.2f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,	//right down
		-0.5f,  0.7f, 0.0f, 1.0f, 0.0f, 0.0f,	//up
		//second triangle
		0.2f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,		//left down
		0.8f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,		//right down
		0.5f,  0.7f, 0.0f, 0.0f, 1.0f, 0.0f,		//up
		//third triangle
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,		//left
		0.1f,  -0.2f, 0.0f, 0.0f, 1.0f, 0.0f,	//down
		//line
		-0.8f, -0.2f, 0.0f, 1.0f, 0.0f, 0.0f,	//left
		0.8f, -0.3f, 0.0f, 1.0f, 0.0f, 0.0f,	//right
		//point
		0.0f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.02f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.04f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.02f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.04f, -0.4f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, -0.42f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.02f, -0.42f, 0.0f, 1.0f, 0.0f, 0.0f,
		-0.04f, -0.42f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.02f, -0.42f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.04f, -0.42f, 0.0f, 1.0f, 0.0f, 0.0f,
	};

	unsigned int indices[6] = {
		3, 4, 5, //first triangle
		3, 6, 7  //second triangle
	};
public:
	ProjectTriangle();
	void draw();
	void render();
	void updateCameraView(glm::mat4 cameraView_);
	void updateCameraPos(glm::vec3 cameraPos_);
};


#endif