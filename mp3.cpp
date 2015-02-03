/* James Maila
 * CS418 - Spring 2015
 * 
 * MP3: Teapot Contest
 */ 

#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include "model.h"
#include "camera.h"
#include "vertex.h"
#include "face.h"

using namespace std;

#define PI 3.14

Model teapot;
Camera cam;

/*
 * Initialize
 *     Inputs: None.
 *     Returns: None.
 *     Description: Initializes the scene and creates the teapot model.
 */
void Initialize()
{
	const char * objPath = "teapot_0.obj";

    teapot = Model(objPath);
    cam = Camera();

    // blue background
    glClearColor(0.5, 0.5, 1.0, 0.0);

    // perspective projection
	glMatrixMode(GL_PROJECTION);
	gluPerspective(50.0, 1.0, 1.0, 50.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// lighting and shadows
	GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lpos[] = { 12.0, 12.0, 12.0, 0.0 };

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	
	// enable z-buffer
	glEnable(GL_DEPTH_TEST);
}

/*
 * GetVertexCoordsAndNormals
 *     Inputs: point - The point (A, B, or C) of the face we are getting values for.
 * 			   currFace - The face object that the point exists on.
 * 			   x, y, z - The vertex coordinates to populate (passed by reference).
 * 			   xn, yn, zn - The vertex normal vector coordinates to populate (passed by reference).
 *     Returns: None.
 *     Description: Given a point and face, this method retrieves vertex coordinates and vertex normal vector coordinates.
 * 					
 */
void GetVertexCoordsAndNormals(
	int point,
	Face currFace,
	float & x, float & y, float & z,
	float & xn, float & yn, float & zn)
{

	switch (point)
	{
		case POINT_A:
			x = currFace.A.X;
			y = currFace.A.Y;
			z = currFace.A.Z;
			xn = teapot.VertexNormals[currFace.A.Index].X;
			yn = teapot.VertexNormals[currFace.A.Index].Y;
			zn = teapot.VertexNormals[currFace.A.Index].Z;
			break;

		case POINT_B:
			x = currFace.B.X;
			y = currFace.B.Y;
			z = currFace.B.Z;
			xn = teapot.VertexNormals[currFace.B.Index].X;
			yn = teapot.VertexNormals[currFace.B.Index].Y;
			zn = teapot.VertexNormals[currFace.B.Index].Z;
			break;

		case POINT_C:
			x = currFace.C.X;
			y = currFace.C.Y;
			z = currFace.C.Z;
			xn = teapot.VertexNormals[currFace.C.Index].X;
			yn = teapot.VertexNormals[currFace.C.Index].Y;
			zn = teapot.VertexNormals[currFace.C.Index].Z;
			break;
	}
} 

/*
 * DrawTeapot
 *     Inputs: None.
 *     Returns: None.
 *     Description: Draws the teapot to the screen.
 */
void DrawTeapot()
{
	if (teapot.Mode == TEXTURE || teapot.Mode == ENVIRONMENT)
	{
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}

	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_TRIANGLES);
		for (unsigned int i=0; i<teapot.Faces.size(); i++)
		{
			Face currFace = teapot.Faces[i];
			float x, y, z;
			float xn, yn, zn;

			// call glVertex on all three points for each face
			for (int point=POINT_A; point<=POINT_C; point++)
			{
				GetVertexCoordsAndNormals(point, currFace, x, y, z, xn, yn, zn);
				glNormal3d(xn, yn, zn);
				glVertex3d(x, y, z);

				if (teapot.Mode == TEXTURE || teapot.Mode == ENVIRONMENT)
				{
					float theta = atan2(z, x);
					glTexCoord2f((theta + PI)/(2*PI), y/2.0);
				}
			}
		}
	glEnd();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
} 

/*
 * Display
 *     Inputs: None.
 *     Returns: None.
 *     Description: The Glut display function. 
 *                  Responsible for drawing to the screen.
 */
void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(cam.Eye.X, cam.Eye.Y, cam.Eye.Z, 
    		  cam.Center.X, cam.Center.Y, cam.Center.Z, 
    		  cam.Up.X, cam.Up.Y, cam.Up.Z);

    // translate in and out
    glTranslated(cam.Translate.X, cam.Translate.Y, cam.Translate.Z);
	
    // rotate left and right
	glRotated(cam.RotateAngleX, cam.RotateX.X, cam.RotateX.Y, cam.RotateX.Z);
	
	// rotate up and down
	glRotated(cam.RotateAngleY, cam.RotateY.X, cam.RotateY.Y, cam.RotateY.Z);

    DrawTeapot();

	glutSwapBuffers();
	glFlush();
	glutPostRedisplay();
}

/*
 * Keyboard
 *     Inputs: key - Ascii key pressed.
 *             x - X mouse position when key pressed.
 *             y - Y mouse position when key pressed.
 *     Outputs: None.
 *     Description: The Glut keyboard function.
 *                  Responsible for interpreting keyboard interrupts.
 */
void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		// rotate backwards about y-axis
		case 'a': cam.RotateAngleY -= 1.0; break;
		// rotate forward about y-axis
		case 'd': cam.RotateAngleY += 1.0; break;
		// rotate left about x-axis
		case 'w': cam.RotateAngleX -= 1.0; break;
		// rotate right about x-axis
		case 's': cam.RotateAngleX += 1.0; break;
		// move forward along z-axis
		case 'e': cam.Translate.Z += 0.25; break;
		// move backwards along z-axis
		case 'q': cam.Translate.Z -= 0.25; break;
		// set no texture mode
		case '1': teapot.Mode = NO_TEXTURE; break;
		// set texture mode, also iterates through availble textures
		case '2': 
			teapot.Mode = TEXTURE;
			teapot.TextureType = (teapot.TextureType + 1) % NUM_TEXTURES;
			switch (teapot.TextureType)
			{
				case CHECKERS: teapot.LoadTexture("textures/checkers.ppm"); break;
				case BRICK: teapot.LoadTexture("textures/brick.ppm"); break;
				case METAL: teapot.LoadTexture("textures/metal.ppm"); break;
				case CERAMIC: teapot.LoadTexture("textures/ceramic.ppm"); break;
				case SKY: teapot.LoadTexture("textures/sky.ppm"); break;
				case LIGHTWOOD: teapot.LoadTexture("textures/lightwood.ppm"); break;
				case DARKWOOD: teapot.LoadTexture("textures/darkwood.ppm"); break;
			}
			break;
		// set environment mode
		case '3': 
			teapot.Mode = ENVIRONMENT;
			teapot.EnvironmentType = (teapot.EnvironmentType + 1) % NUM_TEXTURES;
			teapot.LoadEnvironment("textures/environment.ppm");
			break;
		// exit
		case 27: exit(0);
	}
}

int main(int argc, char** argv)
{
    // initialize glut
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

    // create window
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);
    glutCreateWindow("MP3: Teapot Contest");

    // map display and keyboard glut functions
    glutDisplayFunc(Display);
    glutKeyboardFunc(Keyboard);

    Initialize();

    glutMainLoop();

    return 0;
}