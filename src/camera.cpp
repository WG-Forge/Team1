#include "camera.h"

void Camera::update(int dx, int dy) {
    cameraX += dx;
    cameraY += dy;
}

int Camera::getCameraX() const {
    return cameraX;
}

void Camera::setCameraX(int cameraX_) {
    Camera::cameraX = cameraX_;
}

int Camera::getCameraY() const {
    return cameraY;
}

void Camera::setCameraY(int cameraY_) {
    Camera::cameraY = cameraY_;
}
