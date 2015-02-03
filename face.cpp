#include "face.h"

using namespace std;

Face::Face() { }

Face::Face(Vertex a, Vertex b, Vertex c)
{
	A = a;
	B = b;
	C = c;
}

Face & Face::operator = (Face rhs)
{
    A = rhs.A;
    B = rhs.B;
    C = rhs.C;

    return *this;
}