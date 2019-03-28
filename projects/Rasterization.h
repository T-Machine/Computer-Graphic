#pragma once
#ifndef _RASTERIZATION_
#define _RASTERIZATION_
#include <vector>
using namespace std;

struct Line {
	int A, B, C;
	Line(int x1, int y1, int x2, int y2) {
		A = y2 - y1;
		B = x1 - x2;
		C = x2 * y1 - x1 * y2;
	}
};

void centerlize_line(Line &L, int x, int y);
void Bresenham_line(int x0, int y0, int x1, int y1, float* vertices, int &counter, int size);
void Bresenham_circle(int r, float* vertices, int &counter, int size);
void draw_circle_8(int x, int y, float* vertices, int &counter, int size);
void Triangle_Rasterization(int x0, int y0, int x1, int y1, int x2, int y2, float* vertices, int &counter, int size);

void centerlize_line(Line &L, int x, int y) {
	if (L.A * x + L.B * y + L.C < 0) {
		L.A = -L.A;
		L.B = -L.B;
		L.C = -L.C;
	}
}

void Bresenham_line(int x0, int y0, int x1, int y1, float* vertices, int &counter, int size) {
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

void Bresenham_circle(int r, float* vertices, int &counter, int size) {
	int x = 0, y = r;
	int p = 3 - 2 * r;

	while (x <= y) {
		draw_circle_8(x, y, vertices, counter, size);
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

void draw_circle_8(int x, int y, float* vertices, int &counter, int size) {
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

void Triangle_Rasterization(int x0, int y0, int x1, int y1, int x2, int y2, float* vertices, int &counter, int size) {
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


#endif // !_RASTERIZATION_

