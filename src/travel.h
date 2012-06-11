#ifndef travel_h
#define travel_h

#include "tconfreader.h"
#include "structs.h"
#include "clust.h"
#include "graphalgs.h"
#include <vector>
#include <list>

/*
struct pendler {
	vector<unsigned int> source;
	vector<unsigned int> target;
};

struct travelers {
	vector<pendler> traffic;
	list<openGL_Cluster> circles;
};
*/

class travelCenter {
	private:
		travelCenter();

		/* muss vorher deklariert sein */
		Graph* g;
		/* muss vorher deklariert sein */
		travelers* t;

		/* wird angelegt */
		cluster* cl;

		unsigned int rand_range; 
		unsigned int rand_upper_bound;
		
		/* == CONFIG STUFF */

		/*
		 *	mode 1 => all to all 
		 *		=> fill 1 traffic with source/target= all nodes
		 *	mode 2 => random to random 
		 *		=> max_travelers random pairs
		 *	mode 3 => random to all 
		 *		=> random max_travelers each going to all nodes
		 *	mode 4 => use clusters
		 *		=> use clusters of size clust_step
		 *		take clust_count_top_clusters most
		 *		populated clusters and choose 
		 *		lower/upper top_clusters by 
		 *		percentage, if clust_top_percentage > 0.0,
		 *		else by clust_top_upper/clust_top_lower
		 *		from those, take 
		 *		clust_top_{lower,upper}_nodecount_per_cluster
		 *		many nodes per lower/upper cluster
		 *	mode 5 => use manual_targets as targets, for each
		 *           use given radius and count to determine
		 *           sources
		 *
		 */
		int mode;
		//unsigned int max_travelers;
		unsigned int source_count;
		unsigned int target_count;

		unsigned int weight_lower_bound;
		unsigned int weight_upper_bound;
		
		/* cluster options */
		double clust_step;
		unsigned int clust_count_top_clusters;
		double clust_top_percentage;
		unsigned int clust_top_uppers;
		unsigned int clust_top_lowers;
		unsigned int clust_top_upper_nodecount_per_cluster;
		unsigned int clust_top_lower_nodecount_per_cluster;

		/* per hand eingefügte daten beachten */
		std::vector< travelCircle >* manual_targets;
		unsigned int radius;
		std::vector<unsigned int> nodes;
		unsigned int count;


		/* == CONFIG STUFF END */
		vector<travelconf>* tcfg;

		void workConf(conf* c);
		void clearStuff();

		void mode1();
		void mode2();
		void mode3();
		void mode4();
		void mode5();
		
		bool run(unsigned int i);

public:
		travelCenter(Graph* gr, travelers* tr, conf* c);
		~travelCenter();
		
		bool run();
};


#endif

