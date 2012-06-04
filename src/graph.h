#ifndef graph_h
#define graph_h

#include <iostream>
#include "structs.h"
#include "parser.h"
#include <vector>
#include <list>
#include <queue>
#include <algorithm>

typedef unsigned int N;
//typedef Node N2;
typedef NodeData ND;

typedef Edge E;
typedef EdgeData ED;
//typedef EdgeData2 ED2;

typedef Shortcut S;
typedef ShortcutData SD;

typedef Andrenator_P<E> EdgesIterator;
typedef Andrenator_P<LVLEdge> LVLEdgesIterator;

class SCGraph;

class Dijkstra_Interface {
	public:
		typedef Andrenator_P<E> EdgesIterator;

		virtual unsigned int getNodeCount() = 0;
		virtual unsigned int getEdgeCount() = 0;

		virtual EdgesIterator getOutEdgesIt(unsigned int id) = 0;
		virtual EdgesIterator getInEdgesIt(unsigned int id) = 0;

		virtual Edge* getOutEdge(unsigned int id) = 0;
		virtual Edge* getInEdge(unsigned int id) = 0;
};


/*
 * statischer Graph, der nur Kanten und Knoten
 * und deren Daten enthält
 */
class Graph {
	public:
		//typedef Andrenator_P<E> EdgesIterator;
	/*
	 * Zu jeder Struktur merken wir uns umfangreiche Daten in
	 *		*_data
	 *	Hierdurch sollen Zugriffe auf die anderen Informationen
	 *	schnell und kompakt werden.
	 */
	private:
		/*
		 * statischer Teil des Graphen
		 */
		bool is_set;
		static const int BinID;
		static const std::string dateiendung;

		unsigned int node_count;
		unsigned int edge_count;

		N* nodes_in_offs;
		E* in_edges; 
		N* nodes_out_offs;
		E* out_edges;
		/* 
		 * nodes_{in,out} arrays müssen groesse node_count+1 haben
		 * der letzte ist ein dummy, damit das mit den offsets klappt
		 */

		ND* node_data;
		ED* edge_data;

		/*
		 * private Methoden
		 */
		int checkDataFile(std::string graphdata);
		/*
		 * prüft übergebene Datei auf Verträglichkeit mit Graph
		 * Rückgaben:
		 *		-1 -> veraltete .grp Binärdatei 
		 *		 0 -> Datei nicht existent oder Dateityp unbekannt
		 *		 1 -> voraussichtlich verträgliche .grp Binärdatei
		 *		 2 -> voraussichtlich parsbare .txt Datei
		 */

		void writeBinaryGraphFile(std::string graphdata);
		bool readBinaryGraphFile(std::string graphdata);
		bool parseTextGraphFile(std::string graphdata);

	/*
	 * public Methoden und zeugs
	 */
	public:
		Graph();
		~Graph();

		bool isSet(){ return is_set; }
		bool setGraph(std::string graphdata, bool write_binary);
		/*
		 * graphdata ist ein String, der den Namen einer
		 * gültigen Graphdatei enthält; entweder eine rohe .txt Datei
		 * in parsbarem Format, oder eine .grp Binärdatei,
		 * die von uns geschrieben wurde.
		 *
		 * Die übergebene Datei wird eingelesen und der Graph entsprechend
		 * initialisiert. 
		 */
		
		/*
		 * Daten zu Strukturen abfragen
		 */
		unsigned int getNodeCount();
		unsigned int getEdgeCount();
		unsigned int getEdgeCount(unsigned int node_id);

		/*
		 * get*Data - jeweils mit indexprüfung,
		 * ob die angegebene ID gültig ist.
		 * Ist diese nicht gültig, wird ein 
		 * Default-Initialisiertes Objekt zurück gegeben
		 */
		ND getNodeData(unsigned int id);
		ED getEdgeData(unsigned int id);

		ND* getNodeDataPointer(){ return node_data; }

