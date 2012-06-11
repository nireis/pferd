#include "travel.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <math.h>

travelCenter::travelCenter(Graph* gr, travelers* tr, conf* c) : 
	g(gr), t(tr) 
{
	if(c)
		workConf(c);
	rand_range = weight_upper_bound - weight_lower_bound + 1; 
	rand_upper_bound = RAND_MAX - (RAND_MAX % rand_range);
}
travelCenter::~travelCenter(){
	g = 0;
	t = 0;
	delete cl; cl = 0;
	manual_targets = 0;
}
void travelCenter::workConf(conf* c) 
{
	string str("travel.cfg");
	tconfreader* tcr = new tconfreader(str);
	tcfg = tcr->readConf();
	/*
	mode = c->mode;
	max_travelers = c->max_travelers;
	source_count = c->source_count;
	target_count = c->target_count;
	weight_lower_bound = c->weight_lower_bound;
	weight_upper_bound = c->weight_upper_bound;
	clust_step = c->clust_step;
	clust_count_top_clusters = c->clust_count_top_clusters;
	clust_top_percentage = c->clust_top_percentage;
	clust_top_uppers = c->clust_top_uppers;
	clust_top_lowers = c->clust_top_lowers;
	clust_top_upper_nodecount_per_cluster
	 = c->clust_top_upper_nodecount_per_cluster;
	clust_top_lower_nodecount_per_cluster
	 = c->clust_top_lower_nodecount_per_cluster;
	manual_targets = &(c->manual_targets);
	 */
}
void travelCenter::clearStuff(){
	t->traffic.clear();
	t->circles.clear();
}
void travelCenter::mode1(){
	/* all to all */
	unsigned int s = t->traffic.size();
	t->traffic.resize(1);
	//t->traffic.push_back(pendler());
	t->traffic[s+0].source.resize(g->getNodeCount());
	t->traffic[s+0].target.resize(g->getNodeCount());
	for(unsigned int i = 0; i < g->getNodeCount(); i++){
		t->traffic[s+0].source[i] = i;
		t->traffic[s+0].target[i] = i;
	}

	/* random weights */
	std::srand((unsigned)std::time(0));
	unsigned int r = rand();
	while( r > rand_upper_bound){
		r = rand();
	}
	t->traffic[s+0].weight = weight_lower_bound + (r % rand_range);

}
void travelCenter::mode2(){
	/* random to random , max_travelers pairs */
	unsigned int s = t->traffic.size();
	unsigned int nc = g->getNodeCount();
	std::srand((unsigned)std::time(0));
	t->traffic.resize(count);
	for(unsigned int i = 0; i < count; i++){
		//t->traffic.push_back(pendler());
		t->traffic[s+i].target.resize(1);
		t->traffic[s+i].source.resize(1);
	}
	/* sources */
	for(unsigned int i = 0; i < count; i++){
		unsigned int rand = (std::rand() / RAND_MAX) * nc;
		t->traffic[s+i].source[0] = rand;

		float x = g->getNodeData( t->traffic[s+i].source[0] ).lon;
		float y = g->getNodeData( t->traffic[s+i].source[0] ).lat;
		float r = clust_step/1.0;
		float c = 0.25+(0.0);
		t->circles.push_front( openGL_Cluster(x, y, r, c) );
	}
	/* targets */
	for(unsigned int i = 0; i < count; i++){
		unsigned int rand = (std::rand() / RAND_MAX) * nc;
		t->traffic[s+i].target[0] = rand;

		float x = g->getNodeData( t->traffic[i].source[0] ).lon;
		float y = g->getNodeData( t->traffic[i].source[0] ).lat;
		float r = clust_step/1.0;
		float c = 0.75+(0.0);
		t->circles.push_front( openGL_Cluster(x, y, r, c) );
	}
	/* random weights */
	for(unsigned int i = 0; i < count; i++){
		unsigned int r = rand();
		while( r > rand_upper_bound){
			r = rand();
		}
		t->traffic[s+i].weight = weight_lower_bound + (r % rand_range);
	}

}
void travelCenter::mode3(){
	/* random count to all */
	unsigned int s = t->traffic.size();
	unsigned int nc = g->getNodeCount();
	std::srand((unsigned)std::time(0));
	t->traffic.resize(count);
	for(unsigned int i = 0; i < count; i++){
		//t->traffic.push_back(pendler());
		t->traffic[s+i].target.resize(nc);
		t->traffic[s+i].source.resize(1);
	}
	/* sources */
	for(unsigned int i = 0; i < count; i++){
		unsigned int rand = (std::rand() / RAND_MAX) * nc;
		t->traffic[s+i].source[0] = rand;

		float x = g->getNodeData( t->traffic[s+i].source[0] ).lon;
		float y = g->getNodeData( t->traffic[s+i].source[0] ).lat;
		float r = clust_step/1.0;
		float c = 0.25+(0.0);
		t->circles.push_front( openGL_Cluster(x, y, r, c) );
	}
	/* targets */
	for(unsigned int i = 0; i < count; i++){
		for(unsigned int j = 0; j < nc; j++){
			t->traffic[s+i].target[j] = j;
		}

		/* random weights */
		unsigned int r = rand();
		while( r > rand_upper_bound){
			r = rand();
		}
		t->traffic[s+i].weight = weight_lower_bound + (r % rand_range);
	}
}
void travelCenter::mode4(){
	/*	use clusters of size clust_step
	 *	take clust_count_top_clusters most
	 *	populated clusters and choose 
	 *	lower/upper top_clusters by 
	 *	percentage of clust_count_top_clusters, 
	 *	if clust_top_percentage > 0.0,
	 *	else by clust_top_upper/clust_top_lower
	 *	from those, take 
	 *	clust_top_{lower,upper}_nodecount_per_cluster
	 *	many nodes per lower/upper cluster
	 */
	
	unsigned int s = t->traffic.size();
	cl = new cluster(g, clust_step);

	cl->setMostPopulatedCells( clust_count_top_clusters );

	unsigned int upper;
	unsigned int lower;
	if(clust_top_percentage > 0.0){
		upper = 
			(unsigned int)
			((double)(clust_count_top_clusters) * clust_top_percentage);
		lower = clust_count_top_clusters - upper;

	} else {
		upper = clust_top_uppers;
		lower = clust_top_lowers;
	}

	std::list<unsigned int> starts;
	std::list<unsigned int> targets;

	cl->getNodesUpper(
			clust_top_upper_nodecount_per_cluster,
			upper,
			&targets,
			&(t->circles)
			);
	cl->getNodesLower(
		clust_top_lower_nodecount_per_cluster,
		lower,
		&starts,
		&(t->circles)
		);

	t->traffic.resize( 1 );
	//t->traffic.push_back(pendler());
	t->traffic[s+0].source.reserve(starts.size());
	t->traffic[s+0].target.reserve(targets.size());

	while(! starts.empty()){
		t->traffic[s+0].source.push_back( starts.front() );
		starts.pop_front();

	}
	while(! targets.empty()){
		t->traffic[s+0].target.push_back( targets.front() );
		targets.pop_front();
	}

	/* random weights */
	std::srand((unsigned)std::time(0));
	// TODO Die Scheiße hier umcoden!
	unsigned int r = rand();
	while( r > rand_upper_bound){
		r = rand();
	}
	t->traffic[s+0].weight = 1;//weight_lower_bound + (r % rand_range);

	delete cl; cl = 0;
}
void travelCenter::mode5(){
	/*	use manual_targets as targets, for each
	 * use given radius and count to determine
	 * sources.
	 */
	 
	for(unsigned int i=0; i<manual_targets->size(); i++){
		//unsigned int node_id = (*manual_targets)[i].node_id;
		//unsigned int radius = (*manual_targets)[i].radius;
		//unsigned int count = (*manual_targets)[i].count;
		std::vector<unsigned int> candidates;
		// Finde alle Knoten in dem Radius
		getNeighbours(g, nodes[i], radius, &candidates);
		if(candidates.size() > count){
			// Zufällig <count> Knoten an den Anfang des Vektors setzen und dann
			// den Vektor auf die Größe zuschneiden.
			for(unsigned int i=0; i<count; i++){
				// Eine random number aus dem Bereich [i, #candidates] holen.
				unsigned int r = (rand()/RAND_MAX)*(candidates.size()-i);
				unsigned int dummy = candidates[r];
				candidates[r] = candidates[i];
				candidates[i] = dummy;
			}
			candidates.resize(count);
		}
		// Pendler hinzufügen.
		if(!candidates.empty()){
			t->traffic.push_back(pendler(
						std::vector<unsigned int>(1, nodes[i]),
						candidates,
						1));
		}
	}
}
bool travelCenter::run(){
	clearStuff();
	for(unsigned int i=0;i<tcfg->size();i++){
		mode = (*tcfg)[i].mode;
		count = (*tcfg)[i].count;
		source_count = (*tcfg)[i].source_count;
		target_count = (*tcfg)[i].target_count;
		weight_lower_bound = (*tcfg)[i].weight_lower_bound;
		weight_upper_bound = (*tcfg)[i].weight_upper_bound;
		clust_step = (*tcfg)[i].clust_step;
		clust_count_top_clusters = (*tcfg)[i].clust_count_top_clusters;
		clust_top_percentage = (*tcfg)[i].clust_top_percentage;
		clust_top_uppers = (*tcfg)[i].clust_top_uppers;
		clust_top_lowers = (*tcfg)[i].clust_top_lowers;
		clust_top_upper_nodecount_per_cluster
		 = (*tcfg)[i].clust_top_upper_nodecount_per_cluster;
		clust_top_lower_nodecount_per_cluster
		 = (*tcfg)[i].clust_top_lower_nodecount_per_cluster;
		 nodes = (*tcfg)[i].nodes;
		 radius = (*tcfg)[i].radius;
		 
		//manual_targets = &((*tcfg)[i].manual_targets);
		
		
		run(i);
	}
	return true;
}
bool travelCenter::run(unsigned int i){
	switch(mode)
	{
      case 1:
		{
			mode1();
			break;
		}
		case 2:
		{
			mode2();
			break;
		}
		case 3:
		{
			mode3();
			break;
		}
		case 4:
		{
			mode4();
			break;
		}
		case 5:
		{
			mode5();
			break;
		}
		default:
		{
			std::cout << 
				"TravelCenter: illegal mode in trafficgeneration, one of our assassins will be with you shortly" 
				<< std::endl;
			return false;
			break;
		}
	}
	return true;
}


