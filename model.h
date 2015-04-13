#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <GL/glut.h>
#include "vertex.h"
#include "face.h"
#include "vector.h"

#define NO_TEXTURE 0
#define TEXTURE 1

#define GOLD 0
#define METAL 1
#define WOOD 2

#define NUM_TEXTURES 3


class Model
{
    public:

    	std::vector<Vertex> Vertices;
    	std::vector<Face> Faces;
    	std::vector<Vector> VertexNormals;
    	std::vector<Vector> FaceNormals;
        
    	GLuint Texture;
    	GLuint Environment;

        float YMax;

        int Mode;

        int TextureType;

        bool LightingOn;
        bool TextureOn;
        bool EnvironmentOn;

    	Model();
        Model(const char * objPath);
        bool LoadObject(const char * path);
        unsigned char * ReadPPM(const char * path, int * width, int * height);
        bool LoadTexture(const char * path);
		bool LoadEnvironment(const char * path);

        void PopulateFaceNormals();
        void PopulateVertexNormals();

};

#endif