		/*
		 * ohne Indexprüfung !
		 */
		unsigned int getLowerOutEdgeBound(unsigned int id);
		unsigned int getUpperOutEdgeBound(unsigned int id); 
		unsigned int getLowerInEdgeBound(unsigned int id);
		unsigned int getUpperInEdgeBound(unsigned int id); 
		
		E* getOutEdge(unsigned int edge_id);
		E* getInEdge(unsigned int edge_id);
		
		E* copyOutEdge(unsigned int edge_id);
		E* copyInEdge(unsigned int edge_id);

		/*
		 * zum Updaten von Kantenbelastungen
		 * übernimmt Belastungen eines SCGraph
		 * ignoriert die Belastungen der Shortcuts vom 
		 * SCGraph, diese muss er vorher selbst
		 * verteilt haben.
		 */
		void getEdgeLoads(SCGraph* g);

		/*
		 * hier werden Iteratoren über Kanten nach Aussen gegeben
		 * 
		 * ein Iterator funktioniert 
		 *		* nur ein mal, wenn man damit alle Kanten anschaut
		 *		* für nur einen Knoten
		 *		* für genau alle Kanten der angeforderten Richtung 
		 */
		EdgesIterator getOutEdgesIt(unsigned int node){
			unsigned int nofs = nodes_out_offs[node] ;
			unsigned int c = nodes_out_offs[node+1] - nofs ;
			return EdgesIterator(out_edges + nofs , c ); 
		}
		EdgesIterator getInEdgesIt(unsigned int node){
			unsigned int nifs = nodes_in_offs[node] ;
			unsigned int c = nodes_in_offs[node+1] - nifs ;
			return EdgesIterator(in_edges + nifs , c ); 
		}
		/*
		 * out0_in1 == false == 0 => gebe OutEdges-Iterator
		 * out0_in1 == true == 1 => gebe InEdges-Iterator
		 */
		EdgesIterator getEdgesIt(bool out0_in1, unsigned int node_id){
			unsigned int nfs;
			unsigned int c;
			if( out0_in1 ){
				nfs = nodes_in_offs[node_id] ;
				c = nodes_in_offs[node_id+1] - nfs ;
				return EdgesIterator(in_edges + nfs , c ); 
			} else {
				nfs = nodes_out_offs[node_id] ;
				c = nodes_out_offs[node_id+1] - nfs ;
				return EdgesIterator(out_edges + nfs , c ); 
			}
		}

};

