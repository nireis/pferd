#ifndef graph_h
#define graph_h

#include <iostream>
#include "structs.h"
#include "parser.h"
#include <vector>

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

		/*
		 * get*Data - jeweils mit indexprüfung,
		 * ob die angegebene ID gültig ist.
		 * Ist diese nicht gültig, wird ein 
		 * Default-Initialisiertes Objekt zurück gegeben
		 */
		ND getNodeData(unsigned int id);
		ED getEdgeData(unsigned int id);

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
 * TODO Shortcuts brauchen nicht verwaltet werden,
 *	es sollen nur deren Informationen erfragbar sein.
 */
class SCGraph {
	private:
		/*
		 * Die CH ist eine erweiterte Graphstruktur,
		 * die Shortcuts macht und anschliessend
		 * mit den Edges und weiteren Informationen
		 * des Graphen g vermengt, um eine für sich praktische
		 * Darstellung zu erhalten,
		 * Das kostet Speicher, spart aber Arbeit.
		 */
		Graph* g;

		N* nodes_in_offs;
		N* nodes_out_offs;

		unsigned int* node_order;
		
		unsigned int node_count;
		unsigned int edge_count;
		unsigned int shortcut_count;

		E* out_edges;
		E* in_edges; 

		SD* shortcut_data;
	
		SList<S> shortcutlist;

		SList<unsigned int> uintlist;

	public:
		SCGraph(Graph* gr);
		~SCGraph();
		
		bool setShortcuts();
		/*
		 * initialisiert Offsets für die Shortcuts, 
		 * die über addShortcut(S s)) an die interne
		 * Shortcutliste übergeben wurden. 
		 *
		 * Die Shortcuts werden hierbei aus der Liste gelöscht.
		 *		TODO will man shortcuts nach 
		 *		dem initialisieren überhaupt behalten?
		 */

		void clearShortcuts();
		/* 
		 * löschen aller gesetzten Shortcuts + säubern der Liste 
		 */

		void clearShortcutlist();
		/* 
		 * löschen der Shortcut-Liste 
		 * die gesetzten Offsets werden behalten
		 */

		void clearShortcutOffsets();
		/* 
		 * löschen aller gesetzten Shortcuts
		 * die Shortcut-Liste wird behalten
		 */

		void addShortcut(S sc);

		unsigned int calcIndepSet();
		/*
		 * macht es Sinn, eine bestimmte Strategie
		 * entgegen zu nehmen, 
		 * um das IS zu bestimmen?
		 */

		SD getShortcutData(unsigned int id);
		unsigned int getShortcutCount();

		unsigned int getNodeCount();
		unsigned int getEdgeCount();

		EdgesIterator getOutEdgesIt(unsigned int node){
			return EdgesIterator
				( &(out_edges[nodes_out_offs[node]]) 
				 ,nodes_out_offs[node+1]-nodes_out_offs[node]);
		}
		EdgesIterator getInEdgesIt(unsigned int node){
			return EdgesIterator
				( &(in_edges[nodes_in_offs[node]]) 
				 ,nodes_in_offs[node+1]-nodes_in_offs[node]);
		}

		E* getOutEdge(unsigned int id){ return 0; }
		E* getInEdge(unsigned int id){ return 0; }


};

/*
 * Dynamischer Graph, in den Shortcuts eingefügt werden können 
 * (löschen sollte überflüssig sein)
 *
 * TODO da das Einfügen von Shortcuts in Runden passiert, 
 * daher pro Runde mehrere Shortcuts, geht dies evtl besser
 * als mit Listen
 */
class DSCGraph {
	private:
		std::vector<E>* out_edges;
		std::vector<E>* in_edges;

		Graph* g;

		unsigned int* node_order;
		
		unsigned int node_count;
		unsigned int edge_count;
		unsigned int shortcut_count;
		
		/*struct sc_id {
			sc_id(SD s, unsigned int i) : sd(s), id(i) {}
			sc_id() : sd(), id(0) {}
			SD sd;
			unsigned int id;
		};*/
		std::vector<SD> shortcut_data;

		SList<unsigned int> uintlist;

	public:
		DSCGraph(Graph* gr);
		~DSCGraph();

		void addShortcut(S sc);

		unsigned int getShortcutCount();

		unsigned int getNodeCount();
		unsigned int getEdgeCount();

		EdgesIterator getOutEdgesIt(unsigned int id){
			return EdgesIterator( & out_edges[id][0] , out_edges[id].size() );
		}
		EdgesIterator getInEdgesIt(unsigned int id){
			return EdgesIterator( & in_edges[id][0] , in_edges[id].size() );
		}

		E* getOutEdge(unsigned int id){ return 0; }
		E* getInEdge(unsigned int id){ return 0; }

};

#endif
