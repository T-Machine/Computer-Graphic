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

	//��Ⱦ������ɫ
	glClearColor(0.2f, 0.2f, 0.25f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//�������
	myShader.use();
	render();
}

void ProjectTriangle::render() {
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

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//render
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_LINES, 8, 2);
	glDrawArrays(GL_POINTS, 10, 10);

	//������Դ
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

}