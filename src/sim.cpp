#include "sim.h"


sim::sim(Graph* g, travelers* t, conf* c):
	base_g(g),
	sim_g(0),
	trav(t),
	ch(0),
	chd(0),
	d(0),
	cfg(c),
	graphtype(-1)
{
	// Graphtypen anpassen und
	// Kanten ihrem Typ nach gewichten
	initArrays();
	findGraphEdgesTypes();
	setGraphTypesRight();
	recalcEdgevals();
	normal_rounds_time = 0.0;
	ch_rounds_time = 0.0;
}
sim::~sim(){
	delete sim_g; sim_g = 0;
	delete ch; ch = 0;
	delete chd; chd = 0;
	delete d; d = 0;
	trav = 0;
	cfg = 0;
}
void sim::resetGraph(){
	base_g->updateEdgeLoads();
	EdgeData* ed = base_g->getEdgeDataPointer();
	for(unsigned int i = 0; i < base_g->getEdgeCount(); i++){
		ed[i].load = 0;
	}
	recalcEdgevals();
	base_g->updateEdgeValues();
}
void sim::calcOneRoundNormal(){
	clock_t start, finish;
	double timer;

		start = clock();
	d = new Dijkstra(base_g);
		finish = clock();
		timer = (double(finish)-double(start))/CLOCKS_PER_SEC;

	normal_rounds_time = timer;
	cout << "Dijkstra Round, initialisierung Zeit: " << normal_rounds_time << endl;

	chd = 0;

	simTravelers();

	cout << "Dijkstra Round, Zeit insgesammt: " << normal_rounds_time << endl;
	cout << "> >> "  << normal_rounds_time << endl;

	// graph übernimmt EdgeLoads
	base_g->updateEdgeLoads();

	recalcEdgevals();
	base_g->updateEdgeValues();

	// einfärben der kanten im base_g Graph
	paintEdges();

	// aufräumen der graphen und ch ?
	delete d; d = 0;
}
void sim::calcOneRoundCH(){
	clock_t start, finish;
	double timer = 0.0;

		start = clock();
	sim_g = new SCGraph(base_g);
		finish = clock();
		timer = (double(finish)-double(start))/CLOCKS_PER_SEC;

	ch_rounds_time = timer;
	cout << "CHDijkstra Round, SCGraph init. Zeit: " << ch_rounds_time << endl;

		start = clock();
	ch = new CH(base_g, sim_g);
	ch->calcCH(cfg->chConstVerbose);
		finish = clock();
		timer = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "CHDijkstra Round, CH Konstruktion Zeit: " << timer << endl;
	ch_rounds_time += timer;
	cout << "CHDijkstra Round, SCGraph Shortcuts: " << sim_g->getShortcutCount() << endl;

		start = clock();
	chd = new CHDijkstra(sim_g);
		finish = clock();
		timer = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "CHDijkstra Round, CHDijkstras initialisierung Zeit: " << timer << endl;
	ch_rounds_time += timer;
	d = 0;

	simTravelers();

	// graph übernimmt EdgeLoads
	sim_g->updateEdgeLoads();
		start = clock();
	sim_g->shareShortcutLoads();
	base_g->getEdgeLoads(sim_g);
		finish = clock();
		timer = (double(finish)-double(start))/CLOCKS_PER_SEC;
	cout << "CHDijkstra Round, verteilen der Shortcut Loads Zeit: " << timer << endl;
	ch_rounds_time += timer;
	cout << "CHDijkstra Round, Zeit insgesammt: " << ch_rounds_time << endl;
	cout << "> >> " << ch_rounds_time << endl;

	recalcEdgevals();
	base_g->updateEdgeValues();

	// einfärben der kanten im base_g Graph
	paintEdges();

	// aufräumen der graphen und ch ?
	delete sim_g; sim_g = 0;
	delete ch; ch = 0;
	delete chd; chd = 0;
}
void sim::calcOneRoundBoth(){
/* TODO */
	sim_g = new SCGraph(base_g);
	
	ch = new CH(base_g, sim_g);
	ch->calcCH(cfg->chConstVerbose);
	
	chd = new CHDijkstra(sim_g);
	d = new Dijkstra(base_g);

	simTravelers();

	// graph übernimmt EdgeLoads
	
	// TODO welcher graph der beiden wird angezeigt ?
	//base_g->updateEdgeLoads();
	sim_g->updateEdgeLoads();
	sim_g->shareShortcutLoads();
	base_g->getEdgeLoads(sim_g);

	recalcEdgevals();
	base_g->updateEdgeValues();

	// einfärben der kanten im base_g Graph
	paintEdges();

	// aufräumen der graphen und ch ?
	delete d; d = 0;
	delete sim_g; sim_g = 0;
	delete ch; ch = 0;
	delete chd; chd = 0;
}
bool sim::eqFound(){
	return false;
}
void sim::recalcEdgevals(){
	// der graph kopiert die temporären
	// edge_loads der dijkstras ins eine EdgeData
	EdgeData* ed = base_g->getEdgeDataPointer();
	for(unsigned int i = 0; i < base_g->getEdgeCount(); i++){
		double dist = (double) ed[i].distance;
		double weight = weightEdge( ed[i].type, ed[i].load );
		// runden, um das problem zu umgehen, dass für load=0
		// das gewicht nicht nur vom typ, sondern auch vom 
		// parameter a abhängt
		weight = floor( weight + 1.2 );
		// mache aus weight den faktor für die kantenlänge,
		// um die reisezeit zu erhalten
		weight = weight / 100.0;

		double value = dist / weight;

		// gerundeter neuer wert für die kante
		unsigned int newval = (unsigned int)( floor(value +0.5)) ;
		//cout << "Edge " << i << " old val: " << ed[i].value <<  ", new val: " << newval << ", type: " << ed[i].type << ", load: " << ed[i].load << endl;
		ed[i].value = newval;
	}
	// der graph übernimmt die neuen edge_values
	// der EdgeData an den Kanten
}

