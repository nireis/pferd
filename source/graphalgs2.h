#ifndef graphalgs2_h
#define graphalgs2_h

#include "graph.h"
#include <queue>
#include <limits>
#include <list>
using namespace std;

template <typename G>
class graphalgs2{
	private:
		G* g;
		unsigned int nr_of_nodes;
		/*
		 * Einheitliche Arrays für den Dijkstra,
		 * da wir die Daten global haben wollen.
		 */
		vector<bool> found;
		vector<unsigned int> dist;
		// Reset Liste, um zu wissen welche Felder zurück
		// gesetzt werden müssen. Sollte Laufzeit verbessern.
		list<unsigned int> resetlist;

		// struct für die Elemente aus U des Dijkstra.
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
		
	public:
		graphalgs2(G* g);
		~graphalgs2(){};
		/*
		 * Berechnet ein Maximales Independent Set und kontrahiert
		 * alle Knoten von diesem, welche eine negative Edge
		 * Difference haben.
		 */
		void doOneRound();
		// TODO: Nachfolgendes soll nach den Tests alles private werden.
		/*
		 * Berechnet ein Maximal Independent Set (randomisiert).
		 * Wir benutzen dabei einen naiven Markierungsalgorithmus.
		 *
		 * @return Liste der Knoten des Max Ind Sets.
		 */
		list<unsigned int> independent_set();
		/*
		 * Kontrahiert die Knoten aus der Liste und fügt die 
		 * entstandenen Shortcuts in die Struktur ein.
		 *
		 * @nodes Zu kontrahierende Knoten.
		 */
		void contract_nodes(list<unsigned int>* nodes);
		/*
		 * Kontrahiert einen Knoten.
		 *
		 * @conode Der zu kontrahierende Knoten.
		 */
		void contract_node(unsigned int conode);
		/*
		 *	Berechnet für einen Knoten die möglichen Shortcuts, die von
		 *	ihm ausgehen.
		 *	Ob diese später wirklich eingefügt werden entscheidet die
		 *	Edge Difference.
		 *
		 * @scnode Auf Shortcuts zu untersuchender Knoten.
		 * @sclist Liste der Shortcuts.
		 * @conode Der Knoten, welcher kontrahiert wird.
		 */
		void addShortcuts(unsigned int scnode, list<Shortcut>* sclist, unsigned int conode);
		/*
		 * Macht einen kurzen Dijkstra zu targetnode und fügt auch sonst alle 
		 * anfallende Shortcuts ein.
		 *
		 * @targetnode Der zu erreichende Knoten.
		 * @sclist Liste der Shortcuts.
		 */
		void shortDijkstra(unsigned int targetnode, list<Shortcut>* sclist);
		/*
		 * Setzt die benutzten Felder des found und dist Arrays zurück
		 * um ihn für weitere Dijkstras brauchbar zu machen. Sollte die
		 * Laufzeit verbessern.
		 */
		void reset();
};

template <typename G>
graphalgs2<G>::graphalgs2(G* g):
	nr_of_nodes(g->getNodeCount()),
	found(nr_of_nodes,false),
	dist(nr_of_nodes,numeric_limits<unsigned int>::max()){
	this->g = g;
}

template <typename G>
void graphalgs2<G>::doOneRound(){
	contract_nodes(independent_set());
}

template <typename G>
list<unsigned int> graphalgs2<G>::independent_set(){
   list<unsigned int> solution;
   vector<bool> marked(nr_of_nodes,false);
   EdgesIterator it;
   // Random Nummer für den Startknoten
   srand((unsigned)time(0));
   unsigned int r = rand() % nr_of_nodes;

   // Erster Part der Knoten (wegen der Randomisierung)
   for(unsigned int i=r; i<nr_of_nodes; i++){
      // Prüfen ob der Knoten aufgenommen werden kann
      if(!marked[i]){
         solution.push_front(i);
         // Alle ausgehenden Kanten verfolgen
         it = g->getOutEdgesIt(i);
         while(it.hasNext()){
            marked[it.getNext()->other_node] = true;
         }
         // Alle eingehenden Kanten verfolgen
         it = g->getInEdgesIt(i);
         while(it.hasNext()){
            marked[it.getNext()->other_node] = true;
         }
      }
   }
   // Zweiter Part der Knoten
   for(unsigned int i=0; i<r; i++){
      // Prüfen ob der Knoten aufgenommen werden kann
      if(!marked[i]){
         solution.push_front(i);
         // Alle ausgehenden Kanten verfolgen
         it = g->getOutEdgesIt(i);
         while(it.hasNext()){
            marked[it.getNext()->other_node] = true;
         }
         // Alle eingehenden Kanten verfolgen
         it = g->getInEdgesIt(i);
         while(it.hasNext()){
            marked[it.getNext()->other_node] = true;
         }
      }
   }
   return solution;
}

template <typename G>
void graphalgs2<G>::contract_nodes(list<unsigned int>* nodes){
	while(!nodes->empty()){
		contract_node(nodes->front());
		nodes->pop_front();
	}
}

template <typename G>
void graphalgs2<G>::contract_node(unsigned int conode){
	list<Shortcut> sclist;
	EdgesIterator it = g->getInEdgesIt(conode);
	// Die möglichen Shortcuts berechnen und speichern.
	while(it.hasNext()){
		addShortcuts(it.getNext()->other_node, &sclist, conode);
	}
	// Wenn die edgediff negativ ist, wird der Knoten kontrahiert.
	if(sclist.size()-(g->getEdgeCount(conode)) < 0){
		// TODO: Den Knoten aus Graph entfernen und Shortcuts einfügen.
	}
}

template <typename G>
void graphalgs2<G>::addShortcuts(unsigned int scnode, list<Shortcut>* sclist,
		unsigned int conode){
	unsigned int tmpnode;
	EdgesIterator it = g->getOutEdgesIt(conode);
	// Alle zu erreichenden Knoten durchgehen.
	while(it.hasNext()){
		tmpnode = it.getNext()->other_node;
		// Schauen ob wir noch den kürzesten Pfad für den Knoten suchen müssen.
		if(!found[tmpnode]){
			shortDijkstra(tmpnode, sclist);
		}
	}
	// Die Dijkstraarrays für den nächsten Knoten benutzbar machen.
	reset();
}

template <typename G>
void graphalgs2<G>::shortDijkstra(unsigned int targetnode, list<Shortcut>* sclist){
	// TODO
}

template <typename G>
void graphalgs2<G>::reset(){
	unsigned int current;
	while(!resetlist.empty()){
		current = resetlist.front();
		found[current] = false;
		dist[current] = numeric_limits<unsigned int>::max();
		resetlist.pop_front();
	}
}

#endif