//	class SCGraph {
//		private:
//			Graph* g;
//	
//			// runde, in der ein knoten kontrahiert wurde
//			unsigned int* node_lvl;
//			
//			unsigned int node_count;
//	
//			// der edge_count ist die anzahl der edges,
//			// die Graph* g hat
//			unsigned int edge_count;
//	
//			// der shortcut count ist die anzahl aller shortcuts,
//			// die der graph bisher bekommen hat
//			unsigned int shortcut_count;
//	
//			//wird gebraucht beim einfügen von shortcuts
//			unsigned int current_edge_arrays_size;
//	
//			N* nodes_in_offs;
//			E* in_edges; 
//			N* nodes_out_offs;
//			E* out_edges;
//	
//			ND* node_data;
//			ED* edge_data;
//		
//			// zum merken der übergebenen shortcuts während der CH-runden
//			struct ShortcutArray {
//				ShortcutArray() : array(0), size(0) {}
//				ShortcutArray(Shortcut* a, unsigned int s) :
//					array(a), size(s) {}
//				~ShortcutArray(){ array = 0; }
//				Shortcut* array;
//				unsigned int size;
//			};
//			SList<ShortcutArray> shortcutlist;
//	
//			std::list<Shortcut>* round_shortcutlist;
//			std::list<unsigned int> round_node_blacklist;
//	
//			// für temporäre daten während der CH-runden
//			// evtl ist dies redundant
//			unsigned int* node_in_edges_count;
//			unsigned int* node_out_edges_count;
//	
//			// während der CH-runden soll gemerkt werden, welche knoten
//			// noch nicht kontrahiert wurden
//			std::priority_queue<uint_pair, std::vector<uint_pair>, compare_uint_pair> goodNodes;
//			//std::vector<uint_pair> goodNodes;
//			//unsigned int* goodNodes;
//			//unsigned int goodNodesSize;
//			
//			unsigned int* EdgeLoads;
//	
//			// es wird stehts ein Graph* g gebraucht
//			SCGraph(){ std::cout << "Error: -41" << std::endl; }
//	
//			void clearAlmostEverything();
//	
//		public:
//			SCGraph(Graph* gr);
//			~SCGraph();
//			
//			// geben dem graph einzelne shortcuts/nodes, 
//			// die später gemerged/entfernt werden sollen
//			// stellt sich raus, dies brauchen wir nicht aktiv
//			void addShortcut(unsigned int source_node_id, S sc);
//			void blacklistNode(unsigned int node_id);
//	
//			// fügt über eine Runde übergebenen Shortcuts in den graph ein,
//			// entfernt übergebene blacklisted-nodes aus dem graph
//			bool mergeRound(unsigned int lvl);
//	
//			// vereint alle in den runden übergebenen shortcuts mit Graph* g
//			// hiernach sind EdgeData verfügbar
//			bool mergeShortcutsAndGraph(unsigned int lvl);
//	
//			bool isShortcut(unsigned int edge_id);
//	
//			// gibt die runde aus, in der ein knoten kontrahiert, 
//			// bzw aus dem graph entfernt wurde
//			// falls 0 zurück gegeben wird, befindet sich der graph noch
//			// in der CH oder es wurden teilweise lvl-ids als 0 übergeben
//			// (was man vermeiden sollte, wenn das funktionieren soll)
//			unsigned int getNodeLVL(unsigned int node_id);
//	
//			unsigned int getNodeCount();
//			unsigned int getEdgeCount();
//	
//			//manchmal will man sich freihalten,
//			//ob das verhalten des graphen in runden anders abgefragt
//			//wird als im endgraphen
//			unsigned int getEdgeCount(unsigned int node_id);
//			unsigned int getEdgeCount_Round(unsigned int node_id);
//	
//			unsigned int getShortcutCount();
//			
//			// dies ist die schnittstelle zur
//			// CHConstruction
//			std::list<Shortcut>* getShortcutListPointer(){ 
//				return round_shortcutlist; 
//			}
//			std::list<unsigned int>* getBlackNodesListPointer(){ 
//				return &round_node_blacklist; 
//			}
//			std::priority_queue
//				<uint_pair, std::vector<uint_pair>, compare_uint_pair>* getGoodNodes();
//			//std::vector<uint_pair>* getGoodNodes();
//			//unsigned int* getGoodNodes();
//			//unsigned int getGoodNodesSize();
//	
//			ND getNodeData(unsigned int node_id);
//			ND* getNodeDataPointer(){ return node_data; }
//			ED getEdgeData(unsigned int edge_id);
//	
//			// gibt jeweiliges Ende einer Kante
//			// zu der Kanten-ID raus
//			E* getOutEdge(unsigned int edge_id);
//			E* getInEdge(unsigned int edge_id);
//			E* getEdge(bool out0_in1, unsigned int edge_id);
//			
//			E* copyOutEdge(unsigned int edge_id);
//			E* copyInEdge(unsigned int edge_id);
//	
//			// zum aktualisieren der Loads der
//			// Kanten/Shortcuts
//			//
//			// uint_pair: siehe struct.h
//			void updateEdgeLoads(std::list< uint_pair >* edge_load_values);
//			// das array enthält für edge_load[i] die belastung für
//			// die kante i 
//			void updateEdgeLoads(unsigned int* edge_loads, unsigned int array_length);
//			// verteilt Belastungen der Shortcuts auf deren Eltern
//			void shareShortcutLoads();
//	
//			// verteilt die Loads des internen
//			// EdgeLoads Arrayas, inklusive Shortcuts
//			void updateEdgeLoads();
//			// erhöht Load der übergebenen Edge um 1
//			void addEdgeLoad(unsigned int edge_id);
//			// erhöht Load der angegebenen Edge um times
//			void addEdgeLoad(unsigned int edge_id, unsigned int times);
//	
//			// Edges-Iteratoren wie im anderen Graph,
//			// auch hier möchte man während der runden ein
//			// evtl. anderes verhalten der funktionen haben
//			EdgesIterator getOutEdgesIt(unsigned int node){
//				unsigned int nofs = nodes_out_offs[node] ;
//				unsigned int c = nodes_out_offs[node+1] - nofs ;
//				return EdgesIterator(out_edges + nofs , c ); 
//			}
//			EdgesIterator getInEdgesIt(unsigned int node){
//				unsigned int nifs = nodes_in_offs[node] ;
//				unsigned int c = nodes_in_offs[node+1] - nifs ;
//				return EdgesIterator(in_edges + nifs , c ); 
//			}
//	
//			EdgesIterator getOutEdgesIt_Round(unsigned int node){
//				unsigned int nofs = nodes_out_offs[node] ;
//				unsigned int c = node_out_edges_count[node] ;
//				return EdgesIterator(out_edges + nofs , c ); 
//			}
//			EdgesIterator getInEdgesIt_Round(unsigned int node){
//				unsigned int nifs = nodes_in_offs[node] ;
//				unsigned int c = node_in_edges_count[node] ;
//				return EdgesIterator(in_edges + nifs , c ); 
//			}
//			/*
//			 * out0_in1 == false == 0 => gebe OutEdges-Iterator
//			 * out0_in1 == true == 1 => gebe InEdges-Iterator
//			 */
//			EdgesIterator getEdgesIt(bool out0_in1, unsigned int node_id){
//				unsigned int nfs;
//				unsigned int c;
//				if( out0_in1 ){
//					nfs = nodes_in_offs[node_id] ;
//					c = nodes_in_offs[node_id+1] - nfs ;
//					return EdgesIterator(in_edges + nfs , c ); 
//				} else {
//					nfs = nodes_out_offs[node_id] ;
//					c = nodes_out_offs[node_id+1] - nfs ;
//					return EdgesIterator(out_edges + nfs , c ); 
//				}
//			}
//	};