void sim::simTravelers(){
	std::cout << "> arbeite Routen ab" << std::endl;
	// Alle Traveler fahren lassen.
	weights_sum = 0;

	clock_t start, finish;
	double timer;

	if(chd != 0){
		many2one_chdcounter = 0;
		one2many_chdcounter = 0;
		one2one_chdcounter = 0;
		many2one_chdtimer = 0.0;
		one2many_chdtimer = 0.0;
		one2one_chdtimer = 0.0;

		for(unsigned int i=0; i<trav->traffic.size(); i++){
			// Schauen welche Art weniger Dijkstras benötigt bzw. ob
			// es ein one to one ist.
			pendler* tmp_pendler = &(trav->traffic)[i];
			vector<unsigned int>* src = &tmp_pendler->source;
			vector<unsigned int>* tgt = &tmp_pendler->target;
			unsigned int srcsize = src->size();
			unsigned int tgtsize = tgt->size();
			if(srcsize > tgtsize){
				for(unsigned int j=0; j<tgtsize; j++){
						start = clock();
					chd->manyToOne(src, (*tgt)[j], tmp_pendler->weight);
						finish = clock();
						timer = (double(finish)-double(start))/CLOCKS_PER_SEC;
					many2one_chdtimer += timer;
					many2one_chdcounter ++;
					weights_sum += tmp_pendler->weight ;//* src->size();
				}
			}
			else if(srcsize < tgtsize){
				for(unsigned int j=0; j<srcsize; j++){
						start = clock();
					chd->oneToMany((*src)[j], tgt, tmp_pendler->weight);
						finish = clock();
						timer = (double(finish)-double(start))/CLOCKS_PER_SEC;
					one2many_chdtimer += timer;
					one2many_chdcounter ++;
					weights_sum += tmp_pendler->weight ;//* tgt->size();
				}
			}
			else if(srcsize == 1){ // Beide sizes sind 1.
				weights_sum += tmp_pendler->weight;
					start = clock();
				chd->oneToOne((*src)[0], (*tgt)[0], tmp_pendler->weight);
					finish = clock();
					timer = (double(finish)-double(start))/CLOCKS_PER_SEC;
				one2one_chdtimer += timer;
				one2one_chdcounter ++;
			}
			else{ // Die sizes sind gleich, aber ungleich 1.
				for(unsigned int j=0; j<tgtsize; j++){
						start = clock();
					chd->manyToOne(src, (*tgt)[j], tmp_pendler->weight);
						finish = clock();
						timer = (double(finish)-double(start))/CLOCKS_PER_SEC;
					weights_sum += tmp_pendler->weight * src->size();
					many2one_chdtimer += timer;
					many2one_chdcounter ++;
				}
			}
		}
		std::cout 
			<< "Many to One CHDijkstras: " << many2one_chdcounter 
			<< ", insg. Zeit: " << many2one_chdtimer 
			<< ", Zeit durchschnittlich: " << many2one_chdtimer/many2one_chdcounter << std::endl;
		std::cout 
			<< "One to Many CHDijkstras: " << one2many_chdcounter 
			<< ", insg. Zeit: " << one2many_chdtimer 
			<< ", Zeit durchschnittlich: " << one2many_chdtimer/one2many_chdcounter << std::endl;
		std::cout 
			<< "One to One CHDijkstras: " << one2one_chdcounter 
			<< ", insg. Zeit: " << one2one_chdtimer << ", Zeit durchschnittlich: " 
			<< one2one_chdtimer/one2one_chdcounter << std::endl;
		ch_rounds_time += one2one_chdtimer + many2one_chdtimer + one2many_chdtimer;
	}
	if(d != 0){
		many2one_dcounter = 0;
		one2many_dcounter = 0;
		one2one_dcounter = 0;
		many2one_dtimer = 0.0;
		one2many_dtimer = 0.0;
		one2one_dtimer = 0.0;

		for(unsigned int i=0; i<trav->traffic.size(); i++){
			// Schauen welche Art weniger Dijkstras benötigt bzw. ob
			// es ein one to one ist.
			pendler* tmp_pendler = &(trav->traffic)[i];
			vector<unsigned int>* src = &tmp_pendler->source;
			vector<unsigned int>* tgt = &tmp_pendler->target;
			unsigned int srcsize = src->size();
			unsigned int tgtsize = tgt->size();
			if(srcsize > tgtsize){
				for(unsigned int j=0; j<tgtsize; j++){
						start = clock();
					d->manyToOne(src, (*tgt)[j], tmp_pendler->weight);
						finish = clock();
						timer = (double(finish)-double(start))/CLOCKS_PER_SEC;
					many2one_dtimer += timer;
					many2one_dcounter ++;
					weights_sum += tmp_pendler->weight * src->size();
				}
			}
			else if(srcsize < tgtsize){
				for(unsigned int j=0; j<srcsize; j++){
						start = clock();
					d->oneToMany((*src)[j], tgt, tmp_pendler->weight);
						finish = clock();
						timer = (double(finish)-double(start))/CLOCKS_PER_SEC;
					one2many_dtimer += timer;
					one2many_dcounter ++;
					weights_sum += tmp_pendler->weight * tgt->size();
				}
			}
			else if(srcsize == 1){ // Beide sizes sind 1.
				weights_sum += tmp_pendler->weight;
					start = clock();
				d->oneToOne((*src)[0], (*tgt)[0], tmp_pendler->weight);
					finish = clock();
					timer = (double(finish)-double(start))/CLOCKS_PER_SEC;
				one2one_dtimer += timer;
				one2one_dcounter ++;
			}
			else{ // Die sizes sind gleich, aber ungleich 1.
				for(unsigned int j=0; j<tgtsize; j++){
						start = clock();
					d->manyToOne(src, (*tgt)[j], tmp_pendler->weight);
						finish = clock();
						timer = (double(finish)-double(start))/CLOCKS_PER_SEC;
					weights_sum += tmp_pendler->weight * src->size();
					many2one_dtimer += timer;
					many2one_dcounter ++;
				}
			}
		}
		std::cout 
			<< "Many to One Dijkstras: " << many2one_dcounter 
			<< ", insg. Zeit: " << many2one_dtimer 
			<< ", Zeit durchschnittlich: " << many2one_dtimer/many2one_dcounter << std::endl;
		std::cout 
			<< "One to Many Dijkstras: " << one2many_dcounter 
			<< ", insg. Zeit: " << one2many_dtimer 
			<< ", Zeit durchschnittlich: " << one2many_dtimer/one2many_dcounter << std::endl;
		std::cout 
			<< "One to One Dijkstras: " << one2one_dcounter 
			<< ", insg. Zeit: " << one2one_dtimer << ", Zeit durchschnittlich: " 
			<< one2one_dtimer/one2one_dcounter << std::endl;
		normal_rounds_time += one2one_dtimer + many2one_dtimer + one2many_dtimer;
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
	functionParameter[0][0] = 0.3;
	functionParameter[0][1] = 0.0013;
	// 120
	functionParameter[1][0] = 0.3;
	functionParameter[1][1] = 0.0013;
	// 100
	functionParameter[2][0] = 0.3;
	functionParameter[2][1] = 0.003;
	// 80
	functionParameter[3][0] = 0.5;
	functionParameter[3][1] = 0.003;
	// 70
	functionParameter[4][0] = 0.5;
	functionParameter[4][1] = 0.0035;
	// 50
	functionParameter[5][0] = 1.0;
	functionParameter[5][1] = 0.0065;
	// 45
	functionParameter[6][0] = 1.0;
	functionParameter[6][1] = 0.0065;
	//30
	functionParameter[7][0] = 0.7;
	functionParameter[7][1] = 0.016;
	// 10
	functionParameter[8][0] = 0.0005;
	functionParameter[8][1] = 0.15;
	
	/*
	// 130
	functionParameter[0][0] = -130.0/2000.0;
	functionParameter[0][1] = 0.0013;
	// 120
	functionParameter[1][0] = -120.0/1800.0;
	functionParameter[1][1] = 0.0013;
	// 100
	functionParameter[2][0] = -100.0/1500.0;
	functionParameter[2][1] = 0.003;
	// 80
	functionParameter[3][0] = -80.0/1000.0;
	functionParameter[3][1] = 0.003;
	// 70
	functionParameter[4][0] = -70.0/800.0;
	functionParameter[4][1] = 0.0035;
	// 50
	functionParameter[5][0] = -50.0/500.0;
	functionParameter[5][1] = 0.0065;
	// 45
	functionParameter[6][0] = -45.0/400.0;
	functionParameter[6][1] = 0.0065;
	//30
	functionParameter[7][0] = -30.0/200.0;
	functionParameter[7][1] = 0.016;
	// 10
	functionParameter[8][0] = -10.0/100.0;
	functionParameter[8][1] = 0.15;
	*/
	
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
		//v + a*x
		;
	if( res < 0.0 ) 
		res = 0.0;

	return res;
}
void sim::paintEdges(){
	EdgeData* ed = base_g->getEdgeDataPointer();
	for(unsigned int i = 0; i < base_g->getEdgeCount(); i++){
		double tmpcolour =  0.0 ;
		
		if(1 || ed[i].load != 0){
			tmpcolour = (double)ed[i].load / (double)weights_sum  ;
			//tmpcolour = log(tmpcolour*(exp(2.0)-1.0) + 1.0) ;
			//tmpcolour =  sqrt( sqrt( sqrt (sqrt( tmpcolour ))));
			tmpcolour =    sqrt (sqrt( tmpcolour ));
			
			// aktuelle mögliche geschwindigkeit durch maximale geschwindigkeit
			tmpcolour = 1.0 - 
				(
				 (100.0 * (double(ed[i].distance) / double(ed[i].value)) ) 
				 / double(ed[i].type)
				);
			
		}

		//tmpcolour = 1.0;

		//if(ed[i].load == 0.0)
		//	tmpcolour = 0.0;
		//if(ed[i].load != 0.0)
		//	tmpcolour = 1.0;
		if( tmpcolour > 1.0 )
			tmpcolour = 1.0;

		ed[i].colour = tmpcolour;
	}
}
