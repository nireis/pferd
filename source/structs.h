/* Aus einigen Ueberlegungen heraus stellen wir fest,
 * dass die ganzen Structs nicht wuerdig sind, 
 * in der Naehe anderen Codes zu verweilen
 */

#ifndef structs_h
#define structs_h

struct Simple_Edge {
	unsigned int distance;
	unsigned int source;
	unsigned int target;
};

struct Edge : public Simple_Edge {
	unsigned char type;
	unsigned int id; // damit man kanten im kantenarray finden kann
	// alternativ, statt ID, die differenz des zeigers auf Kante und
	// des zeigers auf anfang des arrays benutzen...
};

struct Shortcut : public Simple_Edge {
	Simple_Edge* from;
	bool from_is_sc; // damit wir wissen, ob dies ein shortcut ist,
	Simple_Edge* to; // der weiter aufgemacht gehoert
	bool to_is_sc;
};

struct Simple_Node {
	//int id; // durch arrayindex gegeben
	unsigned int in_edge_offset; // offset fuer array der eingehenden kanten
	unsigned int out_edge_offset;
};

struct Node : public Simple_Node {
	float lat;
	float lon;
	int elevation;
};

#endif