/*
 * statischer Graph mit Daten eines normalen Graphen
 * und Shortcuts inklusive deren Daten
 *
 * kann über mehrere Runden Shortcuts
 * einfügen und Knoten aus dem Graph schneiden
 *
 * während der Runden sind die Edge-Daten der 
 * Edges nicht erreichbar
 *
 * merkt sich über alle Runden übergebene Shortcuts
 * und kann diese dann mit Graph* g
 * zu einem Graph zusammenfügen
 */
class SCGraph {
	private:
		Graph* g;

		unsigned int* node_lvl;
		
		unsigned int node_count;
		unsigned int edge_count;
		unsigned int shortcut_count;

		unsigned int* goodNodes;
		unsigned int* goodNodesIndex;
		unsigned int goodNodesSize;
		unsigned int current_edge_arrays_size;

		struct AdjEdges {
			Edge* start;
			unsigned int count;

			AdjEdges() : start(0), count(0) {}
			AdjEdges(Edge* s, unsigned int c) : start(s), count(c) {}
		};

		AdjEdges* nodes_out_edges;
		AdjEdges* nodes_in_edges;

		N* nodes_in_offs;
		E* in_edges; 
		N* nodes_out_offs;
		E* out_edges;

		ND* node_data;
		ED* edge_data;
	
		/* sortiert nach Source-Edges */
		std::list<Shortcut>* shortcutlist;

		std::list<Shortcut>* round_shortcutlist;
		std::list<unsigned int> round_node_blacklist;

		//std::priority_queue<uint_pair, std::vector<uint_pair>, compare_uint_pair> goodNodesHeap;
		uint_pair* goodNodesSorted;
		
		unsigned int* EdgeLoads;

