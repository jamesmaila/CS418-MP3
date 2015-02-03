#ifndef VERTEX_H
#define VERTEX_H

class Vertex
{
    public:

	    double X;
	    double Y;
	    double Z;

	    int Index;

        Vertex();
        Vertex(double x, double y, double z, int index);

        Vertex & operator = (Vertex rhs);
};

#endif
