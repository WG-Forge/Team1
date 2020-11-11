#include "Camera.h"

void Camera::updateX(int dx) {
    cameraX += dx;
}

void Camera::updateY(int dy) {
    cameraY += dy;
}

void Camera::update(int dx, int dy) {
    cameraX += dx;
    cameraY += dy;
}

int Camera::getCameraX() const {
    return cameraX;
}

void Camera::setCameraX(int cameraX) {
    Camera::cameraX = cameraX;
}

int Camera::getCameraY() const {
    return cameraY;
}

void Camera::setCameraY(int cameraY) {
    Camera::cameraY = cameraY;
}
