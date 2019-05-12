#include "ProjectRasterization.h"

ProjectRasterization::ProjectRasterization() {
	vs_path = "../Project1/projects/ProjectRasterization/glsl/shader.vs";
	fs_path = "../Project1/projects/ProjectRasterization/glsl/shader.fs";

	for (int i = 0; i < MAX_POINT; i++) {
		vertices[i] = 0.0f;
	}

	counter = 0;
	show_what = 0;
	radius = 0;
	show_Rasterization = false;
	size = WIN_W;

	myShader = Shader(vs_path, fs_path);
}

void ProjectRasterization::init() {

}

void ProjectRasterization::draw() {
	counter = 0;

	ImGui::Begin("Draw triangle or circle");
	ImGui::RadioButton("Triangle", &show_what, 0);
	ImGui::RadioButton("Circle", &show_what, 1);

	if (show_what == 0) {
		ImGui::SliderInt2("vertex1", vertex1, -WIN_H + 5, WIN_H - 5);
		ImGui::SliderInt2("vertex2", vertex2, -WIN_H + 5, WIN_H - 5);
		ImGui::SliderInt2("vertex3", vertex3, -WIN_H + 5, WIN_H - 5);

		Bresenham_line(vertex1[0], vertex1[1], vertex2[0], vertex2[1]);
		Bresenham_line(vertex1[0], vertex1[1], vertex3[0], vertex3[1]);
		Bresenham_line(vertex2[0], vertex2[1], vertex3[0], vertex3[1]);

		ImGui::Checkbox("Triangle Rasterization", &show_Rasterization);

		if (show_Rasterization) {
			Triangle_Rasterization(vertex1[0], vertex1[1], vertex2[0], vertex2[1], vertex3[0], vertex3[1]);
		}
	}
	else if (show_what == 1) {
		ImGui::SliderInt("Radius", &radius, 0, WIN_H - 5);
		Bresenham_circle(radius);
	}

	ImGui::End();

	//渲染背景颜色
	glClearColor(0.2f, 0.2f, 0.25f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glPointSize(POINT_SIZE);

	//激活程序
	myShader.use();
	render();
}

void ProjectRasterization::render() {
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glDrawArrays(GL_POINTS, 0, counter);

	//回收资源
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void ProjectRasterization::clear() {
	
}

void ProjectRasterization::centerlize_line(Line &L, int x, int y) {
	if (L.A * x + L.B * y + L.C < 0) {
		L.A = -L.A;
		L.B = -L.B;
		L.C = -L.C;
	}
}

void ProjectRasterization::Bresenham_line(int x0, int y0, int x1, int y1) {
	int temp = 0, flag = 0;

	int dx = abs(x1 - x0), dy = abs(y1 - y0);
	if (dy > dx) {
		flag = 1;
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	dx = abs(x1 - x0);
	dy = abs(y1 - y0);
	int x = x0, y = y0;
	int dy_2 = dy * 2;
	int dx_2 = dx * 2;

	int p = -dx;
	while (x <= x1) {
		if (p <= 0) {
			p = p + dy_2;
		}
		else {
			if (y1 >= y0) {
				y++;
			}
			else {
				y--;
			}
			p = p + dy_2 - dx_2;
		}
		if (flag == 0) {
			vertices[counter * 2] = (float)(x + 1) / size;
			vertices[counter * 2 + 1] = (float)y / size;
		}
		else {
			vertices[counter * 2] = (float)y / size;
			vertices[counter * 2 + 1] = (float)(x + 1) / size;
		}
		counter++;
		x++;
	}
}

void ProjectRasterization::Bresenham_circle(int r) {
	int x = 0, y = r;
	int p = 3 - 2 * r;

	while (x <= y) {
		draw_circle_8(x, y);
		if (p < 0) {
			p += 4 * x + 6;
		}
		else {
			p += 4 * (x - y) + 10;
			y--;
		}
		x++;
	}
}

void ProjectRasterization::draw_circle_8(int x, int y) {
	int counter_2 = counter * 2;
	vertices[counter_2] = (float)x / size;
	vertices[counter_2 + 1] = (float)y / size;
	vertices[counter_2 + 2] = (float)x / size;
	vertices[counter_2 + 3] = -(float)y / size;
	vertices[counter_2 + 4] = -(float)x / size;
	vertices[counter_2 + 5] = (float)y / size;
	vertices[counter_2 + 6] = -(float)x / size;
	vertices[counter_2 + 7] = -(float)y / size;

	vertices[counter_2 + 8] = (float)y / size;
	vertices[counter_2 + 9] = (float)x / size;
	vertices[counter_2 + 10] = (float)y / size;
	vertices[counter_2 + 11] = -(float)x / size;
	vertices[counter_2 + 12] = -(float)y / size;
	vertices[counter_2 + 13] = (float)x / size;
	vertices[counter_2 + 14] = -(float)y / size;
	vertices[counter_2 + 15] = -(float)x / size;
	counter += 8;
}

void ProjectRasterization::Triangle_Rasterization(int x0, int y0, int x1, int y1, int x2, int y2) {
	vector<Line> Lines;
	int max_x = max(x0, max(x1, x2));
	int max_y = max(y0, max(y1, y2));
	int min_x = min(x0, min(x1, x2));
	int min_y = min(y0, min(y1, y2));
	Lines.push_back(Line(x0, y0, x1, y1));
	Lines.push_back(Line(x1, y1, x2, y2));
	Lines.push_back(Line(x0, y0, x2, y2));

	centerlize_line(Lines[0], x2, y2);
	centerlize_line(Lines[1], x0, y0);
	centerlize_line(Lines[2], x1, y1);

	for (int x = min_x; x <= max_x; x++) {
		for (int y = min_y; y <= max_y; y++) {
			bool inside = true;
			for (int i = 0; i < 3; i++) {
				if (Lines[i].A * x + Lines[i].B * y + Lines[i].C < 0) {
					inside = false;
					break;
				}
			}
			if (inside) {
				vertices[counter * 2] = (float)x / size;
				vertices[counter * 2 + 1] = (float)y / size;
				counter++;
			}
		}
	}
}

void ProjectRasterization::updateCameraView(glm::mat4 cameraView_) {
	//cameraView = cameraView_;
}

void ProjectRasterization::updateCameraPos(glm::vec3 cameraPos_) {
	//cameraPos = cameraPos_;
}
