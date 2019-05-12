#pragma once
#ifndef _PROJECT_CAMERA_
#define _PROJECT_CAMERA_

#include "../Project.h"
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;



class ProjectCamera : public Project {
private:
	const char* vs_path;
	const char* fs_path;
	Shader myShader;

	int show_what;
	int parameter;
	glm::mat4 cameraView;
	bool changeView;
	float ortho_left_right[2];
	float ortho_bottom_top[2];
	float ortho_near_far[2];
	float pers_fov;
	float pers_near_far[2];
	
	float vertices[216] = {
		-2.0f, -2.0f, -2.0f,  0.0f, 0.0f, 1.0f,
		 2.0f, -2.0f, -2.0f,  0.0f, 0.0f, 1.0f,
		 2.0f,  2.0f, -2.0f,  0.0f, 0.0f, 1.0f,
		 2.0f,  2.0f, -2.0f,  0.0f, 0.0f, 1.0f,
		-2.0f,  2.0f, -2.0f,  0.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 0.0f, 1.0f,

		-2.0f, -2.0f,  2.0f,  1.0f, 0.0f, 0.0f,
		 2.0f, -2.0f,  2.0f,  1.0f, 0.0f, 0.0f,
		 2.0f,  2.0f,  2.0f,  1.0f, 0.0f, 0.0f,
		 2.0f,  2.0f,  2.0f,  1.0f, 0.0f, 0.0f,
		-2.0f,  2.0f,  2.0f,  1.0f, 0.0f, 0.0f,
		-2.0f, -2.0f,  2.0f,  1.0f, 0.0f, 0.0f,

		-2.0f,  2.0f,  2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f, -2.0f, -2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f, -2.0f,  2.0f,  0.0f, 1.0f, 0.0f,
		-2.0f,  2.0f,  2.0f,  0.0f, 1.0f, 0.0f,

		 2.0f,  2.0f,  2.0f,  1.0f, 1.0f, 0.0f,
		 2.0f,  2.0f, -2.0f,  1.0f, 1.0f, 0.0f,
		 2.0f, -2.0f, -2.0f,  1.0f, 1.0f, 0.0f,
		 2.0f, -2.0f, -2.0f,  1.0f, 1.0f, 0.0f,
		 2.0f, -2.0f,  2.0f,  1.0f, 1.0f, 0.0f,
		 2.0f,  2.0f,  2.0f,  1.0f, 1.0f, 0.0f,

		-2.0f, -2.0f, -2.0f,  1.0f, 0.0f, 1.0f,
		 2.0f, -2.0f, -2.0f,  1.0f, 0.0f, 1.0f,
		 2.0f, -2.0f,  2.0f,  1.0f, 0.0f, 1.0f,
		 2.0f, -2.0f,  2.0f,  1.0f, 0.0f, 1.0f,
		-2.0f, -2.0f,  2.0f,  1.0f, 0.0f, 1.0f,
		-2.0f, -2.0f, -2.0f,  1.0f, 0.0f, 1.0f,

		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f, 1.0f,
		 2.0f,  2.0f, -2.0f,  0.0f, 1.0f, 1.0f,
		 2.0f,  2.0f,  2.0f,  0.0f, 1.0f, 1.0f,
		 2.0f,  2.0f,  2.0f,  0.0f, 1.0f, 1.0f,
		-2.0f,  2.0f,  2.0f,  0.0f, 1.0f, 1.0f,
		-2.0f,  2.0f, -2.0f,  0.0f, 1.0f, 1.0f,
	};

public:
	ProjectCamera(GLFWwindow* window_);
	GLFWwindow* window;
	void draw();
	void render();
	void updateCameraView(glm::mat4 cameraView_);
	void updateCameraPos(glm::vec3 cameraPos_);
	void changeShowWhat();
};

#endif