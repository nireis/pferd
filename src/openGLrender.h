/*
//	Lightweigth OpenGL visualization for graph nodes and graph edges.
//	Requires freeglut, glew and glm. Check the respective website for the sources.
//	HOW TO:
//	(Windows)
//	Ask me for the VisualStudio Project file...
//	(Linux)
//	Put freeglut,glew and glm folders in the directory of your sources,
//	linking will probably be required for freeglut and glew.
//	Put the shader files (.glsl) in the directory of your binaries.
*/

#ifndef openGLrender_h
#define openGLrender_h

//needed for file_read
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

//freeglut and glew
#include "glew\include\GL\glew.h"
#include "freeglut\include\GL\freeglut.h"

//openGL Math Lib
#include "glm/glm.hpp"
#include "glm/core/type_vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//stores the vertex data structes
#include "structs.h"

#pragma comment(lib,"glew32.lib")

//Rendering is managed completly by this class
class openGLrender
{
private:
	/*
	/	da fuck i just created...
	/	GLUT doesn't work with non-static member functions
	/	workaround using static functions as a wrapper for the draw function
	*/
	static openGLrender* currentInstance;
	static void setInstance(openGLrender*);
	static void displayCallback();
	static void keyboardCallback(static unsigned char, static int, static int);
	static void keyboardArrowsCallback(static int, static int, static int);
	static void idleCallback();
	static void resizeCallback(static int, static int);

	//local reference of the data in the system memory
	unsigned int nodeCount;
	unsigned int edgeCount;
	openGL_Node_3d *nodeArray;
	openGL_Node_3d *edgeArray;

	//stuff needed for openGL
	bool showNodes;
	bool showEdges;
	int wWidth;
	int wHeight;
	glm::vec3 cameraPos;

	//transformation matrixes
	glm::mat4 projMX, modelMX, viewMX;

	//openGL buffer objects
	GLuint vbo_nodes;
	GLuint vbo_edges;

	//shader program
	GLuint program;

	//used to read the shader source code - needs to be replaced
	char* file_read(const char*);

	GLint loadShader(const char*, GLenum);
	bool initShaderProgram();
	bool initNodes();
	bool initEdges();
	void uninit();

	void keyboard(unsigned char, int, int);
	void keyboardArrows(int, int, int);
	void display();
	void idle();
	void resize(int, int);

public:
	//default Constructor/Destructor
	openGLrender();
	~openGLrender();

	//starts the visualization
	bool start(int argc, char* argv[]);
	//set methods
	void setNodeCount(int);
	void setEdgeCount(int);
	void setNodeArray(openGL_Node_3d*);
	void setEdgeArray(openGL_Node_3d*);
	void setCamera(double,double,double);

};

#endif