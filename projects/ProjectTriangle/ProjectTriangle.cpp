#include "ProjectTriangle.h"

ProjectTriangle::ProjectTriangle() {
	vs_path = "../Project1/projects/ProjectTriangle/glsl/shader.vs";
	fs_path = "../Project1/projects/ProjectTriangle/glsl/shader.fs";

	myShader = Shader(vs_path, fs_path);
}

void ProjectTriangle::draw() {
	ImGui::Begin("Change the color of right triangle");
	ImGui::ColorEdit3("Color", (float*)&myColor);

	for (int i = 0; i < 5; i++) {
		vertices[18 + i * 6 + 3] = myColor.x;
		vertices[18 + i * 6 + 4] = myColor.y;
		vertices[18 + i * 6 + 5] = myColor.z;
	}

	ImGui::End();

	//渲染背景颜色
	glClearColor(0.2f, 0.2f, 0.25f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//激活程序
	myShader.use();
	render();
}

void ProjectTriangle::render() {
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//颜色属性
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//render
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_LINES, 8, 2);
	glDrawArrays(GL_POINTS, 10, 10);

	//回收资源
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

}

void ProjectTriangle::updateCameraView(glm::mat4 cameraView_) {
	//cameraView = cameraView_;
}

void ProjectTriangle::updateCameraPos(glm::vec3 cameraPos_) {
	//cameraPos = cameraPos_;
}