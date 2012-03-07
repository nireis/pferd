#ifndef CHConstruction_h
#define CHConstruction_h

#include "graph.h"
#include <queue>
#include <limits>
#include <list>
using namespace std;

template <typename G>
class CHConstruction{
	private:
		// struct für die Elemente aus U des Dijkstra.
		struct U_element{
			unsigned int sourceid;
			Edge* targetedge;
			unsigned int distance;

			U_element(unsigned int sid, Edge* e, unsigned int d)
				:sourceid(sid),targetedge(e),distance(d){}

			U_element()
				:sourceid(0),targetedge(0),distance(0){}
		};

		// Vergleich für structs um der kleinsten Distanz die höchste Priorität zu geben.
		class Compare_U_element{
		public:
			bool operator()(U_element& u1, U_element& u2){
				return u1.distance > u2.distance;
			}
		};
		
		G* g;
		unsigned int nr_of_nodes;
		/*
		 * Einheitliche Arrays und Priority Queue für den Dijkstra,
		 * da wir die Daten global haben wollen.
		 */
		vector<bool> found;
		vector<bool> is_node_black;
		vector<unsigned int> dist;
		priority_queue<U_element, vector<U_element>, Compare_U_element> U;
		// Reset Liste, um zu wissen welche Felder zurück
		// gesetzt werden müssen. Sollte Laufzeit verbessern.
		list<unsigned int> resetlist;
		// Listen um den Graphen nachher umzubauen
		list<Shortcut>* allsclist;
		list<unsigned int>* conodelist;

	public:
		CHConstruction(G* g);
		~CHConstruction(){};
		/*
		 * Berechnet ein Maximales Independent Set und kontrahiert
		 * alle Knoten von diesem, welche eine negative Edge
		 * Difference haben.
		 *
		 * @sclist Zu übergebender Pointer, in welchen nachher ein Pointer
		 * auf die zu legenden Shortcuts geschrieben werden.
		 * @nodelist Zu übergebender Pointer, in den ein Pointer auf die zu 
		 * löschenden Knoten geschrieben werden.
		 */
		void calcOneRound(list<Shortcut>** sclist, list<unsigned int>** nodelist);
		// TODO: Nachfolgendes soll nach den Tests alles private werden.
		/*
		 * Berechnet ein Maximal Independent Set (randomisiert).
		 * Wir benutzen dabei einen naiven Markierungsalgorithmus.
		 *
		 * @return Pointer auf Liste der Knoten des Max Ind Sets.
		 */
		list<unsigned int>* independent_set();
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
		 * @sclist Liste der Shortcuts.
		 * @conode Der Knoten, welcher kontrahiert wird.
		 * @firstSCE Pointer auf die (einzig mögliche) erste Shortcut Kante.
		 */
		void addShortcuts(list<Shortcut>* sclist, unsigned int conode, Edge* firstSCE);
		/*
		 * Macht einen kurzen Dijkstra zu targetnode und fügt auch sonst alle 
		 * anfallende Shortcuts ein.
		 *
		 * @targetnode Der zu erreichende Knoten.
		 * @sclist Liste der Shortcuts.
		 * @firstSCE Pointer auf die (einzig mögliche) erste Shortcut Kante.
		 */
		void shortDijkstra(unsigned int targetnode, unsigned int conode, list<Shortcut>* sclist, Edge* firstSCE);
		/*
		 * Setzt die benutzten Felder des found und dist Arrays zurück
		 * um ihn für weitere Dijkstras brauchbar zu machen. U wird auch zurückgesetzt.
		 * Sollte die Laufzeit verbessern.
		 */
		void resetDij();
};

template <typename G>
CHConstruction<G>::CHConstruction(G* g):
	nr_of_nodes(g->getNodeCount()),
	found(nr_of_nodes,false),
	is_node_black(nr_of_nodes,false),
	dist(nr_of_nodes,numeric_limits<unsigned int>::max()){
	this->g = g;
	allsclist = new list<Shortcut>();
	conodelist = new list<unsigned int>();
}

template <typename G>
void CHConstruction<G>::calcOneRound(list<Shortcut>** sclist, list<unsigned int>** nodelist){
	contract_nodes(independent_set());
	*sclist = this->allsclist;
	*nodelist = this->conodelist;
	// Für die nächste Runde zurücksetzen.
	allsclist = new list<Shortcut>();
	conodelist = new list<unsigned int>();
}

