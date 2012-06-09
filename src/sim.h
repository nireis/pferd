#ifndef sim_h
#define sim_h

#include "graph.h"
#include "chdijkstra.h"
#include "dijkstra.h"
#include "clust.h"
#include "ch.h"
#include "vis.h"
#include <thread>
#include <mutex>
#include <iostream>

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

// Andrés Simulation!

template <typename G, typename D>
class sim{
	private:
		Graph* base_g;
		G* sim_g;
		D* dij;
		travelers* trav;
		CH* ch;
		bool simWithCH;
		template <typename S>
		/*
		 * Startet die Simulation "s".
		 */
		static void calcSim(S s);

	public:
		sim(Graph* g, travelers* t, bool simWithCH);
		/*
		 * Die Funktion, mit welcher die Simulation initialisiert wird
		 * und welche dann eine Simulation mit oder ohne CH (je nach 
		 * Parameter "simWithCH") startet. Der Typ der erstellten Simulation
		 * wird dann "calcSim" als Template-Parameter übergeben.
		 */
		static void initSim(Graph* g, travelers* t, bool simWithCH);

};

template <typename G, typename D>
sim<G, D>::sim(Graph* g, travelers* t, bool swch):
	base_g(g),
	trav(t),
	simWithCH(swch){}


template <typename G, typename D>
void sim<G, D>::initSim(Graph* g, travelers* t, bool simWithCH){
   if(simWithCH){
      sim<SCGraph, CHDijkstra>::calcSim< sim<SCGraph, CHDijkstra> >(
            sim<SCGraph, CHDijkstra>(g, t, simWithCH));
   }   
   else{
      sim<Graph, Dijkstra>::calcSim< sim<Graph, Dijkstra> >(
            sim<Graph, Dijkstra>(g, t, simWithCH));
   }   
}

template <typename G, typename D>
template <typename S>
void sim<G, D>::calcSim(S s){ 
	sim_g = new G
}

#endif

