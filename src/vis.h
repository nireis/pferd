#ifndef vis_h
#define vis_h

//#include <QtGui/QApplication>
//#include <marble/MarbleWidget.h>
//#include <marble/GeoPainter.h>
//#include <iostream>
//#include <sstream>
//#include <list>
#include "graph.h"
#include "CHConstruction.h"
#include "structs.h"
#include "openGLrender.h"
#include "visStructs.h"
 
class vis {
	private:
		//typedef GEdge Edge;
		//typedef GEdgesIterator EdgesIterator;

		int pferd_argc;
		char** pferd_argv;
		//actual render object
		openGLrender render;
		//openGL representation of graph elements
		openGL_Node_3d* nodes;
		openGL_Edge_Node* edges;
		openGL_Node_3d* shortcut_edges;
		openGL_Cluster* circles;
		
		Graph* g;

		//don't know what this one does
		void init();
		//pass on graph data to openGLrender
		void initRenderer(std::list<openGL_Cluster>* circs);

		float merkatorX(float);
		float merkatorY(float);

	public: 
		//initializes graph data
		void initVis(Graph* g, std::list<openGL_Cluster>* circs);
		void initVis(Graph* g);

		vis(int vargc, char** pargv, Graph* gr, std::list<openGL_Cluster>* circs);
		vis(int vargc, char** pargv, Graph* gr);
		vis(int vargc, char** pargv);
		~vis();

		bool start(volatile bool*,bool);
};


#endif
