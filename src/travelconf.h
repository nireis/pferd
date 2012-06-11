#ifndef travelconf_h
#define travelconf_h

#include <vector>


struct travelconf{
	unsigned int mode;
	unsigned int max_travelers;
	unsigned int source_count;
	unsigned int target_count;
	unsigned int weight_lower_bound;
	unsigned int weight_upper_bound;
	std::vector<unsigned int> nodes;
	/* cluster options */
	double clust_step;
	unsigned int clust_count_top_clusters;
	double clust_top_percentage;
	unsigned int clust_top_uppers;
	unsigned int clust_top_lowers;
	unsigned int clust_top_upper_nodecount_per_cluster;
	unsigned int clust_top_lower_nodecount_per_cluster;
};

#endif
