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


sim::sim(Graph* g, travelers* t, conf* c):
	base_g(g),
	sim_g(0),
	trav(t),
	ch(0),
	chd(0),
	cfg(c),
	graphtype(-1)
{
	// Graphtypen anpassen und
	// Kanten ihrem Typ nach gewichten
	initArrays();
	findGraphEdgesTypes();
	setGraphTypesRight();
	recalcEdgevals();
	std::cout << "> Sim init ok" << std::endl;
}
sim::~sim(){
	delete sim_g; sim_g = 0;
	delete ch; ch = 0;
	delete chd; chd = 0;
	trav = 0;
	cfg = 0;
}

void sim::calcOneRound(){
	std::cout << "> Sim starting a round" << std::endl;
	
	sim_g = new SCGraph(base_g);
	ch = new CH(base_g, sim_g);
	ch->calcCH(cfg->chConstVerbose);
	chd = new CHDijkstra(sim_g);
	simTravelers();
	// graph übernimmt EdgeLoads vom simulationsgraph
	sim_g->updateEdgeLoads();
	base_g->updateEdgeLoads();
	sim_g->shareShortcutLoads();
	base_g->getEdgeLoads(sim_g);
	recalcEdgevals();

	// einfärben der kanten im base_g Graph
	paintEdges();

	// vis bescheid sagen, base_g zu malen?
	vis* anzeige = new vis(base_g, &(trav->circles)); anzeige->start();
	delete anzeige; anzeige = 0;

	// aufräumen der graphen und ch ?
	delete sim_g; sim_g = 0;
	delete ch; ch = 0;
	delete chd; chd = 0;

	std::cout << "> Sim finished round" << std::endl;
}

bool sim::eqFound(){
	return false;
}

void sim::recalcEdgevals(){
	// der graph kopiert die temporären
	// edge_loads der dijkstras ins eine EdgeData
	base_g->updateEdgeLoads();
	EdgeData* ed = base_g->getEdgeDataPointer();
	for(unsigned int i = 0; i < base_g->getEdgeCount(); i++){
		double dist = (double) ed[i].distance;
		double weight = weightEdge( ed[i].type, ed[i].load );
		// runden, um das problem zu umgehen, dass für load=0
		// das gewicht nicht nur nom typ, sondern auch vom 
		// parameter a abhängt
		weight = floor( weight + 0.5 );
		// macche aus weight den faktor für die kantenlänge,
		// um die reisezeit zu erhalten
		weight = weight / 100.0;

		double value = dist / weight;

		// gerundeter neuer wert für die kante
		ed[i].value = (unsigned int)( floor(value +0.5));
	}
	// der graph übernimmt die neuen edge_values
	// der EdgeData an den Kanten
	base_g->updateEdgeValues();
}

void sim::simTravelers(){
	// Alle Traveler fahren lassen.
	for(unsigned int i=0; i<trav->traffic.size(); i++){
		// Schauen welche Art weniger Dijkstras benötigt bzw. ob
		// es ein one to one ist.
		pendler* tmp_pendler = &(trav->traffic)[i];
		vector<unsigned int>* src = &tmp_pendler->source;
		vector<unsigned int>* tgt = &tmp_pendler->target;
		unsigned int srcsize = src->size();
		unsigned int tgtsize = tgt->size();
		/*
		if(srcsize > tgtsize){
			for(unsigned int j=0; j<tgtsize; j++){
				chd->manyToOne(src, (*tgt)[j], tmp_pendler->weight);
			}
		}
		else if(srcsize < tgtsize){
			for(unsigned int j=0; j<srcsize; j++){
				chd->oneToMany((*src)[j], tgt, tmp_pendler->weight);
			}
		}
		else if(srcsize == 1){ // Beide sizes sind 1.
			chd->oneToOne((*src)[0], (*tgt)[0], tmp_pendler->weight);
		}
		else{ // Die sizes sind gleich, aber ungleich 1.
			for(unsigned int j=0; j<tgtsize; j++){
				chd->manyToOne(src, (*tgt)[j], tmp_pendler->weight);
			}
		}
		*/
		for(unsigned int i = 0; i< srcsize; i++){
			for(unsigned int j = 0; j < tgtsize; j++){
				chd->oneToOne((*src)[i], (*tgt)[j], tmp_pendler->weight);
			}
		}
		
	}
}

