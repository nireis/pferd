#ifndef visStructs_h
#define visStructs_h

//freeglut and glew
#include <GL/glew.h>
#include <GL/freeglut.h>

//openGL Math Lib
#include <glm/glm.hpp>
#include <glm/core/type_vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//Nodetype 1 for openGL Vis
struct openGL_Node_2d {
	openGL_Node_2d() : lon(0), lat(0) {}
	openGL_Node_2d(float lo, float la) : 
		lon(lo), lat(la) {}
	double lon;
	double lat;
};

//Nodetype 2 for openGL Vis
struct openGL_Node_3d {
	openGL_Node_3d() : lon(0), lat(0), extra(0) {}
	openGL_Node_3d(float lo, float la, float e) : 
		lon(lo), lat(la), extra(e) {}
	float lon;
	float lat;
	float extra;
};

//Nodetype 3 for openGL Vis
struct openGL_Node_3d_uv {
	openGL_Node_3d_uv() : lon(0), lat(0), extra(0) {}
	openGL_Node_3d_uv(float lo, float la, float e, float u, float v) : 
		lon(lo), lat(la), extra(e), u_coord(u), v_coord(v) {}
	float lon;
	float lat;
	float extra;
	float u_coord;
	float v_coord;
};

//Nodetype 4 for openGL Vis
struct openGL_Edge_Node {
	openGL_Edge_Node() : lon(0), lat(0), extra(0), normal_x(0), normal_y(0), normal_z(0) {}
	openGL_Edge_Node(float lo, float la, float e, float nx, float ny, float nz) : 
		lon(lo), lat(la), extra(e), normal_x(nx), normal_y(ny), normal_z(nz) {}
	float lon;
	float lat;
	float extra;
	float normal_x;
	float normal_y;
	float normal_z;
};

//Vertextype for volumerendering
struct vertex {
	vertex() : x(0), y(0), z(0), r(0), g(0), b(0) {}
	vertex(float x_coord, float y_coord, float z_coord, float r_value, float g_value, float b_value) :
	x(x_coord), y(y_coord), z(z_coord), r(r_value), g(g_value), b(b_value) {}
	float x,y,z;
	float r,g,b;
};

//Map-tiles
struct openGL_quad {
	openGL_quad() : vert1(), vert2(), vert3(), vert4() {}
	openGL_Node_3d_uv vert1;
	openGL_Node_3d_uv vert2;
	openGL_Node_3d_uv vert3;
	openGL_Node_3d_uv vert4;
};

//Cluster-Highlighting
struct openGL_Cluster{
	openGL_Cluster() : xCenter(0), yCenter(0), radius(0), color(0) {}
	openGL_Cluster(float x, float y, float r, float c) :
	xCenter(x), yCenter(y), radius(r), color(c) {}
	float xCenter;
	float yCenter;
	float radius;
	float color;
};

//Basic Visualization Entity
struct openGL_Entity{
	openGL_Entity() : vbo_handler(0), geometryType(0), numElements(0), shader_program(0), texture(0),
					world_position(glm::vec3(0.0)), visabilty(false)  {}
	openGL_Entity(GLuint vbo, GLenum type, int num, GLuint shader, GLuint tex, glm::vec3 pos, bool v) :
					vbo_handler(vbo), geometryType(type), numElements(num), shader_program(shader), texture(tex),
					world_position(pos), visabilty(v) {}
	//Geometry
	GLuint vbo_handler;
	GLenum geometryType;
	int numElements;

	GLuint shader_program;
	GLuint texture;
	glm::vec3 world_position;
	bool visabilty;
};


#endif
