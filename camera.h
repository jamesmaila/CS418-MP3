#ifndef CAMERA_H
#define CAMERA_H

#include <math.h>
#include "vector.h"

class Camera
{
    public:
        Vector Eye;
        Vector Center;
        Vector Up;

        Vector Translate;
        Vector RotateX;
        Vector RotateY;

        double RotateAngleX;
        double RotateAngleY;

        Camera();
};

#endif
