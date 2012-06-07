#ifndef sim_h
#define sim_h

#include "graph.h"
#include "chdijkstra.h"

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

		simpletraffic* traffic;
		unsigned int trafficSize;

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

	public:
		simulation(Graph* gr);
		~simulation();

		void setEdgeValues(EdgeData* ed, unsigned int edge_count);
		void setSCGraph(SCGraph* scgr);
		void setEdgeColours(EdgeData* ed, unsigned int edge_count);
};


#endif

