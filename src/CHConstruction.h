#ifndef CHConstruction_h
#define CHConstruction_h

#include "graph.h"
#include <queue>
#include <limits>
#include <list>
#include <map>
#include <thread>
#include <mutex>

#define numThreads std::thread::hardware_concurrency()

using namespace std;

struct DijkstraData;

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

		unsigned int numRounds;
		// Die zu kontrahierenden Knoten
		list<unsigned int>* nodes;
		// Listen um den Graphen nachher umzubauen
		list<Shortcut>* allsclist;
		list<unsigned int>* conodelist;
		// Um sich das Arithmetische Mittel der letzten Runde zu merken.
		int lastArithMean;
		int tmpArithMean;
		// Mutexes für die Parallelisierung.
      mutex mBlacken;
      mutex mGetNext;
      mutex mInsertSC;
		mutex mArithMean;

	public:
		// Strukt für einzelne Daten des Dijkstra.
		struct DijkstraData{
			// Standarddaten
			vector<bool> found;
			priority_queue<U_element, vector<U_element>, Compare_U_element> U;
			// Attribute um mehrere kürzeste Pfade festzustellen.
			unsigned int lastDist;
			map<unsigned int, Shortcut> sameDist;
			// Reset Liste, um zu wissen welche Felder zurück
			// gesetzt werden müssen. Sollte Laufzeit verbessern.
			list<unsigned int> resetlist;
			// Um sich das Arithmetisches Mittel zu merken.
			int tmpArithMean;

			DijkstraData(unsigned int nr_of_nodes):
				found(nr_of_nodes,false){
			}
		};

	private:
		/*
		 * Berechnet ein Maximal Independent Set (randomisiert).
		 * Wir benutzen dabei einen naiven Markierungsalgorithmus.
		 *
		 * @return Pointer auf Liste der Knoten des Max Ind Sets.
		 */
		list<unsigned int>* independent_set();
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
		void addShortcuts(DijkstraData* dd, list<Shortcut>* sclist, unsigned int conode, Edge* firstSCE);
		/*
		 * Macht einen kurzen Dijkstra zu targetnode und fügt auch sonst alle 
		 * anfallende Shortcuts ein.
		 *
		 * @targetnode Der zu erreichende Knoten.
		 * @sclist Liste der Shortcuts.
		 * @firstSCE Pointer auf die (einzig mögliche) erste Shortcut Kante.
		 */
		void shortDijkstra(DijkstraData* dd, unsigned int targetnode, unsigned int conode, list<Shortcut>* sclist, Edge* firstSCE);
		/*
		 * Setzt die benutzten Felder des found und dist Arrays zurück
		 * um ihn für weitere Dijkstras brauchbar zu machen. U wird auch zurückgesetzt.
		 * Sollte die Laufzeit verbessern.
		 */
		void resetDij(DijkstraData* dd);
      /*
       * Schwärzt einen Knoten.
       *
       * @id Die id des zu schwärzenden Knoten.
       */
      void blackenNode(unsigned int id);
      /*
       * Fügt die Shortcuts in die große Liste ein.
       *
       * @sclist Die einzufügenden Shortcuts.
       */
      void insertShortcuts(list<Shortcut>* sclist);

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
		 * @return Gibt true zurück, solange man noch nicht fertig ist.
		 */
		bool calcOneRound(list<Shortcut>* sclist, list<unsigned int>* nodelist);
		/*
		 * Kontrahiert einen Knoten.
		 *
		 * @conode Der zu kontrahierende Knoten.
		 */
		void contract_node(DijkstraData* dd, unsigned int conode);
      /*
       * Holt sich den nächsten zu kontrahierenden Knoten.
		 *
       * @id Der gefragte Knoten.
       * @return True gdw. noch Knoten vorhanden waren.
       */
      bool getNextNode(unsigned int* id);
		/*
		 * Das arithmetische Mittel übergeben.
		 *
		 * @am Den Wert des arith Mittels für einen Thread.
		 */
		void addArithMean(int am);
		unsigned int getNrOfNodes();
};

template <typename G>
unsigned int CHConstruction<G>::getNrOfNodes(){
	return nr_of_nodes;
}

template <typename G>
void run(CHConstruction<G>* chc){
	unsigned int tmpNode;
	typename CHConstruction<G>::DijkstraData dd(chc->getNrOfNodes());
	dd.tmpArithMean = 0;
	while(chc->getNextNode(&tmpNode)){
		chc->contract_node(&dd, tmpNode);
	}
	chc->addArithMean(dd.tmpArithMean);
}

