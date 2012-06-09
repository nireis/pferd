#include "sim.h"


void simplesim::update_weight(){
	//EdgeData* edptr = g.getEdgeDataPointer();
}

void simplesim::next(unsigned int n){
	for(unsigned int i=0; i<n; i++){
		this->next();
	}
}

void simplesim::next(){
	
}


/* === === */


simulation::simulation(Graph* gr) :
	g(gr),
	scg(0),
	graphtype(-1),
	cl(0),
	oscg(0),
	ch(0)
{ 
	initTypes();
}
void simulation::initTypes(){
	smallTypes[0] =  0; // not used
	smallTypes[1] =  130;
	smallTypes[2] =  120;
	smallTypes[3] =  80;
	smallTypes[4] =  70;
	smallTypes[5] =  0; // not used
	smallTypes[6] =  130;
	smallTypes[7] =  50;
	smallTypes[8] =  45;
	smallTypes[9] =  30;
	smallTypes[10] = 50;
	smallTypes[11] = 30;
	smallTypes[12] = 50;
}
simulation::~simulation(){
	g=0;
	scg=0;
	delete cl; cl = 0;
	delete oscg; oscg = 0;
	delete ch; ch = 0;
}

void simulation::findGraphEdgesTypes(){
	EdgeData* ed = g->getEdgeDataPointer();
	graphtype = 0;
	for(unsigned int i = 0; i < g->getEdgeCount(); i++){
		/* Fall VVS/Stuttgart: Typen sind im Bereich 130 ... 10 */
		if( ed[i].type == 5)
		{	std::cout << "Simulation: Found missing Edge Type 5! " << std::endl; }
		if( ed[i].type >= 30)
		{ graphtype = 2; break; } 
		/* Fall 15K/150K/1500K/15000 Graphen: Typen sind 1 ... 12 */
		if( ed[i].type < 10 && ed[i].type != 0)
		{ graphtype = 1; break; }
	}
	ed=0;
}

void simulation::setEdgeValues(EdgeData* ed, unsigned int edge_count){
	if( graphtype == -1 )
		findGraphEdgesTypes();

	if( graphtype == 0 ){
		std::cout << "Simulation: Unbekannte Graphtypen" << std::endl;
	} else {
		for(unsigned int i = 0; i < edge_count; i++){
			double ttype = (double)ed[i].type;
			if(graphtype == 1){
				ttype = (double)smallTypes[(unsigned int)ttype];
			}
			unsigned int newvalue = (unsigned int)std::floor(
					(((double)ed[i].distance) / ((ttype/100.0))) +0.5 //zum runden
					);
			//TODO std::cout << "Value diff: " << newvalue - ed[i].value << std::endl;
			ed[i].value = newvalue;
		}
	}
}

void simulation::setSCGraph(SCGraph* scgr){
	scg = scgr;
}
void simulation::setEdgeColours(EdgeData* ed, unsigned int edge_count){
	double mult = 4.0;
	for(unsigned int i = 0; i < edge_count; i++){
		double ttype = ed[i].type;
			if(graphtype == 1){
				ttype = (double)smallTypes[(unsigned int)ttype];
			}
		double c = ((double)ed[i].load) / (130 /* ttype */ * mult);
		if(c < 0.0)
			c = 0.0;
		if(c > 1.0)
			c = 1.0;
		ed[i].colour = c;
	}
}
void simulation::runVis(bool graph0_scgraph1){
	if( graph0_scgraph1 ){
	} else {
	}
}
/* TODO neue Vis von michi */
//void simulation::launchVisThread(Graph* g, std::list<openGL_Cluster>* clist){
//}
//void simulation::launchVisThread(SCGraph* g, std::list<openGL_Cluster>* clist){
//}
