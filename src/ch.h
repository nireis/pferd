#ifndef ch_h
#define ch_h

#include <queue>
#include <ctime>
#include <cstdlib>
#include <limits>
#include <iostream>

#include "graph.h"
#include "structs.h"
#include "CHConstruction.h"
#include <list>


class CH {
	private:
		CH();

		bool isDone;

		Graph* g;
		SCGraph* scg;

		CHConstruction<SCGraph> algos;

		unsigned int rounds;
		unsigned int max_rounds;

		list<Shortcut>* sclistpointer;
		list<unsigned int>* bnlistpointer;

	public:
		CH(Graph* gr, SCGraph* scgr);
		~CH();

		void calcCH(bool verbose);
		void calcCHverbose();
};






#endif
