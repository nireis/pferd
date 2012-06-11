#ifndef sim_h
#define sim_h

#include "graph.h"
#include "chdijkstra.h"
#include "clust.h"
#include "ch.h"
#include "vis.h"
#include "structs.h"
#include <thread>
#include <mutex>

struct simpletraffic {
	unsigned int src;
	unsigned int dest;
};

class simplesim {
private:
	Graph g;
	simpletraffic* traffic;
	unsigned int traffic_count;
	void update_weight();
	int count ;//= 0;
	
public:
	void next();
	void next(unsigned int n);
	

};


class simulation {
	private:
		simulation();

		Graph* g;
		SCGraph* scg;

		int graphtype;

		cluster* cl;
		SCGraph* oscg;
		CH* ch;

		unsigned int weights_sum;

		/*
		 * das Bestimmen der Typen der Kanten
		 * ist deshalb wichtig, 
		 * weil wir für usnere Graphen
		 * zwei verschiedene Nöglichkeiten haben,
		 * wie die Typen der Kanten
		 * angegeben werden
		 */
		unsigned int smallTypes[13];
		void initTypes();
		void findGraphEdgesTypes();
		/* TODO neue Vis von michi */
//		void launchVisThread(Graph* g, std::list<openGL_Cluster>* clist); 
//		void launchVisThread(SCGraph* g, std::list<openGL_Cluster>* clist);

	public:
		simulation(Graph* gr);
		~simulation();

		void setEdgeValues(EdgeData* ed, unsigned int edge_count);
		void setSCGraph(SCGraph* scgr);
		void setEdgeColours(EdgeData* ed, unsigned int edge_count);
		void runVis(bool graph0_scgraph1);

		void run();
};

class sim{
	private:
		Graph* base_g;
		SCGraph* sim_g;
		travelers* trav;
		CH* ch;
		CHDijkstra* chd;
		conf* cfg;

		unsigned int weights_sum;

		int graphtype;
		unsigned int smallTypes[13];
		double functionParameter[9][2];
		void initArrays();
		void findGraphEdgesTypes();
		void setGraphTypesRight();

		double weightEdge(unsigned int type, unsigned int load);
		void paintEdges();

		void simTravelers();
		void recalcEdgevals();
	public:
		sim(Graph* g, travelers* t, conf* c);
		~sim();
		void calcOneRound();
		bool eqFound();
};

#endif

