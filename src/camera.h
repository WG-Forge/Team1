#ifndef RAIL_SIMULATOR_CAMERA_H
#define RAIL_SIMULATOR_CAMERA_H

class Camera
{
  public:
    Camera() = default;

    int GetCameraX() const;
    int GetCameraY() const;
    void SetCameraX(int cameraX);
    void SetCameraY(int cameraY);

  private:
    int cameraX = 0;
    int cameraY = 0;
};

#endif // RAIL_SIMULATOR_CAMERA_H
