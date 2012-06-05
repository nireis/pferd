#ifndef chdijkstra_h
#define chdijkstra_h

#include <iostream>
#include "graph.h"
#include <limits>
#include <vector>
using namespace std;

class CHDijkstras{
	private:
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
		 * Method to mark ascending edges for the one to many Dijkstra.
		 */
		void markAscEdges(vector<unsigned int>* nodes, vector<unsigned int>* marked);

		/*
		 * Setzt den one to one Dijkstra zurück.
		 */
		void resetOneToOne();

		/*
		 * Setzt den one to many Dijkstra zurück.
		 */
		void resetOneToMany();

		SCGraph* g;
		unsigned int nr_of_nodes;
		// Variablen für die Dijkstras...
		// -1, wenn der Knoten noch nicht gefunden wurde, bei found_by.
		vector<unsigned int> dist;
		vector< vector<int> > o_found_by;
		vector<unsigned int> marked;
		vector<int> m_found_by;
		// ...inkl. Resetlisten.
		vector< vector<unsigned int>> o_reset_found_by;
		vector<unsigned int> m_reset_found_by;
		vector<unsigned int> m_reset_marked;

	public:

		CHDijkstras(SCGraph* g);

		/*
		* One to one Dijkstra auf der CH.
		*/
		unsigned int oneToOne(unsigned int node_id0, unsigned int node_id1);

		/*
		 * One to many Dijkstra auf der CH. Die Distanzen werden in den targets-vector geschrieben
		 * and die entsprechende Stelle.
		 */
		void oneToMany(unsigned int node_id0, vector<unsigned int>* targets);
};

#endif
