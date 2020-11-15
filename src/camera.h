#ifndef RAIL_SIMULATOR_CAMERA_H
#define RAIL_SIMULATOR_CAMERA_H

class Camera
{
  private:
    int cameraX = 0;
    int cameraY = 0;

  public:
    Camera() = default;

    void update(int dx, int dy);

    int getCameraX() const;

    void setCameraX(int cameraX);

    int getCameraY() const;

    void setCameraY(int cameraY);
};

#endif // RAIL_SIMULATOR_CAMERA_H
