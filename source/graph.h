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

	public:
		/* anscheinend brauchen wir, wenn der dijkstra nicht zu spezialisiert
		 * sein soll auf den offset array, klassen, die den zugriff auf 
		 * kanten von knoten kapseln. das soll hier passieren.
		 * die arbeit, die man im dijkstra sowieso machen muessten, 
		 * wird also in getEdge(i) ausgelagert.
		 * hinzu kommt dann der umstand, dass das ganze in klassen 
		 * und funktionen gepackt ist, die selber erstmal speicher brauchen
		 * (lokale variablen, etc) , aber diese würde man sich sehr wahrscheinlich
		 * sowieso irgendwo merken müssen oder stets neu rechnen
		 */
		class AbstractEdgesOfNode{
			public:
				unsigned int node_id;
				unsigned int edge_count;
				virtual bool getEdge(unsigned int edge_id, E& e)=0;
				virtual ~AbstractEdgesOfNode();
		};

		class OutEdgesOfNode : public AbstractEdgesOfNode{
			public:
				OutEdgesOfNode();
				~OutEdgesOfNode();
				OutEdgesOfNode(unsigned int node_id);
				bool getEdge(unsigned int edge_id, E& e);

			private:
				unsigned int nodes_array_base;
		};

		class InEdgesOfNode : public AbstractEdgesOfNode{
			public:
				InEdgesOfNode();
				~InEdgesOfNode();
				InEdgesOfNode(unsigned int node_id);
				bool getEdge(unsigned int edge_id, E& e);

			private:
				unsigned int nodes_array_base;
		};

	private:

		unsigned int node_count;
		unsigned int edge_count;
	
		// statischer teil des graphen
		
		// arrays aller kanten und knoten
		N* nodes; 
		// nodes muss groesse node_count+1 haben! 
		// der letzte ist ein dummy, damit das mit den offsets klappt
		E* edges;
		
		E** in_edges; // arrays von pointern auf kanten
		// hierrin suchen wir mit den offsets nach kanten
		// (int*)[]; // eigentlich sollen das arrays mit pointern drin sein
		// E** out_edges; - ist redundant, siehe implementierung in der .cpp
		// anhand der offsets kann man direkt in E* edges die ausgehenden kanten finden

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
				//  WICHTIG: N* n - die nodes sind genau _nc+1_ nodes,
				//  der letzte ein dummy mit offsets=0
		
		~Graph();

		void initOffsets(); // done

		void initShortcutOffsets();

		void clearShortcuts(); // done

		void addShortcut(S& sc);

		std::list<E*> getAdjOutEdges(unsigned int node_id);

		std::list<E*> getAdjInEdges(unsigned int node_id);

		int getNodeCount();

//		void getAdjOutEdges(unsigned int node_id, 
//					E** array_of_edge_pointers, unsigned int &adj_edge_count);
//
//		void getAdjOutEdges(unsigned int node_id, 
//					E** array_of_edge_pointers, unsigned int &adj_edge_count);

		/* methoden implementieren, um:
		  * graph zu initialisieren -> offsets setzen
		  * shortuts zu verwalten
		  * gewichte/auslastung der kanten zu verwalten
		  */
};

#include "graph.hpp"

#endif
