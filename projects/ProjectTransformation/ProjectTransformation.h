#pragma once
#ifndef _PROJECT_TRANSFORMATION_
#define _PROJECT_TRANSFORMATION_

#include "../Project.h"
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;



class ProjectTransformation : public Project {
private:
	const char* vs_path;
	const char* fs_path;
	Shader myShader;

	bool isTestDepth;
	int show_what;
	
	float vertices[180] = {
		-2.0f, -2.0f, -2.0f,  0.0f, 0.0f,
		 2.0f, -2.0f, -2.0f,  1.0f, 0.0f,
		 2.0f,  2.0f, -2.0f,  1.0f, 1.0f,
		 2.0f,  2.0f, -2.0f,  1.0f, 1.0f,
		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 0.0f,

		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f,
		 2.0f, -2.0f,  2.0f,  1.0f, 0.0f,
		 2.0f,  2.0f,  2.0f,  1.0f, 1.0f,
		 2.0f,  2.0f,  2.0f,  1.0f, 1.0f,
		-2.0f,  2.0f,  2.0f,  0.0f, 1.0f,
		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f,

		-2.0f,  2.0f,  2.0f,  1.0f, 0.0f,
		-2.0f,  2.0f, -2.0f,  1.0f, 1.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f,
		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f,
		-2.0f,  2.0f,  2.0f,  1.0f, 0.0f,

		 2.0f,  2.0f,  2.0f,  1.0f, 0.0f,
		 2.0f,  2.0f, -2.0f,  1.0f, 1.0f,
		 2.0f, -2.0f, -2.0f,  0.0f, 1.0f,
		 2.0f, -2.0f, -2.0f,  0.0f, 1.0f,
		 2.0f, -2.0f,  2.0f,  0.0f, 0.0f,
		 2.0f,  2.0f,  2.0f,  1.0f, 0.0f,

		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f,
		 2.0f, -2.0f, -2.0f,  1.0f, 1.0f,
		 2.0f, -2.0f,  2.0f,  1.0f, 0.0f,
		 2.0f, -2.0f,  2.0f,  1.0f, 0.0f,
		-2.0f, -2.0f,  2.0f,  0.0f, 0.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f,

		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f,
		 2.0f,  2.0f, -2.0f,  1.0f, 1.0f,
		 2.0f,  2.0f,  2.0f,  1.0f, 0.0f,
		 2.0f,  2.0f,  2.0f,  1.0f, 0.0f,
		-2.0f,  2.0f,  2.0f,  0.0f, 0.0f,
		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f
	};

public:
	ProjectTransformation();
	void draw();
	void render();
	void updateCameraView(glm::mat4 cameraView_);
	void updateCameraPos(glm::vec3 cameraPos_);
};

#endif