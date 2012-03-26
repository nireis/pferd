/*
//            _|\\__/|,
//          ,((\\````\\\\_ 
//        ,(())     `))\\
//      ,(()))       ,_ \\
//     ((())'   |        \\
//     )))))     >.__     \\
//     ((('     /    `-. .c|
//hjw          /        `-`'
//http://www.asciiworld.com/-Horses-.html 
//
//	Pferd's lightweigth openGL visualization!
//	Version 0.75 (advanced texture testing)
//	TODO for 0.8: complete texture loading & displaying
//	TODO for 0.9: add labels for nodes/edges
//	TODO for 1.0: optimize version 0.9
//	TODO for 1.1: add mouse support
//	TODO for 2.0: project map onto a sphere
//
//	Requires freeglut, glew, glm and SOIL! Check the respective websites for the sources.
//
//	HOW TO:
//	(Windows)
//	Ask me for the VisualStudio Project files etc...
//	(Linux)
//	Link glew and SOIL Libs.
//	Put the shader files (.glsl) in the directory of your binaries.
//	For testing the latest build: Put textures files in the directory of your binaries.
*/

#ifndef openGLrender_h
#define openGLrender_h

//freeglut and glew
#include <GL/glew.h>
#include <GL/freeglut.h>

//needed for file_read
#include <stdio.h>
#include <stdlib.h>

#include <iostream>

//openGL Math Lib
#include <glm/glm.hpp>
#include <glm/core/type_vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//library for easy texture loading
//chek http://www.lonesock.net/soil.html for more information
#ifdef _WIN32
	#include "soil/include/SOIL.h"
#else
	#include <SOIL.h>
#endif

//stores the vertex data structes
#include "structs.h"

//pragmas seem to be only necessary in windows
#ifdef _WIN32
	#pragma comment(lib,"glew32.lib")
	#pragma comment(lib,"SOIL.lib")
#endif

//Rendering is managed completly by this class
class openGLrender
{
private:
	/*
	/	da fuck i just created...
	/	GLUT doesn't work with non-static member functions
	/	workaround using static functions as a wrappers
	*/
	static openGLrender* currentInstance;
	static void setInstance(openGLrender*);
	static void displayCallback();
	static void keyboardCallback(unsigned char, int, int);
	static void keyboardArrowsCallback(int, int, int);
	static void idleCallback();
	static void resizeCallback(int, int);

	//local reference of the data in the system memory
	unsigned int nodeCount;
	unsigned int edgeCount;
	openGL_Node_3d *nodeArray;
	openGL_Node_3d *edgeArray;

	//stuff needed for openGL
	bool showNodes;
	bool showEdges;
	bool showMap;
	int wWidth;
	int wHeight;
	glm::vec3 cameraPos;

	//transformation matrixes
	glm::mat4 projMX, modelMX, viewMX;

	//openGL buffer objects
	GLuint vbo_map;
	GLuint vbo_nodes;
	GLuint vbo_edges;

	//openGL textures
	GLuint *map_textures;
	int mapCount;

	//shader program
	GLuint program;
	GLuint program2;

	//used to read the shader source code - needs to be replaced
	//till then, thanks to http://en.wikibooks.org/wiki/OpenGL_Programming
	//for the code
	char* file_read(const char*);

	//loads & compiles shader sources
	GLint loadShader(const char*, GLenum);
	//initializes shader programs
	bool initShaderProgram();
	//loads all nodes
	bool initNodes();
	//loads all edge geometry
	bool initEdges();
	//loads map geometry
	bool initMap();
	//loads map textures and actually heavily relies on SOIL to do all the nasty stuff
	bool initTextures();
	//clean up
	void uninit();
	
	//manages keyboard input
	void keyboard(unsigned char, int, int);
	void keyboardArrows(int, int, int);
	//renders geometry on the screen
	void display();
	//idle function, called if GLUT eventqueue is empty
	void idle();
	//handles window resizing
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
	void setCamera(float,float,float);

};

#endif
