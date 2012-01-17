#ifndef ch_h
#define ch_h

#include "graph.h"
#include "structs.h"
#include <vector>

typedef Shortcut S;
typedef ShortcutData SD;

class CH : public Dijkstra_Interface {
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
		CH(Graph* gr);
		~CH();
		
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

		virtual unsigned int getNodeCount();
		virtual unsigned int getEdgeCount();

		virtual EdgesIterator getOutEdgesIt(unsigned int node){
			return EdgesIterator
				( &(out_edges[nodes_out_offs[node]]) 
				 ,nodes_out_offs[node+1]-nodes_out_offs[node]);
		}
		virtual EdgesIterator getInEdgesIt(unsigned int node){
			return EdgesIterator
				( &(in_edges[nodes_in_offs[node]]) 
				 ,nodes_in_offs[node+1]-nodes_in_offs[node]);
		}

		virtual E* getOutEdge(unsigned int id){ return 0; }
		virtual E* getInEdge(unsigned int id){ return 0; }

};

class DynCH : public Dijkstra_Interface {
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
		SList<SD> shortcut_data;

		SList<unsigned int> uintlist;

	public:
		DynCH(Graph* gr);
		~DynCH();

		void addShortcut(S sc);

		unsigned int getShortcutCount();

		virtual unsigned int getNodeCount();
		virtual unsigned int getEdgeCount();

		virtual EdgesIterator getOutEdgesIt(unsigned int id){
			return EdgesIterator( & out_edges[id][0] , out_edges[id].size() );
		}
		virtual EdgesIterator getInEdgesIt(unsigned int id){
			return EdgesIterator( & in_edges[id][0] , in_edges[id].size() );
		}

		virtual E* getOutEdge(unsigned int id){ return 0; }
		virtual E* getInEdge(unsigned int id){ return 0; }
};

#endif
