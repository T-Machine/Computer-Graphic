#include "ProjectCamera.h"

using namespace std;

ProjectCamera::ProjectCamera(GLFWwindow* window_) {
	vs_path = "../Project1/projects/ProjectCamera/glsl/shader.vs";
	fs_path = "../Project1/projects/ProjectCamera/glsl/shader.fs";

	myShader = Shader(vs_path, fs_path);
	show_what = 0;
	parameter = 0;
	cameraView = glm::mat4(1.0f);
	window = window_;
	changeView = false;
}

void ProjectCamera::draw() {
	
	ImGui::Begin("Camera");
	ImGui::RadioButton("Orthographic", &show_what, 0);
	ImGui::RadioButton("Perspective", &show_what, 1);
	ImGui::RadioButton("View changing", &show_what, 2);
	ImGui::RadioButton("Move camera", &show_what, 3);
	ImGui::Separator();

	if (show_what == 0) {
		ImGui::Checkbox("Change view", &changeView);
		ImGui::SliderFloat2("left & right", ortho_left_right, -20.0f, 20.0f);
		ImGui::SliderFloat2("bottom & top", ortho_bottom_top, -20.0f, 20.0f);
		ImGui::SliderFloat2("near & far", ortho_near_far, -20.0f, 20.0f);
	}
	if (show_what == 1) {
		ImGui::Checkbox("Change view", &changeView);
		ImGui::SliderFloat("FOV", &pers_fov, 0.0f, 90.0f);
		ImGui::SliderFloat2("near & far", pers_near_far, 0.0f, 100.0f);
	}
	else if (show_what == 2) {
		ImGui::RadioButton("radius 1", &parameter, 0);
		ImGui::RadioButton("radius 2", &parameter, 1);
	}
	else if (show_what == 3) {
		//�������
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	ImGui::End();

	
	//��Ȳ���
	glEnable(GL_DEPTH_TEST);

	//��Ⱦ������ɫ
	glClearColor(0.2f, 0.2f, 0.25f, 0.0f);
	//�����Ȼ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//�������
	myShader.use();
	render();
}

void ProjectCamera::render() {
	//����VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//����VBO�洢��������
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//���������ݸ��Ƶ�����
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//λ������
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//��ɫ����
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);



	//GLM
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	float getTime = (float)glfwGetTime();
	float r = 0.0f;

	//��ʼλ��
	if (show_what == 0 || show_what == 1) {
		model = glm::translate(model, glm::vec3(-1.5f, 0.5f, -1.5f));
		//תһ��
		//model = glm::rotate(model, 1.0f, glm::vec3(0.2f, 0.0f, 0.2f));
		if (changeView) {
			view = glm::rotate(view, 1.0f, glm::vec3(0.2f, 0.0f, 0.2f));
		}
	}

	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -15.0f));

	//����
	if (show_what == 0) {
		projection = glm::ortho(ortho_left_right[0], ortho_left_right[1], ortho_bottom_top[0], ortho_bottom_top[1], ortho_near_far[0], ortho_near_far[1]);
	}
	//͸��
	else if (show_what == 1) {
		projection = glm::perspective(glm::radians(pers_fov), (float)SCREEN_W / (float)SCREEN_H, pers_near_far[0], pers_near_far[1]);
	}
	//��ת�����
	else if (show_what == 2) {
		projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_W / (float)SCREEN_H, 0.1f, 100.0f);
		r = parameter == 0 ? 10.0f : 15.0f;
		float camPosX = sin(getTime) * r;
		float camPosZ = cos(getTime) * r;
		view = glm::lookAt(glm::vec3(camPosX, 0.0, camPosZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	}
	//�ƶ������
	else if (show_what == 3) {
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_W / (float)SCREEN_H, 0.1f, 100.0f);
		view = cameraView;
	}

	

	//uniform
	myShader.setMat4("model", model);
	myShader.setMat4("view", view);
	myShader.setMat4("projection", projection);


	// draw
	glDrawArrays(GL_TRIANGLES, 0, 36);

	//������Դ
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	
}

void ProjectCamera::updateCameraView(glm::mat4 cameraView_) {
	cameraView = cameraView_;
}

void ProjectCamera::updateCameraPos(glm::vec3 cameraPos_) {
	//cameraPos = cameraPos_;
}

void ProjectCamera::changeShowWhat() {
	//ȡ����겶��
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	show_what = 2;
}