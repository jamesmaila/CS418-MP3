#include "model.h"
#include "vector.h"
#include "face.h"
#include <iostream>
#include <stdio.h>
#include <cstring>

using namespace std;

/*
 * Model
 *     Inputs: None.
 *     Outputs: None.
 *     Description: Default constructor for model, does nothing.
 */
Model::Model() { }

/*
 * Model
 *     Inputs: path - Location of object file (.obj) we want to load.
 *     Outputs: None.
 *     Description: Calls LoadFile on newly declared model.
 */
Model::Model(const char * objPath)
{
	Mode = NO_TEXTURE;

	Lighting = true;

	if (!LoadObject(objPath)) { cout << "Could not load object file."; }
}

/*
 * PopulateFaceNormals
 *     Inputs: None.e
 *     Outputs: None.
 *     Description: Populates face normal list.
 */
void Model::PopulateFaceNormals()
{
	for (unsigned int i=0; i<Faces.size(); i++)
	{
		Face currFace = Faces[i];

		double x1 = currFace.A.X - currFace.C.X;
		double y1 = currFace.A.Y - currFace.C.Y;
		double z1 = currFace.A.Z - currFace.C.Z;

		double x2 = currFace.B.X - currFace.C.X;
		double y2 = currFace.B.Y - currFace.C.Y;
		double z2 = currFace.B.Z - currFace.C.Z;

		Vector a = Vector(x1, y1, z1);
		Vector b = Vector(x2, y2, z2);

		FaceNormals.push_back(a.CrossProduct(b));
	}

}

/*
 * PopulateVertexNormals
 *     Inputs: None.
 *     Outputs: None.
 *     Description: Populates vertex normal list.
 */
void Model::PopulateVertexNormals()
{
	// initialize vertex normals to zero
	for (unsigned int i=0; i<Vertices.size(); i++)
	{
		VertexNormals.push_back(Vector(0.0, 0.0, 0.0));
	}

	// sum each vertex normal on this face
	for (unsigned int i=0; i<Faces.size(); i++)
	{
		VertexNormals[Faces[i].A.Index] += FaceNormals[i];
		VertexNormals[Faces[i].B.Index] += FaceNormals[i];
		VertexNormals[Faces[i].C.Index] += FaceNormals[i];
	}

	// normalize each vertex normal
	for (unsigned int i=0; i<VertexNormals.size(); i++)
	{
		VertexNormals[i].Normalize();
	} 
}


/*
 * LoadObject
 *     Inputs: path - Location of object file we want to load.
 *     Outputs: None.
 *     Description: Populates model with data from path file.
 */
bool Model::LoadObject(const char * path)
{
	FILE * inFile = fopen(path, "r");

	if (inFile == NULL) { return false; }

	int vIndex = 0;

	while (true)
	{
		char lineHeader[128];

		int res = fscanf(inFile, "%s", lineHeader);

		if (res == EOF) { break; }

		// parse vertex
		if (strcmp(lineHeader, "v") == 0)
		{
			double x, y, z;
			fscanf(inFile, "%lf %lf %lf\n", &x, &y, &z);

			Vertex v = Vertex(x, y, z, vIndex);

			Vertices.push_back(v);

			vIndex++;
		}
		// parse face
		else if (strcmp(lineHeader, "f") == 0)
		{
			int indexA, indexB, indexC;
			fscanf(inFile, "%d %d %d", &indexA, &indexB, &indexC);

			// object files are indexed starting at 1, not 0
			indexA -= 1;
			indexB -= 1;
			indexC -= 1;

			Vertex a = Vertices[indexA];
			Vertex b = Vertices[indexB];
			Vertex c = Vertices[indexC];

			Faces.push_back(Face(a, b, c));
		}
	}

	fclose(inFile);

	PopulateFaceNormals();
	PopulateVertexNormals();

	return true;
}

/*
 * ReadPPM
 *     Inputs: path - Location of PPM file we want to read.
 *     Outputs: Pointer to PPM contents.
 *     Description: Parses a PPM file and returns a pointer to the data.
 * 					Method taken from "http://www.cs.rit.edu/usr/local/pub/wrc/courses/cg/textures/ppm/ppm.c"
 */
unsigned char * Model::ReadPPM(const char * path, int * width, int * height)
{
    FILE* fp;
    int i, w, h, d;
    unsigned char* image;
    char head[70];
    
    fp = fopen(path, "rb");
    if (!fp) 
    {
        perror(path);
        return NULL;
    }

    // check magic cookies for valid raw PPM
    fgets(head, 70, fp);
    if (strncmp(head, "P6", 2)) 
    {
        fprintf(stderr, "%s: Not a raw PPM file\n", path);
        return NULL;
    }
    
    // grab the three elements in the header (width, height, maxval)
    i = 0;
    while (i < 3)
    {
        fgets(head, 70, fp);
        // skip comments
        if (head[0] == '#') { continue; }

        if (i == 0) { i += sscanf(head, "%d %d %d", &w, &h, &d); }
        else if (i == 1) { i += sscanf(head, "%d %d", &h, &d); }
        else if (i == 2) { i += sscanf(head, "%d", &d); }
    }
    
    // grab all the image data in one fell swoop.
    image = (unsigned char*)malloc(sizeof(unsigned char)*w*h*3);
    fread(image, sizeof(unsigned char), w*h*3, fp);
    fclose(fp);
    
    *width = w;
    *height = h;
    return image;
}

/*
 * LoadTexture
 *     Inputs: path - Location of texture file we want to load.
 *     Outputs: None.
 *     Description: Populates model with a texture.
 */
bool Model::LoadTexture(const char * path)
{
	int width, height;
	unsigned char * ppm = ReadPPM(path, &width, &height);

	if (ppm == NULL) { return false; }

	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ppm);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    return true;
}

/*
 * LoadEnvironment
 *     Inputs: path - Location of environment file we want to load.
 *     Outputs: None.
 *     Description: Populates model with an environment.
 */
bool Model::LoadEnvironment(const char * path)
{
	int width, height;
	unsigned char * ppm = ReadPPM(path, &width, &height);

	if (ppm == NULL) { return false; }

	glGenTextures(1, &Environment);
	glBindTexture(GL_TEXTURE_2D, Environment);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ppm);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB, GL_UNSIGNED_BYTE, ppm);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

    return true;
}


