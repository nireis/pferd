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

	/* 
	 * Iteratoren für Kanten von Nodes
	 */
	public:
		class OE_Andrenator {
			private:
				unsigned int max;
				unsigned int pos;
				E* start;
			public:
				OE_Andrenator(){
					pos = 0;
					max = 0;
					start = 0;
				}

				OE_Andrenator(unsigned int node, Graph<E, N, S> *g){
					start = &(g->edges[g->nodes[node].out_edge_offset]);
					pos = 0;
					max = g->nodes[node+1].out_edge_offset 
						- g->nodes[node].out_edge_offset;
				}

				~OE_Andrenator(){
					start = 0;
				}
				
				bool hasNext(){
					return (pos < max);
				}

				E* getNext(){
			//		if(pos < max){
			//			pos = pos + 1;
					return &start[pos++];
			//		}
			//		return 0;
				}
		};
		class IE_Andrenator {
			private:
				unsigned int max;
				unsigned int pos;
				E** start;
			public:
				IE_Andrenator(){
					pos = 0;
					max = 0;
					start = 0;
				}

				IE_Andrenator(unsigned int node, Graph<E, N, S> *g){
					start = &(g->in_edges[g->nodes[node].in_edge_offset]);
					pos = 0;
					max = g->nodes[node+1].in_edge_offset 
						- g->nodes[node].in_edge_offset;
				}

				~IE_Andrenator(){
					start = 0;
				}
				
				bool hasNext(){
					return (pos < max);
				}

				E* getNext(){
					if(pos < max){
						pos = pos + 1;
					return start[pos-1];
					}
					return 0;
				}
		};


	/*
	 * Eigentliche interne Daten des Graphen
	 */
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
	
	/*
	 * Methoden und zeugs
	 */
	public:

		typedef OE_Andrenator OutEdgesIterator;
		typedef IE_Andrenator InEdgesIterator;

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
		
		virtual ~Graph();

		Edge* getEdge(unsigned int id);

		unsigned int getLowerEdgeBound(unsigned int id);

		unsigned int getUpperEdgeBound(unsigned int id);

		void initOffsets(); // done

		void initShortcutOffsets();

		void clearShortcuts(); // done

		void addShortcut(S& sc);

		unsigned int getNodeCount();
		
		unsigned int getEdgeCount();
		
		OutEdgesIterator getOutEdgesIt(unsigned int node){
			return OutEdgesIterator(node, this);
		}

		InEdgesIterator getInEdgesIt(unsigned int node){
			return InEdgesIterator(node, this);
		}

		/* methoden implementieren, um:
		  * shortuts zu verwalten
		  * gewichte/auslastung der kanten zu verwalten
		  */
/*
		void printGraph(){

			
			Graph::OutEdgesIterator it = getOutEdges(0);

			std::cout << node_count << std::endl;
			std::cout << edge_count << std::endl;
			for(unsigned int i = 0; i < node_count; i++){
				std::cout << i<< " " << nodes[i].lat<< " "  << nodes[i].lon<< " "  << nodes[i].elevation << std::endl;
			}
			for(unsigned int i = 0; i < edge_count; i++){
				std::cout << edges[i].source << " " << edges[i].target << " "  << edges[i].distance << " "  << edges[i].type << std::endl;
			}
		}
		void printGraphIt(){

			
			std::cout << node_count << std::endl;
			std::cout << edge_count << std::endl;
			for(unsigned int i = 0; i < node_count; i++){
				std::cout << i<< " " << nodes[i].lat<< " "  << nodes[i].lon<< " "  << nodes[i].elevation << std::endl;
			}
			E* e;
			for(unsigned int i = 0; i < node_count; i++){
				OutEdgesIterator it = getOutEdges(i);
				while (it.hasNext()){
					e = it.getNext();
					std::cout << e->source << " " << e->target << " "  << e->distance << " "  << e->type << std::endl;
				}
			}
		}
*/
};

#include "graph.hpp"

#endif
