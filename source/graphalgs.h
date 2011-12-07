#ifndef graphalgs_h
#define graphalgs_h

#include "graph.h"

class Graphalgs{
	private:
		Graph<Edge, Node, Shortcut>* g;
		
		// struct für die Elemente aus U incl. Vergleich
		struct U_element{
			unsigned int distance;
			unsigned int id;
			unsigned int eid;

			U_element(unsigned int d, unsigned int i, unsigned int ei){
				distance = d;
				id = i;
				eid = ei;
			}

			U_element(){
				distance = 0;
				id = 0;
				sid = 0;
			}
		};

		// Vergleich für structs um der kleinsten Distanz die höchste Priorität zu geben.
		class Compare_U_element{
			public:
				bool operator()(U_element& u1, U_element& u2){
					if(u1.distance > u2.distance){
						return true;
					}
					return false;
				}
		};

	public:
		/*
		 * Konstruktor!
		 */
		Graphalgs(Graph<Edge, Node, Shortcut>* g);
		/*
		 * node_id ist die ID des Knotens, von dem aus Dijkstra gestartet werden soll.
		 *
		 * Macht einen "normalen" Dijkstra auf dem gegebenen Knoten.
		 */
		void Dijkstra(unsigned int node_id);
};

#endif
