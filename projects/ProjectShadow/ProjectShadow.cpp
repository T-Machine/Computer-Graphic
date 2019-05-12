#include "ProjectShadow.h"

using namespace std;

ProjectShadow::ProjectShadow(GLFWwindow* window_) {
	sceneShader_vs_path = "../Project1/projects/ProjectShadow/glsl/sceneShader.vs";
	sceneShader_fs_path = "../Project1/projects/ProjectShadow/glsl/sceneShader.fs";
	depthShader_vs_path = "../Project1/projects/ProjectShadow/glsl/depthShader.vs";
	depthShader_fs_path = "../Project1/projects/ProjectShadow/glsl/depthShader.fs";
	sceneShader = Shader(sceneShader_vs_path, sceneShader_fs_path);
	depthShader = Shader(depthShader_vs_path, depthShader_fs_path);

	cameraView = glm::mat4(1.0f);
	cameraPos = glm::vec3(1.0f);
	window = window_;

	r = 4.0f;
	viewTheta = 0.0f;
	viewFai = 0.5f;
	project_mode = 0;

	lightPos_Y = 5.0f;
	lightPos_X = -2.0f;
	lightPos_Z = -1.0f;
	cubeMove_Y = 0.0f;
	cubeMove_X = 0.0f;
	cubeMove_Z = 0.0f;
	lightPos = glm::vec3(lightPos_X, lightPos_Y, lightPos_Z);
	SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;


	// 为生成Depth Map，需要一个FBO对象
	// unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// 创建2D纹理，供帧缓冲的深度缓冲使用
	// unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// 把生成的深度纹理作为FBO的深度缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	// 设置GL_NONE，不使用颜色数据进行渲染
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	sceneShader.use();
	// 传入深度贴图
	sceneShader.setInt("shadowMap", 0);

}

void ProjectShadow::draw() {
	ImGui::Begin("Shadow");

	ImGui::RadioButton("Orthographic", &project_mode, 0);
	ImGui::RadioButton("Perspective", &project_mode, 1);
	ImGui::Separator();
	ImGui::SliderFloat("Light Pos_Y", &lightPos_Y, 0.0f, 7.5f);
	ImGui::SliderFloat("Light Pos_X", &lightPos_X, -2.0f, 3.0f);
	ImGui::SliderFloat("Light Pos_Z", &lightPos_Z, -2.0f, 3.0f);
	ImGui::Separator();
	ImGui::SliderFloat("move cube Y", &cubeMove_Y, -2.0f, 2.0f);
	ImGui::SliderFloat("move cube X", &cubeMove_X, -2.0f, 2.0f);
	ImGui::SliderFloat("move cube Z", &cubeMove_Z, -2.0f, 2.0f);
	ImGui::Separator();
	ImGui::SliderFloat("view theta", &viewTheta, 0.0f, 6.28f);
	ImGui::SliderFloat("view fai", &viewFai, 0.1f, 3.14f);
	ImGui::SliderFloat("distance", &r, 4.0f, 20.0f);

	ImGui::End();

	lightPos = glm::vec3(lightPos_X, lightPos_Y, lightPos_Z);

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

void ProjectShadow::render() {
	// ------第一步，渲染深度贴图------
	// 光源空间的变换
	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 7.5f;
	if (project_mode == 0) {
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
	}
	else if (project_mode == 1) {
		lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane);
	}
	
	lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
	lightSpaceMatrix = lightProjection * lightView;
	depthShader.use();
	depthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	renderObject(depthShader);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// ------第二步，使用深度贴图计算阴影------
	// set VirePort to screen size & clear buffer
	glViewport(0, 0, SCREEN_W, SCREEN_H);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	sceneShader.use();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCREEN_W / (float)SCREEN_H, 0.1f, 100.0f);

	glm::mat4 view = cameraView;
	// change view
	float camPosX = sin(viewFai) * sin(viewTheta) * r;
	float camPosZ = sin(viewFai) * cos(viewTheta) * r;
	float camPosY = cos(viewFai) * r;
	view = glm::lookAt(glm::vec3(camPosX, camPosY, camPosZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

	// uniform
	sceneShader.setMat4("projection", projection);
	sceneShader.setMat4("view", view);
	sceneShader.setVec3("viewPos", cameraPos);
	sceneShader.setVec3("lightPos", lightPos);
	sceneShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
	// texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	renderObject(sceneShader);

}

void ProjectShadow::renderObject(const Shader &shader) {
	// render plane
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", model);

	unsigned int planeVAO = 0;
	unsigned int planeVBO = 0;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);	// colors
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);

	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);


	// render cube
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f + cubeMove_X, 1.5f + cubeMove_Y, 0.0 + cubeMove_Z));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setMat4("model", model);

	unsigned int cubeVAO = 0;
	unsigned int cubeVBO = 0;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);
	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);	// positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);	// normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);	// colors
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	// delete
	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteBuffers(1, &cubeVBO);
}

void ProjectShadow::updateCameraView(glm::mat4 cameraView_) {
	//cameraView = cameraView_;
}

void ProjectShadow::updateCameraPos(glm::vec3 cameraPos_) {
	cameraPos = cameraPos_;
}