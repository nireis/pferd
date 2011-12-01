#ifndef graph_h
#define graph_h

#include <list>
#include "structs.h"

// welche kanten/knoten der graph nutzt soll variabel sein
// wir wollen ein wenig rumexperimentieren, wie die datenstrukturen
// sich im platzverbrauch verhalten, ob uns also auch ne groessere 
// struktur wehtun wuerde
// E ~ Edges , N  ~ Nodes , S ~ Shortcuts
template <typename E, typename N, typename S> 
class Graph {
	
	private:

		unsigned int node_count;
		unsigned int edge_count;
	
		//voererst alles simpel halten
		// statischer teil des graphen
		//
		N* nodes; // arrays aller kanten und knoten
		E* edges;
	
		E** in_edges; // arrays von pointern auf kanten
		E** out_edges; // hierrin suchen wir mit den offsets nach kanten
		// (int*)[]; // eigentlich sollen das arrays mit pointern drin sein

		// dynamischer teil des graphen, Ss

		std::list<S> shortcuts; //alle shortscuts

		S** in_shortcuts; // array mit pointern
		S** out_shortcuts;// auf shortcuts
		// (int*)[]; // eigentlich sollen das arrays mit pointern drin sein
	public:

		Graph();
		Graph(unsigned int nc, unsigned int ec, // Graph von aussen setzen
				N* n, E* e,
				E** ie, E** oe,
				S** is, S** os);

		Graph(unsigned int nc, unsigned int ec, // Graph klein initialisieren
				N* n, E* e);
				// Uebergeben werden: anzahl der Edges/Nodes; 
				// Pointer auf Array mit Instanzen der Edges/Nodes

				// E** ie, E** oe, // die offsets kann die klasse selber machen.
				// sinnvoller? trennt parsen des graphen vom aufbau.
				// S** is, S** os); // die shortcuts kommen viel spaeter hinzu.

		void initOffsets();

		void initShortcutOffsets();

		void clearShortcuts();

		void addShortcut(S& sc);
		
		void getEdges(unsigned int node_id, E** aedges, int& count);

		void sortEdgesBy(bool sortby);

		/* methoden implementieren, um:
		  * graph zu initialisieren -> offsets setzen
		  * shortuts zu verwalten
		  * gewichte/auslastung der kanten zu verwalten
		  */
};

#endif
