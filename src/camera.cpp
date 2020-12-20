#include "camera.h"

int Camera::GetCameraX() const
{
    return cameraX;
}

int Camera::GetCameraY() const
{
    return cameraY;
}

void Camera::SetCameraX(int cameraX_)
{
    Camera::cameraX = cameraX_;
}

void Camera::SetCameraY(int cameraY_)
{
    Camera::cameraY = cameraY_;
}
