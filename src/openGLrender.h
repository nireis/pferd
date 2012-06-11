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
//	TODO for 0.85: edge coloring, edge offset along normal vector
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
//	For testing the latest build: Put textures files in the data directory.
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
	#include "SOIL.h"
#else
	#include <SOIL/SOIL.h>
#endif

//stores the vertex data structes
#include "visStructs.h"

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
	static void mouseCallback(int, int);
	static void mouseClickCallback(int button, int state, int x, int y);
	static void keyboardCallback(unsigned char, int, int);
	static void keyboardArrowsCallback(int, int, int);
	static void idleCallback();
	static void resizeCallback(int, int);
	static void timerCallback(int);

	//boolean used to switch between graph and volume rendering
	bool render_mode;
	//transformation matrixes
	glm::mat4 projMX, modelMX, viewMX;
	//window size
	int wWidth;
	int wHeight;

	//used to read the shader source code - needs to be replaced
	//till then, thanks to http://en.wikibooks.org/wiki/OpenGL_Programming
	//for the code
	char* file_read(const char*);
	//loads & compiles shader sources
	GLint loadShader(const char*, GLenum);
	//initializes shader programs
	GLuint initShaderProgram(const char* , const char*, const char**, int);

	//renders geometry on the screen
	void display();
	//idle function, called if GLUT eventqueue is empty
	void idle();
	//handles window resizing
	void resize(int, int);
	//timer function for camera rotation
	void timer();

	//manages mouse input
	void mouse(int, int);
	void mouseClick(int, int, int, int);
	//manages keyboard input
	void keyboard(unsigned char, int, int);
	void keyboardArrows(int, int, int);


	/*
	*	private variables and functions used for volume-rendering
	*/

	//camera position
	float cam_alpha, cam_beta, cam_dist;
	//used for camera test during rendering
	bool cam_BBtest;
	// bounding box dimensions
	glm::vec3 dimension;
	// transformation matrixes
	glm::mat4 texMX;
	// buffer object
	GLuint vbo_boundingBox;
	// 3d texture
	GLuint tex_3D;
	//shader programs
	GLuint frontface_prgm;
	GLuint backface_prgm;
	//initializes bounding box geometry
	bool initBoundingBox(glm::vec3);
	//initializes a 3D volumetexture
	bool init3DTex(glm::vec3, char*);
	bool init3Dto2DTex(glm::vec3);
	//initializes volume rendering
	bool initVolumeVis();
	//displays volume - this function gets called in the display() function when render mode is set to volume
	void displayVolume();
	//clean up
	void uninitVolume();


	/*
	*	private variables and functios used for graph-rendering
	*/

	//local reference of the data in the system memory
	unsigned int nodeCount;
	unsigned int edgeCount;
	unsigned int shortcutCount;
	unsigned int clusterCount;
	openGL_Node_3d *nodeArray;
	openGL_Edge_Node *edgeArray;
	openGL_Node_3d *shortcutArray;
	openGL_Cluster *clusterArray;

	//stuff needed for openGL
	glm::vec3 cameraPos;
	float camZoom;
	int mouse_delta_x0;
	int mouse_delta_x1;
	int mouse_delta_y0;
	int mouse_delta_y1;
	bool swap;
	bool mouse_mode;

	//Entities
	openGL_Entity *sceneEntities;
	int entityCount;

	//openGL textures
	GLuint *map_textures;
	int mapCount;

	//loads node/shortcut type geometry
	bool initOpenGL_Node_3d(GLuint*, openGL_Node_3d*, int);
	//loads edge type geometry
	bool initOpenGL_Edge_Node(GLuint*, openGL_Edge_Node* , int);
	//loads cluster type geometry
	bool initOpenGL_Cluster(GLuint*, openGL_Cluster);
	//initializes all scene entities used for graph rendering
	bool initGraphVis();
	//displays graph - this function gets called in the display() function when render mode is set to graph
	void displayGraph();

	/*
	//loads map geometry
	bool initMap();
	//loads map textures and actually heavily relies on SOIL to do all the nasty stuff
	bool initTextures();
	//draw node label
	bool drawText(glm::mat4);
	*/

	//clean up
	void uninitGraph();
	

public:
	//default Constructor/Destructor
	openGLrender();
	~openGLrender();

	//starts the visualization
	bool start(int argc, char* argv[]);
	//set methods
	void setNodeCount(int);
	void setEdgeCount(int);
	void setClusterCount(int);
	void setShortcutEdgeCount(int);
	void setNodeArray(openGL_Node_3d*);
	void setEdgeArray(openGL_Edge_Node*);
	void setShortcutEdgeArray(openGL_Node_3d*);
	void setClusterArray(openGL_Cluster*);
	void setCamera(float,float,float);

};

#endif
