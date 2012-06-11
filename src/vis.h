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
		//actual render object
		openGLrender render;
		//openGL representation of graph elements
		openGL_Node_3d* nodes;
		openGL_Edge_Node* edges;
		openGL_Node_3d* shortcut_edges;
		openGL_Cluster* circles;

		struct pipe {
			pipe() : 
				graph(0),
				scgraph(0)
			{}
			pipe(Graph* g) : 
				graph(g),
				scgraph(0)
			{}
			pipe(SCGraph* g) : 
				graph(0),
				scgraph(g)
			{}
			~pipe(){
				graph = 0;
				scgraph = 0;
			}

			Graph* graph;
			SCGraph* scgraph;

			unsigned int getNodeCount(){
				unsigned int ret = 0;
				if(graph){
					ret = graph->getNodeCount();
				}
				if(scgraph){
					ret = scgraph->getNodeCount();
				}
				return ret;
			}

			unsigned int getEdgeCount(){
				unsigned int ret = 0;
				if(graph){
					ret = graph->getEdgeCount();
				}
				if(scgraph){
					ret = scgraph->getEdgeCount();
				}
				return ret;
			}

			unsigned int getShortcutCount(){
				unsigned int ret = 0;
				if(graph){
					ret = 0;
				}
				if(scgraph){
					ret = scgraph->getShortcutCount();
				}
				return ret;
			}

			EdgesIterator getOutEdgesIt(unsigned int id){
				EdgesIterator ret;
				if(graph){
					ret = graph->getOutEdgesIt(id);
				}
				if(scgraph){
					ret = scgraph->getOutEdgesIt(id);
				}
				return ret;
			}

			EdgeData getEdgeData(unsigned int id){
				EdgeData ret;
				if(graph){
					ret = graph->getEdgeData(id);
				}
				if(scgraph){
					ret = scgraph->getEdgeData(id);
				}
				return ret;
			}

			bool isShortcut(unsigned int id){
				bool ret = 0;
				if(graph){
					ret =  false;
				}
				if(scgraph){
					ret =  scgraph->isShortcut(id);
				}
				return ret;
			}

			NodeData* getNodeDataPointer(){
				NodeData* ret = 0;
				if(graph){
					ret = graph->getNodeDataPointer();
				}
				if(scgraph){
					ret = scgraph->getNodeDataPointer();
				}
				return ret;
			}

		};

		pipe p;

		//don't know what this one does
		void init();
		//pass on graph data to openGLrender
		void initRenderer(std::list<openGL_Cluster>* circs);

		float merkatorX(float);
		float merkatorY(float);

	public: 
		//initializes graph data
		void initVis(Graph* g, std::list<openGL_Cluster>* circs);
		void initVis(SCGraph* g, std::list<openGL_Cluster>* circs);
		void initVis(Graph* g);
		void initVis(SCGraph* g);

		vis(Graph* g, std::list<openGL_Cluster>* circs);
		vis(SCGraph* g, std::list<openGL_Cluster>* circs);
		vis(Graph* g);
		vis(SCGraph* g);
		vis();
		~vis();

		bool start(bool*,bool);
};


#endif
