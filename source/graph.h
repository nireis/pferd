#ifndef graph_h
#define graph_h

#include <list>

class Graph {

	public:

		struct Simple_Edge {
			unsigned int distance; // negative distanz sinnvoll?
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
			Edge* from;
			bool from_is_sc; // damit wir wissen, ob dies ein shortcut ist,
			Edge* to;			// der weiter aufgemacht gehoert
			bool to_is_sc;
		};

//		class ShortcutList { //shortcuts wollen wir oft erstellen
//			private:				// und spaeter durch neue ersetzen, daher listen
//				std::list<Shortcut> liste;
//			public: // das ganze pipen
//		};
//
//		class EdgeList {
//			private:
//				std::list<unsigned int> liste; // wir nutzen die IDs der kanten
//				// diese spaeter einfach durchlaufen und auslastung aktualisieren
//			public: // das ganze pipen
//		};

		struct Simple_Node {
			//int id; // durch arrayindex gegeben
			unsigned int in_edge_offset; // offset fuer array der eingehenden kanten
			unsigned int out_edge_offset;

		//	std::list<Shortcut>* in_shortcuts; //listen einkommender/ausgehender shortcuts
		//	std::list<Shortcut>* out_shortcuts;
		};
		
		struct Node : public Simple_Node {
			float lat;
			float lon;
			int elevation;
		};
	

	private:

		unsigned int node_count;
		unsigned int edge_count;
	
		//voererst alles simpel halten
		// statischer teil des graphen
		//
		Simple_Node* nodes; // arrays aller kanten und knoten
		Simple_Edge* edges;
	
		Simple_Edge** in_edges; // arrays von pointern auf kanten
		Simple_Edge** out_edges; // hierrin suchen wir mit den offsets nach kanten
		// (int*)[]; // eigentlich sollen das arrays mit pointern drin sein

		// dynamischer teil des graphen, Shortcuts

		std::list<Shortcut> shortcuts; //alle shortscuts

		Shortcut** in_shortcuts; // array mit pointern
		Shortcut** out_shortcuts;// auf shortcuts
		// (int*)[]; // eigentlich sollen das arrays mit pointern drin sein
	public:

		Graph();
		Graph(unsigned int nc, unsigned int ec, // Graph von aussen setzen
				Simple_Node* sn, Simple_Edge* se,
				Simple_Edge** ie, Simple_Edge** oe,
				Shortcut** is, Shortcut** os);

		Graph(unsigned int nc, unsigned int ec, // Graph klein initialisieren
				Simple_Node* sn, Simple_Edge* se);
				// Simple_Edge** ie, Simple_Edge** oe, // die offsets kann die klasse selber machen.
				// sinnvoller? trennt parsen des graphen vom aufbau.
				// Shortcut** is, Shortcut** os); // die shortcuts kommen viel spaeter hinzu.

		/* methoden implementieren, um:
		  * graph zu initialisieren -> offsets setzen
		  * shortuts zu verwalten
		  * gewichte/auslastung der kanten zu verwalten
		  */
};

#endif
