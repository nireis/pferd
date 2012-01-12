#ifndef graph_h
#define graph_h

#include <iostream>
#include "structs.h"
#include "parser.h"

typedef unsigned int N;
typedef Node N2;
typedef NodeData ND;

typedef Edge E;
typedef EdgeData ED;
typedef EdgeData2 ED2;

class EdgesIterator_Interface {
	public:
		virtual bool hasNext() const  = 0;
		virtual E getNext() = 0;
};

class Dijkstra_Interface {
	public:
		typedef Andrenator_P<E> EdgesIterator;

		virtual unsigned int getNodeCount() = 0;
		virtual unsigned int getEdgeCount() = 0;

		virtual EdgesIterator getOutEdgesIt(unsigned int id) = 0;
		virtual EdgesIterator getInEdgesIt(unsigned int id) = 0;
};

class Graph : public Dijkstra_Interface {
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
		 *		TODO optional machen, ob .grp neu geschrieben wird
		 */
		
		/*
		 * Daten zu Strukturen abfragen
		 */
		virtual unsigned int getNodeCount();
		virtual unsigned int getEdgeCount();

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
		virtual EdgesIterator getOutEdgesIt(unsigned int node){
			unsigned int nofs = nodes_out_offs[node] ;
			unsigned int c = nodes_out_offs[node+1] - nofs ;
			return EdgesIterator
					((out_edges + nofs )
					, c ); 
		}
		virtual EdgesIterator getInEdgesIt(unsigned int node){
			return EdgesIterator
				( &(in_edges[nodes_in_offs[node] ]) 
				 ,nodes_in_offs[node+1] - nodes_in_offs[node] ); 
		}
		
		/*
		 * stuff
		 */
		void print(unsigned int i){
			for(unsigned int j = 0; j <= i; j++){
			E e = in_edges[j];
				std::cout << " id: " << e.id << std::endl;
				std::cout << " other_node: " << e.other_node << std::endl ;
				std::cout << " value: " << e.value << std::endl;
				std::cout << " type: " << edge_data[ e.id ].type << std::endl;
			}
		}

};


class Graph2 {
	public:
		class EdgesIterator {
			private:
				unsigned int max;
				E* start;
			public:
				EdgesIterator() : max(0), start(0) {}
		
				EdgesIterator(E* strt, unsigned int mx) : 
					max(mx), start(strt) {}
		
				~EdgesIterator(){
					start = 0;
				}
				
				bool hasNext() const {
					return max != 0;
				}
		
				E* getNext() {
					--max;
					return start++;
				}
		};
	private:
		bool is_set;
		static const int BinID;
		static const std::string dateiendung;

		unsigned int node_count;
		unsigned int edge_count;

		N* nodes_in_offs;
		E* in_edges; 
		N* nodes_out_offs;
		E* out_edges;
		ND* node_data;
		ED* edge_data;

		int checkDataFile(std::string graphdata);
		void writeBinaryGraphFile(std::string graphdata);
		bool readBinaryGraphFile(std::string graphdata);
		bool parseTextGraphFile(std::string graphdata);

	public:
		Graph2();
		~Graph2();

		bool isSet(){ return is_set; }
		bool setGraph(std::string graphdata, bool write_binary);
		unsigned int getNodeCount();
		unsigned int getEdgeCount();
		ND getNodeData(unsigned int id);
		ED getEdgeData(unsigned int id);

		unsigned int getLowerOutEdgeBound(unsigned int id);
		unsigned int getUpperOutEdgeBound(unsigned int id); 
		unsigned int getLowerInEdgeBound(unsigned int id);
		unsigned int getUpperInEdgeBound(unsigned int id); 
		E* getOutEdge(unsigned int id);
		E* getInEdge(unsigned int id);

		EdgesIterator getOutEdgesIt(unsigned int node){
			unsigned int nofs = nodes_out_offs[node] ;
			unsigned int c = nodes_out_offs[node+1] - nofs ;
			return EdgesIterator
					((out_edges + nofs )
					, c ); 
		}
		EdgesIterator getInEdgesIt(unsigned int node){
			return EdgesIterator
				( &(in_edges[nodes_in_offs[node] ]) 
				 ,nodes_in_offs[node+1] - nodes_in_offs[node] ); 
		}
};
#endif
