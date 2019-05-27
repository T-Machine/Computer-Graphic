#include "ProjectBezier.h"

ProjectBezier::ProjectBezier(GLFWwindow* window_) {
	vs_path = "../Project1/projects/ProjectBezier/glsl/shader.vs";
	fs_path = "../Project1/projects/ProjectBezier/glsl/shader.fs";
	myShader = Shader(vs_path, fs_path);

	for (int i = 0; i < MAX_POINT; i++) {
		vertices[i] = 0.0f;
	}
	for (int i = 0; i < 100; i++) {
		controlVertices[i] = 0.0f;
	}
	for (int i = 0; i < 100; i++) {
		assistVertices[i] = 0.0f;
	}
	factorial[0] = 1;
	for (int i = 1; i < 13; i++) {
		factorial[i] = i * factorial[i - 1];
	}

	window = window_;
	controlNum = 0;
	assistNum = 0;
	verticesNum = 0;
	frame = 0.0;
}

int ProjectBezier::getFactorial(int n) {
	return n > 12 ? 1 : factorial[n];
}

void ProjectBezier::draw() {

	//ImGui::Begin("Bezier");
	//ImGui::End();

	//渲染背景颜色
	glClearColor(0.2f, 0.2f, 0.25f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//激活程序
	myShader.use();
	render();
}

void ProjectBezier::render() {
	// clear
	verticesNum = 0;

	//创建VAO
	// unsigned int controlVAO;
	glGenVertexArrays(1, &controlVAO);
	glBindVertexArray(controlVAO);
	//创建VBO存储顶点数据
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//将顶点数据复制到缓存
	glBufferData(GL_ARRAY_BUFFER, sizeof(controlVertices), controlVertices, GL_STATIC_DRAW);
	//位置属性
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// draw control points
	glBindVertexArray(controlVAO);
	myShader.setVec3("pointColor", glm::vec3(0.8, 0.0, 0.0));
	if (controlNum > 0) {
		glPointSize(8);
		glDrawArrays(GL_POINTS, 0, controlNum);
	}
	if (controlNum > 1) {
		glDrawArrays(GL_LINE_STRIP, 0, controlNum);
	}

	// draw Bezier line
	// unsigned int bezierVAO;
	if (controlNum > 1) {
		createBezierLine();
		glGenVertexArrays(1, &bezierVAO);
		glBindVertexArray(bezierVAO);
		glDeleteBuffers(1, &VBO);
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glBindVertexArray(bezierVAO);
		myShader.setVec3("pointColor", glm::vec3(1.0, 0.5, 0.2));
		glPointSize(3);
		glDrawArrays(GL_POINTS, 0, verticesNum);
	}

	// draw assist lines
	myShader.setVec3("pointColor", glm::vec3(0.0, 1.0, 1.0));
	frame += 0.01;
	frame = frame > 1 ? 0 : frame;

	createAssistLines(frame);
	

	//回收资源
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &controlVAO);
	glDeleteVertexArrays(1, &bezierVAO);
	glDeleteVertexArrays(1, &assistVAO);
	glDeleteBuffers(1, &VBO);

}

void ProjectBezier::createBezierLine() {
	int n = controlNum - 1;
	for (float t = 0.0; t < 1.0; t += 0.001) {
		glm::vec2 p = controlPoints[0] * Bernstein(0, n, t);
		for (int i = 1; i <= n; i++) {
			p = p + controlPoints[i] * Bernstein(i, n, t);
		}
		vertices[verticesNum * 2] = p.x;
		vertices[verticesNum * 2 + 1] = p.y;
		verticesNum++;
	}
}

float ProjectBezier::Bernstein(int i, int n, float t) {
	return getFactorial(n) / (getFactorial(i) * getFactorial(n - i)) * pow(t, i) * pow(1 - t, n - i);
}

void ProjectBezier::createAssistLines(float t) {
	//int count = controlNum;
	// init assistVertices
	for (int i = 0; i < controlNum * 2; i++) {
		assistVertices[i] = controlVertices[i];
	}

	for (int count = controlNum; count > 1; count--) {
		for (int i = 0; i < count - 1; i++) {
			// 利用向量计算中间点
			glm::vec2 p1 = glm::vec2(assistVertices[i * 2], assistVertices[i * 2 + 1]);
			glm::vec2 p2 = glm::vec2(assistVertices[(i+1) * 2], assistVertices[(i+1) * 2 + 1]);
			glm::vec2 P = p1 * (1 - t) + p2 * t;

			// test
			assistVertices[i * 2] = P.x;
			assistVertices[i * 2 + 1] = P.y;
		}
		drawAssistLines(count - 1);
	}
	
}

void ProjectBezier::drawAssistLines(int count) {
	glGenVertexArrays(1, &assistVAO);
	glBindVertexArray(assistVAO);
	glDeleteBuffers(1, &VBO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(assistVertices), assistVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(assistVAO);
	glPointSize(6);
	glDrawArrays(GL_POINTS, 0, count);
	glDrawArrays(GL_LINE_STRIP, 0, count);
}

void ProjectBezier::addControlPoint(glm::vec2 p) {
	controlPoints.push_back(p);
	controlVertices[controlNum * 2] = p.x;
	controlVertices[controlNum * 2 + 1] = p.y;
	controlNum++;
}

void ProjectBezier::deleteControlPoint() {
	if (controlNum > 0) {
		controlPoints.pop_back();
		controlNum--;
	}
}


void ProjectBezier::updateCameraView(glm::mat4 cameraView_) {
	// ...
}
void ProjectBezier::updateCameraPos(glm::vec3 cameraPos_) {
	// ...
}