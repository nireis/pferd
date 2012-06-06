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
 
class vis {
	private:
		openGLrender render;
		openGL_Node_3d* nodes;
		openGL_Edge_Node* edges;
		openGL_Node_3d* shortcut_edges;

	public: 
		vis(Graph* g);
		vis(SCGraph* g);
		~vis();

		bool start();
};


#endif
