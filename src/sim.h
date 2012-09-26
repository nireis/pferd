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
#include <ctime>


class sim{
	private:
		Graph* base_g;
		SCGraph* sim_g;
		travelers* trav;
		CH* ch;
		CHDijkstra* chd;
		Dijkstra* d;
		conf* cfg;

		unsigned int many2one_chdcounter;
		unsigned int one2many_chdcounter;
		unsigned int one2one_chdcounter;
		unsigned int many2one_dcounter;
		unsigned int one2many_dcounter;
		unsigned int one2one_dcounter;

		double many2one_chdtimer;
		double one2many_chdtimer;
		double one2one_chdtimer;
		double many2one_dtimer;
		double one2many_dtimer;
		double one2one_dtimer;

		double normal_rounds_time;
		double ch_rounds_time;

		unsigned int weights_sum;

		vector< vector<unsigned int> > loadhistory;
		vector<double> currentweight;
		vector<double> lastweight;

		int graphtype;
		unsigned int smallTypes[13];
		double functionParameter[9][2];
		std::list<Edge>* shortcuts;
		void initArrays();
		void findGraphEdgesTypes();
		void setGraphTypesRight();

		double weightEdge(unsigned int type, double load);
		void paintEdges();

		void simTravelers();
		void recalcEdgevals();
		void pokeVis();
	public:
		sim(Graph* g, travelers* t, conf* c, std::list<Edge>* scuts);
		~sim();
		void calcOneRoundNormal();
		void calcOneRoundCH();
		void calcOneRoundBoth();
		void resetGraph();
		bool eqFound();
};

#endif

