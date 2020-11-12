#include "camera.h"

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