void sim::initArrays(){
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

	// 130
	functionParameter[0][0] = -130.0/40.0;//0.3;
	functionParameter[0][1] = 0.0;//0.0013;
	// 120
	functionParameter[1][0] = -120.0/35.0;//0.3;
	functionParameter[1][1] = 0.0;//0.0013;
	// 100
	functionParameter[2][0] = -100.0/25.0;//0.3;
	functionParameter[2][1] = 0.0;//0.003;
	// 80
	functionParameter[3][0] = -80.0/20.0;//0.5;
	functionParameter[3][1] = 0.0;//0.003;
	// 70
	functionParameter[4][0] = -70.0/15.0;//0.5;
	functionParameter[4][1] = 0.0;//0.0035;
	// 50
	functionParameter[5][0] = -50.0/10.0;//1.0;
	functionParameter[5][1] = 0.0;//0.0065;
	// 45
	functionParameter[6][0] = -45.0/8.0;//1.0;
	functionParameter[6][1] = 0.0;//0.0065;
	//30
	functionParameter[7][0] = -30.0/4.0;//0.7;
	functionParameter[7][1] = 0.0;//0.016;
	// 10
	functionParameter[8][0] = -10.0/1.0;//0.0005;
	functionParameter[8][1] = 0.0;//0.15;
	
}
void sim::findGraphEdgesTypes(){
	EdgeData* ed = base_g->getEdgeDataPointer();
	graphtype = 0;
	for(unsigned int i = 0; i < base_g->getEdgeCount(); i++){
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
void sim::setGraphTypesRight(){
	EdgeData* ed = base_g->getEdgeDataPointer();
	if(graphtype == 1){
		for(unsigned int i = 0; i < base_g->getEdgeCount(); i++){
			ed[i].type = smallTypes[ ed[i].type ];
		}
	}
}
double sim::weightEdge(unsigned int type, unsigned int load){
	double a, b;
	switch(type){
		case 130:
			{
				a = functionParameter[0][0];
				b = functionParameter[0][1];
				break;
			}
		case 120:
			{
				a = functionParameter[1][0];
				b = functionParameter[1][1];
				break;
			}
		case 100:
			{
				a = functionParameter[2][0];
				b = functionParameter[2][1];
				break;
			}
		case 80:
			{
				a = functionParameter[3][0];
				b = functionParameter[3][1];
				break;
			}
		case 70:
			{
				a = functionParameter[4][0];
				b = functionParameter[4][1];
				break;
			}
		case 50:
			{
				a = functionParameter[5][0];
				b = functionParameter[5][1];
				break;
			}
		case 45:
			{
				a = functionParameter[6][0];
				b = functionParameter[6][1];
				break;
			}
		case 30:
			{
				a = functionParameter[7][0];
				b = functionParameter[7][1];
				break;
			}
		case 10:
			{
				a = functionParameter[8][0];
				b = functionParameter[8][1];
				break;
			}
		default:
			{
				std::cout << "Sim: Unknown Edge Type Error, Type " << type << std::endl;
				a = 0;
				b = 0;
				break;
			}
	}

	// werte Gewichtungsfunktion aus
	double v = (double)type;
	double x = (double)load;
	double res =  /* TODO lineare funktion testweise */
		v - ((v * a) / (a + ((v-a)*exp(-b*x))));
		//v - a*x;
		;
	if( res < 0.0 ) 
		res = 0.0;
	if(res > type)
		res = type;

	return res;
}
void sim::paintEdges(){
	EdgeData* ed = base_g->getEdgeDataPointer();
	for(unsigned int i = 0; i < base_g->getEdgeCount(); i++){
		double tmpcolour =  0.0 ;
		
		if(ed[i].load != 0){
			tmpcolour = (double)ed[i].load / (double)cfg->max_travelers;
		}

		//if(ed[i].load == 0.0)
		//	tmpcolour = 0.0;
		//if(ed[i].load != 0.0)
		//	tmpcolour = 1.0;
		if( tmpcolour > 1.0 )
			tmpcolour = 1.0;


		if(ed[i].load != 0 && tmpcolour != 0.0 && tmpcolour != 1.0)
			cout << tmpcolour << endl;

		ed[i].colour = tmpcolour;
	}
}
