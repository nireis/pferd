#ifndef sim_h
#define sim_h

#include "graph.h"
#include "chdijkstra.h"
#include "dijkstra.h"
#include "clust.h"
#include "ch.h"
#include "vis.h"
#include "structs.h"
#include <thread>
#include <mutex>


class sim{
	private:
		Graph* base_g;
		SCGraph* sim_g;
		travelers* trav;
		CH* ch;
		CHDijkstra* chd;
		Dijkstra* d;
		conf* cfg;

		double normal_rounds_time;
		double ch_rounds_time;

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
		void calcOneRoundNormal();
		void calcOneRoundCH();
		void calcOneRoundBoth();
		bool eqFound();
};

#endif

