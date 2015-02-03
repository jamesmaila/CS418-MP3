#include "camera.h"
#include "vector.h"

using namespace std;

/*
 * Camera
 *     Inputs: None.
 *     Returns: None.
 *     Description: The default constructor for camera.
 *                  Applies initial settings for the object.
 */
Camera::Camera()
{
    Eye = Vector(0.0, 2.0, 5.0);
    Center = Vector(0.0, 1.25, 0.0);
    Up = Vector(0.0, 1.0, 0.0);

    Translate = Vector(0.0, 0.0, -3.0);
    RotateX = Vector(1.0, 0.0, 0.0);
    RotateY = Vector(0.0, 1.0, 0.0);

    RotateAngleX = 20.0;
    RotateAngleY = 0.0;
}

