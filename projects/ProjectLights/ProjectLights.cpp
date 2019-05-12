#include "ProjectLights.h"

using namespace std;

ProjectLights::ProjectLights() {
	vs_path = "../Project1/projects/ProjectLights/glsl/shader.vs";
	fs_path = "../Project1/projects/ProjectLights/glsl/shader.fs";
	light_vs_path = "../Project1/projects/ProjectLights/glsl/light_shader.vs";
	light_fs_path = "../Project1/projects/ProjectLights/glsl/light_shader.fs";
	Gourand_vs_path = "../Project1/projects/ProjectLights/glsl/Gourand_shader.vs";
	Gourand_fs_path = "../Project1/projects/ProjectLights/glsl/Gourand_shader.fs";

	cubeShader = Shader(vs_path, fs_path);
	lightShader = Shader(light_vs_path, light_fs_path);
	GourandShader = Shader(Gourand_vs_path, Gourand_fs_path);

	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	objectColor = glm::vec3(0.5f, 1.0f, 0.3f);
	lightPos = glm::vec3(1.2f, 1.0f, 1.2f);
	viewPos = glm::vec3(0.0f, 0.0f, 0.0f);
	ambientStrength = 0.1f;
	shininess = 32;
	specularStrength = 0.5f;
	diffuseStrength = 1.0f;

	r = 4.0f;
	lightRPos = 1.2f;
	lightYPos = 1.0f;
	viewTheta = 0.0f;
	viewFai = 0.5f;
	//viewPosZ = 0.0f;

	shading_mode = 0;
	show_what = 0;
}

void ProjectLights::draw() {
	
	ImGui::Begin("Lights");
	
	ImGui::SliderFloat("view theta", &viewTheta, 0.0f, 6.28f);
	ImGui::SliderFloat("view fai", &viewFai, 0.1f, 3.14f);
	ImGui::Separator();
	ImGui::RadioButton("Static light", &show_what, 0);
	ImGui::RadioButton("moving light", &show_what, 1);

	if (show_what == 1) {
		ImGui::Separator();
		ImGui::SliderFloat("radius", &lightRPos, 1.0f, 2.0f);
		ImGui::SliderFloat("height", &lightYPos, -2.0f, 2.0f);
	}
	ImGui::Separator();
	ImGui::RadioButton("Phong Shading", &shading_mode, 0);
	ImGui::RadioButton("Gourand Shading", &shading_mode, 1);

	ImGui::Separator();
	ImGui::SliderFloat("ambientStrength", &ambientStrength, 0.0f, 1.0f);
	ImGui::SliderFloat("diffuseStrength", &diffuseStrength, 0.0f, 2.0f);
	ImGui::SliderFloat("specularStrength", &specularStrength, 0.0f, 3.0f);
	ImGui::SliderInt("shininess", &shininess, 1, 256);


	ImGui::End();


	//深度测试
	glEnable(GL_DEPTH_TEST);

	//渲染背景颜色
	glClearColor(0.2f, 0.2f, 0.25f, 0.0f);
	//清除深度缓冲
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//激活程序
	//myShader.use();
	render();
}

void ProjectLights::render() {
	//创建VAO
	unsigned int cubeVAO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	//创建VBO存储顶点数据
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//将顶点数据复制到缓存
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//位置属性
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//法线属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//创建光源VAO
	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	//再建VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	
	//GLM
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	// matrix
	//view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));
	//view = glm::rotate(view, 1.0f, glm::vec3(1.0f, -1.0f, 0.0f));
	float camPosX = sin(viewFai) * sin(viewTheta) * r;
	float camPosZ = sin(viewFai) * cos(viewTheta) * r;
	float camPosY = cos(viewFai) * r;
	view = glm::lookAt(glm::vec3(camPosX, camPosY, camPosZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_W / (float)SCREEN_H, 0.1f, 100.0f);
	//uniform
	float getTime = (float)glfwGetTime();
	viewPos = glm::vec3(camPosX, camPosY, camPosZ);
	if (show_what == 1) {
		lightPos = glm::vec3(sin(getTime) * lightRPos, lightYPos, cos(getTime) * lightRPos);
	}

	// shader1
	if (shading_mode == 0) {
		cubeShader.use();
		//parm
		cubeShader.setFloat("ambientStrength", ambientStrength);
		cubeShader.setFloat("specularStrength", specularStrength);
		cubeShader.setFloat("diffuseStrength", diffuseStrength);
		cubeShader.setInt("shininess", shininess);
		// vector
		cubeShader.setVec3("objectColor", objectColor);
		cubeShader.setVec3("lightColor", lightColor);
		cubeShader.setVec3("lightPos", lightPos);
		cubeShader.setVec3("viewPos", viewPos);
		// matrix
		cubeShader.setMat4("model", model);
		cubeShader.setMat4("view", view);
		cubeShader.setMat4("projection", projection);
	}
	else if (shading_mode == 1) {
		GourandShader.use();
		//parm
		GourandShader.setFloat("ambientStrength", ambientStrength);
		GourandShader.setFloat("specularStrength", specularStrength);
		GourandShader.setFloat("diffuseStrength", diffuseStrength);
		GourandShader.setInt("shininess", shininess);
		// vector
		GourandShader.setVec3("objectColor", objectColor);
		GourandShader.setVec3("lightColor", lightColor);
		GourandShader.setVec3("lightPos", lightPos);
		GourandShader.setVec3("viewPos", viewPos);
		// matrix
		GourandShader.setMat4("model", model);
		GourandShader.setMat4("view", view);
		GourandShader.setMat4("projection", projection);
	}
	
	// draw cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// shader2
	lightShader.use();
	model = glm::translate(model, lightPos);
	model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
	lightShader.setMat4("model", model);
	lightShader.setMat4("view", view);
	lightShader.setMat4("projection", projection);
	// draw lamp
	glBindVertexArray(lightVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);


	//回收资源
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);
	
}

void ProjectLights::updateCameraView(glm::mat4 cameraView_) {
	//cameraView = cameraView_;
}

void ProjectLights::updateCameraPos(glm::vec3 cameraPos_) {
	//cameraPos = cameraPos_;
}