#ifndef FACE_H
#define FACE_H

#include "vertex.h"

#define POINT_A 0
#define POINT_B 1
#define POINT_C 2

class Face
{
    public:
    	Vertex A;
    	Vertex B;
    	Vertex C;

        Face();
        Face(Vertex a, Vertex b, Vertex c);

        Face & operator = (Face rhs);
};

#endif
