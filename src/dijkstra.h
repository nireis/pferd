#ifndef dijkstra_h
#define dijkstra_h

#include <iostream>
#include "graph.h"
#include <limits>
#include <vector>
using namespace std;

class Dijkstra{
	private:
		//typedef GEdge Edge;
		//typedef GEdgesIterator EdgesIterator;

		 // struct für die Elemente aus U incl. Vergleich
		 struct U_element_bi{
			 unsigned int distance;
			 unsigned int id;
			 unsigned int eid;
			 bool found_by;
		 
			 U_element_bi(unsigned int d, unsigned int i, unsigned int ei, bool f_b)
				 :distance(d),id(i),eid(ei),found_by(f_b){}
		 
			 U_element_bi()
				 :distance(0),id(0),eid(0),found_by(0){}
		 };
		 
		 // Vergleich für structs um der kleinsten Distanz die höchste Priorität zu geben.
		 class Compare_U_element_bi{
			 public:
				 bool operator()(U_element_bi& u1, U_element_bi& u2){
					 return u1.distance > u2.distance;
				 }
		};

		// struct für die Elemente aus U incl. Vergleich
		struct U_element{
			unsigned int distance;
			unsigned int id;
			unsigned int eid;

			U_element(unsigned int d, unsigned int i, unsigned int ei)
				:distance(d),id(i),eid(ei){}

			U_element()
				:distance(0),id(0),eid(0){}
		};

		// Vergleich für structs um der kleinsten Distanz die höchste Priorität zu geben.
		class Compare_U_element{
			public:
				bool operator()(U_element& u1, U_element& u2){
					return u1.distance > u2.distance;
				}
		};

		/*
		 * Setzt den one to one Dijkstra zurück.
		 */
		void resetOne();

		/*
		 * Setzt den one to many Dijkstra zurück.
		 */
		void resetMany();

		/*
		 * Setzt den Neighbours Algorithmus zurück.
		 */
		void resetNeigh(vector<unsigned int>* n_reset_found, unsigned int node_id);

		Graph* g;
		unsigned int nr_of_nodes;
		// Variablen für die Dijkstras...
		// -1, wenn der Knoten noch nicht gefunden wurde, bei found_by.
		vector<unsigned int> dist;
		vector< vector<int> > o_found_by;
		vector<int> m_found_by;
		vector<bool> n_found;
		// ...inkl. Resetlisten.
		vector< vector<unsigned int>> o_reset_found_by;
		vector<unsigned int> m_reset_found_by;

	public:

		Dijkstra(Graph* g);

		/*
		* One to one Dijkstra.
		*/
		unsigned int oneToOne(unsigned int node_id0, unsigned int node_id1, unsigned int weight);

		/*
		 * One to many Dijkstra. Die Distanzen werden in den targets-vector geschrieben
		 * and die entsprechende Stelle.
		 */
		void oneToMany(unsigned int node_id0, vector<unsigned int>* targets, unsigned int weight);

		/*
		 * Many to one Dijkstra, welcher die Distanzen in den sources-vector schreibt.
		 */
		void manyToOne(vector<unsigned int>* sources, unsigned int node_id0, unsigned int weight);

		/*
		 * Sucht alle Nachbarn im Radius "radius" von node_id aus und speichert diese in nbrs.
		 * Der Radius ist dabei die Distanz des Weges auf dem Graph.
		 */
		void getNeighbours(Graph* g, unsigned int node_id, unsigned int radius,
				vector<unsigned int>* nbrs);
};

#endif
