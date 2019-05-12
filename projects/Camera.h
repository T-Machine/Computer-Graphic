#pragma once
#ifndef _CAMERA_
#define _CAMERA_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

using namespace std;

class Camera {
public:
	Camera(float zPos);
	glm::mat4 getCameraView();
	glm::vec3 getCameraPos();
	void moveForward(float speed);
	void moveBack(float speed);
	void moveRight(float speed);
	void moveLeft(float speed);
	void rotateCamera(float yaw, float pitch);

private:
	glm::mat4 cameraView;
	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;
};

//构造时初始化Z轴位置
Camera::Camera(float zPos) {
	cameraView = glm::mat4(1.0f);
	cameraPos = glm::vec3(0.0f, 0.0f, zPos);
	cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
}

glm::mat4 Camera::getCameraView() {
	cameraView = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	return cameraView;
}

glm::vec3 Camera::getCameraPos() {
	return cameraPos;
}

void Camera::moveForward(float speed) {
	cameraPos += cameraFront * speed;
}

void Camera::moveBack(float speed) {
	cameraPos -= cameraFront * speed;
}

void Camera::moveRight(float speed) {
	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
}

void Camera::moveLeft(float speed) {
	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
}

void Camera::rotateCamera(float yaw, float pitch) {
	glm::vec3 front;
	front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);
}

#endif