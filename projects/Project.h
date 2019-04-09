#pragma once
#ifndef _PROJECT_
#define _PROJECT_

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_glfw.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../shader/shader.h"
#include "tool.h"

#define SCREEN_W 800
#define SCREEN_H 600

using namespace std;

class Project {
private:
	const char* vs_path;
	const char* fs_path;
	Shader myShader;

public:
	virtual void draw() = 0;
};


#endif