#ifndef graph_h
#define graph_h

#include <iostream>
#include "structs.h"
#include "parser.h"
#include <vector>
#include <list>

typedef unsigned int N;
//typedef Node N2;
typedef NodeData ND;

typedef Edge E;
typedef EdgeData ED;
//typedef EdgeData2 ED2;

typedef Shortcut S;
typedef ShortcutData SD;

typedef Andrenator_P<E> EdgesIterator;

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
		
		E* getOutEdge(unsigned int id);
		E* getInEdge(unsigned int id);

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

			//return EdgesIterator
			//	( &(in_edges[nodes_in_offs[node] ]) 
			//	 ,nodes_in_offs[node+1] - nodes_in_offs[node] ); 
		}

};

/*
 * statischer Graph mit Daten eines normalen Graphen
 * und Shortcuts, inklusive deren Daten
 *
 * kann über mehrere Runden Shortcuts
 * einfügen (wenn zuvor mindestens so viele Kanten
 * entfernt wurden) und Knoten aus dem Graph schneiden
 *
 * während der Runden sind Shortcuts als solche
 * nicht erkennbar
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

		N* nodes_in_offs;
		E* in_edges; 
		N* nodes_out_offs;
		E* out_edges;

		ND* node_data;
		ED* edge_data;
		//SD* shortcut_data;
	
		SList<S> shortcutlist;

		std::list<Shortcut> round_shortcutlist;
		std::list<unsigned int> round_node_blacklist;

		SList<unsigned int> free_edge_ids;

		//bitvec blacklist;

		unsigned int* node_in_edges_count;
		unsigned int* node_out_edges_count;

		SCGraph(){ std::cout << "Error: -41" << std::endl; }

		void clearEverything();

	public:
		SCGraph(Graph* gr);
		~SCGraph();
		
		void addShortcut(S sc);
		
		void blacklistNode(unsigned int node_id);

		//bool isBlack(unsigned int node_id);

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
			return &round_shortcutlist; }
		std::list<unsigned int>* getBlackNodesListPointer(){ 
			return &round_node_blacklist; }

		ND getNodeData(unsigned int node_id);
		ED getEdgeData(unsigned int edge_id);
		//SD getShortcutData(unsigned int edge_id);

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
			unsigned int nofs = nodes_out_offs[node] ;
			unsigned int c = node_out_edges_count[node] ;
			return EdgesIterator(out_edges + nofs , c ); 
		}
		EdgesIterator getInEdgesIt_Round(unsigned int node){
			unsigned int nifs = nodes_in_offs[node] ;
			unsigned int c = node_in_edges_count[node] ;
			return EdgesIterator(in_edges + nifs , c ); 
		}

};


#endif
