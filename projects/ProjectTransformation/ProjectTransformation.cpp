#include "ProjectTransformation.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace std;

ProjectTransformation::ProjectTransformation() {
	vs_path = "../Project1/projects/ProjectTransformation/glsl/shader.vs";
	fs_path = "../Project1/projects/ProjectTransformation/glsl/shader.fs";

	myShader = Shader(vs_path, fs_path);
	isTestDepth = false;
	show_what = 0;
}

void ProjectTransformation::draw() {
	
	ImGui::Begin("Transformation");
	ImGui::Checkbox("Open depth test", &isTestDepth);
	ImGui::Separator();
	
	if(ImGui::CollapsingHeader("Choose Transformation")) {
		ImGui::RadioButton("Translation", &show_what, 0);
		ImGui::RadioButton("Rotation", &show_what, 1);
		ImGui::RadioButton("Scaling", &show_what, 2);
		ImGui::RadioButton("Bonus", &show_what, 3);
	}

	ImGui::End();

	if (isTestDepth) {
		//深度测试
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}

	//渲染背景颜色
	glClearColor(0.2f, 0.2f, 0.25f, 0.0f);
	//清除深度缓冲
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//激活程序
	//myShader.use();
	render();
}

void ProjectTransformation::render() {
	//创建VAO
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//创建VBO存储顶点数据
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//将顶点数据复制到缓存
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//纹理属性
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//设置纹理
	unsigned int texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int w, h, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("resources/worry.jpg", &w, &h, &nrChannels, 0);
	if (data) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
	myShader.use();
	myShader.setInt("texture1", 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	//myShader.use();

	//GLM
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);

	float getTime = (float)glfwGetTime();
	if (show_what == 0) {		
		float translateValue = sin(getTime) * 2;
		model = glm::translate(model, glm::vec3(translateValue, 0.0f, 0.0f));
		model = glm::rotate(model, 1.0f, glm::vec3(0.2f, 0.0f, 0.0f));
	}
	else if (show_what == 1) {
		model = glm::rotate(model, getTime, glm::vec3(1.0f, 0.0f, 1.0f));
	}
	else if (show_what == 2) {
		float scaleValue = abs(sin(getTime)) + 0.2f;
		model = glm::rotate(model, 1.0f, glm::vec3(0.0f, 1.0f, 1.0f));
		model = glm::scale(model, glm::vec3(scaleValue, scaleValue, scaleValue));	
	}
	else if (show_what == 3) {
		//..
	}
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -15.0f));
	if (show_what == 3) {
		view = glm::rotate(view, 0.5f, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_W / (float)SCREEN_H, 0.1f, 100.0f);

	//uniform
	myShader.setMat4("model", model);
	myShader.setMat4("view", view);
	myShader.setMat4("projection", projection);

	//draw array
	if (show_what == 3) {
		for (unsigned int i = 0; i < 3; i++) {
			//model = glm::translate(model, cubePositions[i]);
			if (i == 0) {
				model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
				model = glm::rotate(model, getTime / 3, glm::vec3(0.0f, 1.0f, 0.0f));
			}
			else if (i == 1) { 
				model = glm::translate(model, glm::vec3(sin(getTime)*7, 0.0f, cos(getTime)*7));
				model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
				model = glm::rotate(model, getTime, glm::vec3(0.0f, 1.0f, 0.0f));
			}
			else if (i == 2) {
				model = glm::translate(model, glm::vec3(sin(getTime*2)*4.0, 0.0f, cos(getTime*2)*4.0));
				model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			}

			myShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	
	//glDrawArrays(GL_TRIANGLES, 0, 36);

	//回收资源
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	
}

void ProjectTransformation::updateCameraView(glm::mat4 cameraView_) {
	//cameraView = cameraView_;
}

void ProjectTransformation::updateCameraPos(glm::vec3 cameraPos_) {
	//cameraPos = cameraPos_;
}