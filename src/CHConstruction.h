#ifndef CHConstruction_h
#define CHConstruction_h

#include "graph.h"
#include <queue>
#include <limits>
#include <list>
#include <map>
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
		// Attribute um mehrere kürzeste Pfade festzustellen.
		unsigned int lastDist;
		map<unsigned int, Shortcut> sameDist;
		// Reset Liste, um zu wissen welche Felder zurück
		// gesetzt werden müssen. Sollte Laufzeit verbessern.
		list<unsigned int> resetlist;
		// Listen um den Graphen nachher umzubauen
		list<Shortcut>* allsclist;
		list<unsigned int>* conodelist;

		/*
		 * Berechnet ein Maximal Independent Set (randomisiert).
		 * Wir benutzen dabei einen naiven Markierungsalgorithmus.
		 *
		 * @return Pointer auf Liste der Knoten des Max Ind Sets.
		 */
		list<unsigned int>* independent_set();
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

	public:
		CHConstruction(G* g);
		~CHConstruction();
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
		void calcOneRound(list<Shortcut>* sclist, list<unsigned int>* nodelist);
};

template <typename G>
CHConstruction<G>::CHConstruction(G* g):
	nr_of_nodes(g->getNodeCount()),
	found(nr_of_nodes,false),
	is_node_black(nr_of_nodes,false),
	dist(nr_of_nodes,numeric_limits<unsigned int>::max()){
	this->g = g;
	allsclist = 0;
	conodelist = 0;
}

template <typename G>
CHConstruction<G>::~CHConstruction(){
	allsclist = 0;
	conodelist = 0;
}

template <typename G>
void CHConstruction<G>::calcOneRound(list<Shortcut>* sclist, list<unsigned int>* nodelist){
	allsclist = sclist;
	conodelist = nodelist;
	list<unsigned int>* nodes = independent_set();
	while(!nodes->empty()){
		contract_node(nodes->front());
		nodes->pop_front();
	}
	delete nodes;
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
	if(sclist.size() < (g->getEdgeCount(conode))){
		conodelist->push_front(conode);
		is_node_black[conode] = true;
		allsclist->splice(allsclist->end(), sclist);
	}
}

template <typename G>
void CHConstruction<G>::addShortcuts(list<Shortcut>* sclist,
		unsigned int conode, Edge* firstSCE){
	unsigned int tmpnode;
	unsigned int scnode = firstSCE->other_node;
	// Den ersten Knoten des Dijkstra abarbeiten.
	Edge* currentEdge;
	EdgesIterator it = g->getOutEdgesIt(scnode);
	lastDist = 0;
	dist[scnode] = 0;
	found[scnode] = true;
	resetlist.push_front(scnode);
	while(it.hasNext()){
		currentEdge = it.getNext();
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
	it = g->getOutEdgesIt(conode);
	while(it.hasNext()){
		tmpnode = it.getNext()->other_node;
		// Schauen ob wir noch den kürzesten Pfad für den Knoten suchen müssen.
		if(!found[tmpnode]){
			shortDijkstra(tmpnode, conode, sclist, firstSCE);
		}
	}
	// Prüfen ob noch ein weiterer Pfad mit gleicher Distanz für die
	// letzten gesichteten Shortcuts mit gleicher Länge existiert.
	// Mögliche Kanten der Länge 0 werden im Moment NICHT berücksichtigt! TODO??
	lastDist = U.top().distance;
	U.pop();
	while(U.top().distance == lastDist && !U.empty()){
		sameDist.erase(U.top().targetedge->other_node);
		U.pop();
	}
	// Die letzten Shortcuts einfügen.
	for(map<unsigned int, Shortcut>::iterator it=sameDist.begin(); it!=sameDist.end(); it++){
		sclist->push_front(it->second);
	}
	sameDist.clear();
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
	if((tmpid = U.top().targetedge->other_node) != targetnode){
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
		lastDist = dist[tmpid];
		U.pop();
		// Die restlichen Knoten abarbeiten.
		while((tmpid = U.top().targetedge->other_node) != targetnode){
			// Prüfen ob noch uneindeutige kürzeste Wege gefunden werden können.
			if(lastDist == U.top().distance){
				// Löschen, wenn einer existiert.
				sameDist.erase(tmpid);
			}
			else{
				for(map<unsigned int, Shortcut>::iterator it=sameDist.begin(); it!=sameDist.end(); it++){
					sclist->push_front(it->second);
				}
				sameDist.clear();
			}
			if(!found[tmpid]){
				dist[tmpid] = U.top().distance;
				found[tmpid] = true;
				resetlist.push_front(tmpid);
				if(U.top().sourceid == conode){
					sameDist.insert(pair<unsigned int, Shortcut>(tmpid,
							Shortcut(U.top().targetedge->value+firstSCE->value,
								firstSCE->other_node, tmpid, firstSCE->id, U.top().targetedge->id)));
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
			lastDist = dist[tmpid];
			U.pop();
		}
		resetlist.push_front(targetnode);
	}
	// Von targetnode noch die Shortcuts legen und in die Reset Liste einfügen,
	// falls es der letzte Knoten war, der zu bearbeiten war in dieser Dijkstra
	// Runde.
	if(U.top().sourceid == conode){
		sameDist.insert(pair<unsigned int, Shortcut>(targetnode,
				Shortcut(U.top().targetedge->value+firstSCE->value, firstSCE->other_node, targetnode,
					firstSCE->id, U.top().targetedge->id)));
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