template <typename G>
CHConstruction<G>::CHConstruction(G* g):
	nr_of_nodes(g->getNodeCount()),
	numRounds(0),
	lastArithMean(0){
	this->g = g;
	allsclist = 0;
	conodelist = 0;
	cout << "Berechnung wird mit " << numThreads << " Threads durchgeführt." << endl;
}

template <typename G>
CHConstruction<G>::~CHConstruction(){
	allsclist = 0;
	conodelist = 0;
}

template <typename G>
bool CHConstruction<G>::calcOneRound(list<Shortcut>* sclist, list<unsigned int>* nodelist){
	numRounds++;
   allsclist = sclist;
   conodelist = nodelist;
	tmpArithMean = 0;
	nodes = independent_set();
	int len = nodes->size();
   list<thread> threadlist;
	// Threads erstellen, die auf den jeweiligen Prozessoren laufen sollen.
   for(unsigned int i=0; i<numThreads; i++){
      threadlist.push_back(thread(&run<G>, this));
   }
   for(unsigned int i=0; i<numThreads; i++){
      threadlist.front().join();
      threadlist.pop_front();
   }
   delete nodes;
	cout << len << endl;
	// Das arithmetische Mittel dieser Runde berechnen.
	if(len != 0){
		cout << "Arith Mean: " << tmpArithMean/len << endl;
		lastArithMean = tmpArithMean/len;
		return true;
	}
	return false;
}

template <typename G>
list<unsigned int>* CHConstruction<G>::independent_set(){
   list<unsigned int>* solution = new list<unsigned int>;
	// hole goodNodes beim Graphen ab
	std::priority_queue<uint_pair, std::vector<uint_pair>, compare_uint_pair>* goodNodes = g->getGoodNodes();
	// frühzeitiges abbrechen; sinnvoll ?
	if(goodNodes->empty()){
		return solution;
	}
   vector<bool> marked(nr_of_nodes,false);
   EdgesIterator it;

	while(!goodNodes->empty()){
		uint_pair tmp = goodNodes->top();
		if(!marked[tmp.id]){
			solution->push_front(tmp.id);
			// Alle ausgehenden Kanten verfolgen
			it = g->getOutEdgesIt_Round(tmp.id);
			while(it.hasNext()){
				marked[it.getNext()->other_node] = true;
			}
			// Alle eingehenden Kanten verfolgen
			it = g->getInEdgesIt_Round(tmp.id);
			while(it.hasNext()){
				marked[it.getNext()->other_node] = true;
			}
		}
		goodNodes->pop();
	}
   return solution;
}


template <typename G>
void CHConstruction<G>::contract_node(DijkstraData* dd, unsigned int conode){
	list<Shortcut>* sclist = new list<Shortcut>;
	EdgesIterator it = g->getInEdgesIt_Round(conode);
	Edge* c_edge;
	// Die möglichen Shortcuts berechnen und speichern.
	while(it.hasNext()){
		c_edge = it.getNext();
		addShortcuts(dd, sclist, conode, c_edge);
	}
	// Wenn die edgediff kleiner als das arith Mittel der letzten Runde ist
	// wird der Knoten kontrahiert.
	int tmpEdgeDiff = (int)(sclist->size() - g->getEdgeCount(conode));
	dd->tmpArithMean += tmpEdgeDiff;
	if(tmpEdgeDiff <= lastArithMean){
		blackenNode(conode);
		insertShortcuts(sclist);
	}
	delete sclist;
}

template <typename G>
void CHConstruction<G>::addShortcuts(DijkstraData* dd, list<Shortcut>* sclist,
		unsigned int conode, Edge* firstSCE){
	unsigned int tmpnode;
	unsigned int scnode = firstSCE->other_node;
	// Den ersten Knoten des Dijkstra abarbeiten.
	Edge* currentEdge;
	vector<unsigned int> nextNodes;
	EdgesIterator it = g->getOutEdgesIt_Round(scnode);
	dd->found[scnode] = true;
	dd->resetlist.push_front(scnode);
	while(it.hasNext()){
		currentEdge = it.getNext();
		nextNodes.push_back(currentEdge->other_node);
		dd->U.push(U_element(scnode,currentEdge,currentEdge->value));
	}
	// Den ersten Knoten schon zur Reset Liste hinzufügen. Grund: siehe langes Kommentar
	// in shortDijkstra.
	if(!dd->U.empty()){
		dd->resetlist.push_front(dd->U.top().targetedge->other_node);
	}
	else{
		cout << "Warnung: U ist leer" << endl;
	}
	// Alle zu erreichenden Knoten durchgehen.
	it = g->getOutEdgesIt_Round(conode);
	while(it.hasNext()){
		tmpnode = it.getNext()->other_node;
		// Schauen ob schon ein Shortcut exisitert.
		for(unsigned int i=0; i<nextNodes.size(); i++){
			if(nextNodes[i] == tmpnode){
				goto skip;
			}
		}
		// Schauen ob wir noch den kürzesten Pfad für den Knoten suchen müssen.
		if(!dd->found[tmpnode]){
			shortDijkstra(dd, tmpnode, conode, sclist, firstSCE);
		}
		skip:;
	}
	// Die Dijkstraarrays für den nächsten Knoten benutzbar machen.
	resetDij(dd);
}