		SCGraph(){ std::cout << "Error: -41" << std::endl; }

		void prepareNodes(unsigned int lvl);
		unsigned int sortShortcuts();
		void buildGraph();
		void fillGoodNodes();
		void buildGraphFinal();

	public:
		SCGraph(Graph* gr);
		~SCGraph();
		
		void addShortcut(unsigned int source_node_id, S sc);
		void blacklistNode(unsigned int node_id);

		bool mergeRound(unsigned int lvl);

		bool mergeShortcutsAndGraph(unsigned int lvl);

		bool isShortcut(unsigned int edge_id);

		unsigned int getNodeLVL(unsigned int node_id);

		unsigned int getNodeCount();
		unsigned int getEdgeCount();

		unsigned int getEdgeCount(unsigned int node_id);
		unsigned int getEdgeCount_Round(unsigned int node_id);

		unsigned int getShortcutCount();
		
		std::list<Shortcut>* getShortcutListPointer(){ 
			return round_shortcutlist; 
		}
		std::list<unsigned int>* getBlackNodesListPointer(){ 
			return &round_node_blacklist; 
		}
		//std::priority_queue
		//	<uint_pair, std::vector<uint_pair>, compare_uint_pair>* getGoodNodes(){
		//		return &goodNodesHeap;
		//	}
		uint_pair* getGoodNodes(){
			return goodNodesSorted;
		}
		unsigned int getGoodNodesSize(){
			return goodNodesSize;
		}

		ND getNodeData(unsigned int node_id);
		ND* getNodeDataPointer(){ return node_data; }
		ED getEdgeData(unsigned int edge_id);

		E* getOutEdge(unsigned int edge_id);
		E* getInEdge(unsigned int edge_id);
		E* getEdge(bool out1_in0, unsigned int edge_id);
		
		E* copyOutEdge(unsigned int edge_id);
		E* copyInEdge(unsigned int edge_id);

		void updateEdgeLoads(std::list< uint_pair >* edge_load_values);
		void updateEdgeLoads(unsigned int* edge_loads, unsigned int array_length);
		void shareShortcutLoads();

		void updateEdgeLoads();
		void addEdgeLoad(unsigned int edge_id);
		void addEdgeLoad(unsigned int edge_id, unsigned int times);

		EdgesIterator getOutEdgesIt(unsigned int node){
			unsigned int nofs = nodes_out_offs[node] ;
			unsigned int c = nodes_out_offs[node+1] - nofs ;
			return EdgesIterator(out_edges + nofs , c ); 
		}
		EdgesIterator getInEdgesIt(unsigned int node){
			unsigned int nifs = nodes_in_offs[node] ;
			unsigned int c = nodes_in_offs[node+1] - nifs ;
			return EdgesIterator(in_edges + nifs , c ); 
		}

		EdgesIterator getOutEdgesIt_Round(unsigned int node){
			Edge* s = nodes_out_edges[node].start ;
			unsigned int c = nodes_out_edges[node].count ;
			return EdgesIterator(s, c ); 
		}
		EdgesIterator getInEdgesIt_Round(unsigned int node){
			Edge* s = nodes_in_edges[node].start ;
			unsigned int c = nodes_in_edges[node].count ;
			return EdgesIterator(s , c ); 
		}
		/*
		 * out0_in1 == false == 0 => gebe OutEdges-Iterator
		 * out0_in1 == true == 1 => gebe InEdges-Iterator
		 */
		EdgesIterator getEdgesIt(bool out0_in1, unsigned int node_id){
			unsigned int nfs;
			unsigned int c;
			if( out0_in1 ){
				nfs = nodes_in_offs[node_id] ;
				c = nodes_in_offs[node_id+1] - nfs ;
				return EdgesIterator(in_edges + nfs , c ); 
			} else {
				nfs = nodes_out_offs[node_id] ;
				c = nodes_out_offs[node_id+1] - nfs ;
				return EdgesIterator(out_edges + nfs , c ); 
			}
		}
};

#endif
