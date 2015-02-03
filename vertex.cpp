#include "vertex.h"

using namespace std;

Vertex::Vertex()
{
	X = 0.0;
	Y = 0.0;
	Z = 0.0;
	
	Index = 0;
}

Vertex::Vertex(double x, double y, double z, int index)
{
	X = x;
	Y = y;
	Z = z;

	Index = index;
}

Vertex & Vertex::operator = (Vertex rhs)
{
    X = rhs.X;
    Y = rhs.Y;
    Z = rhs.Z;

    Index = rhs.Index;

    return *this;
}
