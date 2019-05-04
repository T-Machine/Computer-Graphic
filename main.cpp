#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "projects/Camera.h"
#include "projects/Project.h"
#include "projects/ProjectRasterization/ProjectRasterization.h"
#include "projects/ProjectTriangle/ProjectTriangle.h"
#include "projects/ProjectTransformation/ProjectTransformation.h"
#include "projects/ProjectCamera/ProjectCamera.h"
#include "projects/ProjectLights/ProjectLights.h"


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define MAX_POINT 42000

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow *window);

const char* glsl_version = "#version 330";
Camera *camera;

//���ڼ�����֡ʱ���
float deltaTime = 0.0f;
float lastFrame = 0.0f;
//���ڼ�������ƶ�
float last_mouseX = 400.0f, last_mouseY = 300.0f, yaw = -90.0f, pitch = 0.0f;
bool firstMove = true;

Project *project;

int main()
{
	//ʵ����GLFW����
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//�������ڶ���
	GLFWwindow* window = glfwCreateWindow(SCREEN_W, SCREEN_H, "HW2", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	//����������������Ϊ��ǰ�߳���������
	glfwMakeContextCurrent(window);
	//ע�ᴰ�����Żص�����
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	//ע������ƶ��ص�����
	glfwSetCursorPosCallback(window, mouse_callback);

	//��ʼ��GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		return -1;
	}

	//imgui
	ImGui::CreateContext();
	ImGuiIO &io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	//Project *project = new ProjectRasterization();
	//Project *project = new ProjectTriangle();
	//Project *project = new ProjectTransformation();
	//project = new ProjectCamera(window);
	Project *project = new ProjectLights();

	camera = new Camera(10.0f);

	while (!glfwWindowShouldClose(window))
	{
		//����ʱ���
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//��������
		processInput(window);

		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//((ProjectCamera*)project)->updateCameraView(camera->getCameraView());
		project->draw();

		//ImGui::ShowDemoWindow(&show_demo_window);

		//imgui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		//glfwPollEvents();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	//������Դ
	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float speed = 2.5f * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->moveForward(speed);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->moveBack(speed);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->moveLeft(speed);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->moveRight(speed);

	//if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
	//	((ProjectCamera*)project)->changeShowWhat();
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMove) {
		last_mouseX = xpos;
		last_mouseY = ypos;
		firstMove = false;
	}

	yaw += (xpos - last_mouseX) * 0.05;
	pitch += (last_mouseY - ypos) * 0.05;
	pitch = pitch > 89.0f ? 89.0f : pitch;
	pitch = pitch < -89.0f ? -89.0f : pitch;

	last_mouseX = xpos;
	last_mouseY = ypos;

	camera->rotateCamera(yaw, pitch);

}