template <typename G>
list<unsigned int>* CHConstruction<G>::independent_set(){
   list<unsigned int>* solution = new list<unsigned int>;
   vector<bool> marked(nr_of_nodes,false);
   EdgesIterator it;
   // Random Nummer für den Startknoten
   srand((unsigned)time(0));
   unsigned int r = rand() % nr_of_nodes;

   // Erster Part der Knoten (wegen der Randomisierung)
   for(unsigned int i=r; i<nr_of_nodes; i++){
      // Prüfen ob der Knoten aufgenommen werden kann
      if(!is_node_black[i] && !marked[i]){
         solution->push_front(i);
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
      if(!is_node_black[i] && !marked[i]){
         solution->push_front(i);
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
void CHConstruction<G>::contract_nodes(list<unsigned int>* nodes){
	while(!nodes->empty()){
		// cout << "nodes ist nicht leer" << endl;
		contract_node(nodes->front());
		nodes->pop_front();
	}
}

template <typename G>
void CHConstruction<G>::contract_node(unsigned int conode){
	list<Shortcut> sclist;
	EdgesIterator it = g->getInEdgesIt(conode);
	Edge* c_edge;
	// Die möglichen Shortcuts berechnen und speichern.
	while(it.hasNext()){
		c_edge = it.getNext();
		addShortcuts(&sclist, conode, c_edge);
	}
	// Wenn die edgediff negativ ist, wird der Knoten kontrahiert.
	// cout << "sclist größe: " << sclist.size() << endl;
	if(sclist.size() < (g->getEdgeCount(conode))){
		conodelist->push_front(conode);
		is_node_black[conode] = true;
		/*if(sclist.empty()){
			cout << "sclist ist leer" << endl;
		}
		else{
			cout << "sclist ist nicht leer" << endl;
		}*/
		allsclist->splice(allsclist->begin(), sclist);
		/*if(allsclist->empty()){
			cout << "allsclist ist leer" << endl;
		}
		else{
			cout << "allsclist ist nicht leer" << endl;
		}*/
	}
}

template <typename G>
void CHConstruction<G>::addShortcuts(list<Shortcut>* sclist,
		unsigned int conode, Edge* firstSCE){
	unsigned int tmpnode;
	unsigned int scnode = firstSCE->other_node;
	EdgesIterator it = g->getOutEdgesIt(conode);
	// Den ersten Knoten des Dijkstra abarbeiten.
	Edge* currentEdge;
	EdgesIterator itfirst = g->getOutEdgesIt(scnode);
	dist[scnode] = 0;
	found[scnode] = true;
	resetlist.push_front(scnode);
	while(itfirst.hasNext()){
		currentEdge = itfirst.getNext();
		U.push(U_element(scnode,currentEdge,currentEdge->value));
	}
	// Den ersten Knoten schon zur Reset Liste hinzufügen. Grund: siehe langes Kommentar
	// in shortDijkstra.
	if(!U.empty()){
		resetlist.push_front(U.top().targetedge->other_node);
	}
	else{
		cout << "Warnung: U ist leer" << endl;
	}
	// Alle zu erreichenden Knoten durchgehen.
	while(it.hasNext()){
		tmpnode = it.getNext()->other_node;
		// Schauen ob wir noch den kürzesten Pfad für den Knoten suchen müssen.
		if(!found[tmpnode]){
			shortDijkstra(tmpnode, conode, sclist, firstSCE);
		}
	}
	// Die Dijkstraarrays für den nächsten Knoten benutzbar machen.
	resetDij();
}

template <typename G>
void CHConstruction<G>::shortDijkstra(unsigned int targetnode, unsigned int conode,
		list<Shortcut>* sclist, Edge* firstSCE){
	unsigned int tmpid;
	EdgesIterator it;
	Edge* currentEdge;
	// Der erste Knoten kann nie einen Shortcut erzeugen und wurde noch nicht
	// gefunden, da es der noch zu bearbeitende Knoten aus der letzten Runde ist,
	// oder einer der ersten Knoten. Außerdem muss er nicht in die Resetliste einge-
	// fügt werden, das dies auch schon gemacht wurde, für den Fall, dass er nichtmehr
	// abgearbeitet werden muss.
	tmpid = U.top().targetedge->other_node;
	dist[tmpid] = U.top().distance;
	found[tmpid] = true;
	it = g->getOutEdgesIt(tmpid);
	while(it.hasNext()){
		currentEdge = it.getNext();
		// Wenn sie noch nicht gefunden wurde...
		if(!found[currentEdge->other_node]){
			// ...tu sie in U
			U.push(U_element(
						tmpid,currentEdge,currentEdge->value+dist[tmpid]));
		}   
	}
	U.pop();
	// Die restlichen Knoten abarbeiten.
	while(!U.empty() && (tmpid = U.top().targetedge->other_node) != targetnode){
		if(!found[tmpid]){
			dist[tmpid] = U.top().distance;
			found[tmpid] = true;
			resetlist.push_front(tmpid);
			if(U.top().sourceid == conode){
				sclist->push_front(
						Shortcut(U.top().targetedge->value+firstSCE->value, firstSCE->other_node, tmpid,
							firstSCE->id, U.top().targetedge->id));
			}
			it = g->getOutEdgesIt(tmpid);
			while(it.hasNext()){
				currentEdge = it.getNext();
            // Wenn sie noch nicht gefunden wurde...
            if(!found[currentEdge->other_node]){
               // ...tu sie in U
               U.push(U_element(
							tmpid,currentEdge,currentEdge->value+dist[tmpid]));
            }   
			}
		}
		/*if(!U.empty()){
			cout << "U ist nicht leer" << endl;
		}
		else{
			cout << "U ist leer" << endl;
		}*/
		U.pop();
	}
	// Von targetnode noch die Shortcuts legen und in die Reset Liste einfügen,
	// falls es der letzte Knoten war, der zu bearbeiten war in dieser Dijkstra
	// Runde.
	resetlist.push_front(targetnode);
	if(U.top().sourceid == conode){
		sclist->push_front(
				Shortcut(U.top().targetedge->value+firstSCE->value, firstSCE->other_node, targetnode,
					firstSCE->id, U.top().targetedge->id));
	}
}

template <typename G>
void CHConstruction<G>::resetDij(){
	unsigned int current;
	while(!resetlist.empty()){
		current = resetlist.front();
		found[current] = false;
		dist[current] = numeric_limits<unsigned int>::max();
		resetlist.pop_front();
	}
	U = priority_queue<U_element, vector<U_element>, Compare_U_element>();
}

#endif
