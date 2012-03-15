/*
//	Lightweigth OpenGL visualization for graph nodes and graph edges.
//	Requires freeglut, glew and glm. Check the respective website for the sources.
//	HOW TO: Coming soon...
*/




#ifndef openGLrender_h
#define openGLrender_h

//für file_read bnötigt
#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
#include <GL/glut.h>

#include "glm/glm.hpp"
#include "glm/core/type_vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "structs.h"
#include "parser.h"

#pragma comment(lib,"glew32.lib")

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

	unsigned int nodeCount;
	unsigned int edgeCount;
	NodeData *nodeArray;
	openGL_Edge_Node *edgeArray;

	glm::vec3 cameraPos;
	glm::mat4 projMX, modelMX, viewMX;
	GLuint vbo_nodes;
	GLuint vbo_edges;
	GLuint program;

	//organisierte Methode zum Einlesen von Shadern - wird noch ersetzt
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

public:
	openGLrender();
	~openGLrender();

	bool start(int argc, char* argv[]);

	void setNodeCount(int);
	void setEdgeCount(int);
	void setNodeArray(NodeData*);
	void setEdgeArray(openGL_Edge_Node*);

};

#endif