template <typename G>
void CHConstruction<G>::shortDijkstra(DijkstraData* dd, unsigned int targetnode,
		unsigned int conode, list<Shortcut>* sclist, Edge* firstSCE){
	unsigned int tmpid;
	EdgesIterator it;
	Edge* currentEdge;
	// Der erste Knoten kann nie einen Shortcut erzeugen und wurde noch nicht
	// gefunden, da es der noch zu bearbeitende Knoten aus der letzten Runde ist,
	// oder einer der ersten Knoten. Außerdem muss er nicht in die Resetliste einge-
	// fügt werden, das dies auch schon gemacht wurde, für den Fall, dass er nichtmehr
	// abgearbeitet werden muss.
	if((tmpid = dd->U.top().targetedge->other_node) != targetnode){
		unsigned int tmpdist = dd->U.top().distance;
		dd->found[tmpid] = true;
		it = g->getOutEdgesIt_Round(tmpid);
		while(it.hasNext()){
			currentEdge = it.getNext();
			// Wenn sie noch nicht gefunden wurde...
			if(!dd->found[currentEdge->other_node]){
				// ...tu sie in U
				dd->U.push(U_element(
							tmpid,currentEdge,currentEdge->value+tmpdist));
			}   
		}
		dd->U.pop();

		// Die restlichen Knoten abarbeiten.
		while((tmpid = dd->U.top().targetedge->other_node) != targetnode){
			if(!dd->found[tmpid]){
				tmpdist = dd->U.top().distance;
				dd->found[tmpid] = true;
				dd->resetlist.push_front(tmpid);
				if(dd->U.top().sourceid == conode){
					sclist->push_front(Shortcut(dd->U.top().targetedge->value+firstSCE->value,
						firstSCE->other_node, tmpid, firstSCE->id, dd->U.top().targetedge->id));
				}
				it = g->getOutEdgesIt_Round(tmpid);
				while(it.hasNext()){
					currentEdge = it.getNext();
					// Wenn sie noch nicht gefunden wurde...
					if(!dd->found[currentEdge->other_node]){
						// ...tu sie in U
						dd->U.push(U_element(
								tmpid,currentEdge,currentEdge->value+tmpdist));
					}   
				}
			}
			dd->U.pop();
		}
		dd->resetlist.push_front(targetnode);
	}
	// Von targetnode noch die Shortcuts legen und in die Reset Liste einfügen,
	// falls es der letzte Knoten war, der zu bearbeiten war in dieser Dijkstra
	// Runde.
	if(dd->U.top().sourceid == conode){
		sclist->push_front(Shortcut(dd->U.top().targetedge->value+firstSCE->value,
			firstSCE->other_node, tmpid, firstSCE->id, dd->U.top().targetedge->id));
	}
}

template <typename G>
void CHConstruction<G>::resetDij(DijkstraData* dd){
	unsigned int current;
	while(!dd->resetlist.empty()){
		current = dd->resetlist.front();
		dd->found[current] = false;
		dd->resetlist.pop_front();
	}
	dd->U = priority_queue<U_element, vector<U_element>, Compare_U_element>();
}

template <typename G>
bool CHConstruction<G>::getNextNode(unsigned int* id){
	unique_lock<mutex> lock(mGetNext);
	if(!nodes->empty()){
		*id = nodes->front();
		nodes->pop_front();
		return true;
	}
	return false;
}

template <typename G>
void CHConstruction<G>::addArithMean(int am){
	unique_lock<mutex> lock(mArithMean);
	tmpArithMean += am;
}

template <typename G>
void CHConstruction<G>::blackenNode(unsigned int id){
	unique_lock<mutex> lock(mBlacken);
	conodelist->push_front(id);
}

template <typename G>
void CHConstruction<G>::insertShortcuts(list<Shortcut>* sclist){
	unique_lock<mutex> lock(mInsertSC);
	allsclist->splice(allsclist->end(), *sclist);
}

#endif
