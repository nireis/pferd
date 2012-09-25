#ifndef graph_h
#define graph_h

#include <iostream>
#include "structs.h"
#include "parser.h"
#include <vector>
#include <list>
#include <queue>
#include <algorithm>

class SCGraph;

/*
 * statischer Graph, der nur Kanten und Knoten
 * und deren Daten enthält
 */
class Graph {
	public:
		//typedef GEdge Edge;
		//typedef GEdgesIterator EdgesIterator;

	/*
	 * Zu jeder Struktur merken wir uns umfangreiche Daten in
	 *		*_data
	 *	Hierdurch sollen Zugriffe auf die anderen Informationen
	 *	schnell und kompakt werden.
	 */
	private:
		typedef Edge E;
		typedef EdgeData ED;
		typedef unsigned int N;
		typedef NodeData ND;
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

		unsigned int* EdgeLoads;

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
		ED* getEdgeDataPointer(){ return edge_data; }

		/*
		 * ohne Indexprüfung !
		 */
		unsigned int getLowerOutEdgeBound(unsigned int id);
		unsigned int getUpperOutEdgeBound(unsigned int id); 
		unsigned int getLowerInEdgeBound(unsigned int id);
		unsigned int getUpperInEdgeBound(unsigned int id); 
		
		E* getOutEdge(unsigned int edge_id);
		E* getInEdge(unsigned int edge_id);
		E* getEdge(bool out1_in0, unsigned int edge_id);
		
		E* copyOutEdge(unsigned int edge_id);
		E* copyInEdge(unsigned int edge_id);

		void addEdgeLoad(unsigned int edge_id);
		void addEdgeLoad(unsigned int edge_id, unsigned int times);

		/*
		 * zum Updaten von Kantenbelastungen
		 * übernimmt Belastungen eines SCGraph
		 * ignoriert die Belastungen der Shortcuts vom 
		 * SCGraph, diese muss er vorher selbst
		 * verteilt haben.
		 */
		void getEdgeLoads(SCGraph* g);

		/*
		 * zum Updaten der Values der Kanten, 
		 * wenn diese von aussen in EdgeData
		 * geändert wurden
		 */
		void updateEdgeValues();
		void updateEdgeLoads();

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
	public:
		//typedef SCGEdge Edge;
		//typedef SCGEdgesIterator EdgesIterator;
		//typedef GEdge RoundEdge;
		//typedef GEdgesIterator RoundEdgesIterator;
	
	private:
		typedef Edge E;
		typedef EdgeData ED;
		typedef unsigned int N;
		typedef NodeData ND;
		typedef Shortcut S;

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
		Edge* round_in_edges;
		N* nodes_out_offs;
		E* out_edges;
		Edge* round_out_edges;

		ND* node_data;
		ED* edge_data;
	
		/* sortiert nach Source-Edges */
		std::list<Shortcut>* shortcutlist;

		std::list<Shortcut>* round_shortcutlist;
		std::list<unsigned int> round_node_blacklist;

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

		bool mergeRound(unsigned int lvl, bool verbose);

		bool mergeShortcutsAndGraph(unsigned int lvl, bool verbose);

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
		uint_pair* getGoodNodes(){
			return goodNodesSorted;
		}
		unsigned int getGoodNodesSize(){
			return goodNodesSize;
		}

		ND getNodeData(unsigned int node_id);
		ND* getNodeDataPointer(){ return node_data; }
		ED getEdgeData(unsigned int edge_id);
		ED* getEdgeDataPointer(){ return edge_data; }

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
