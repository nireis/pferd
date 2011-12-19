#ifndef graph_h
#define graph_h

#include <iostream>
#include "structs.h"
#include "parser.h"

typedef Node N;
typedef NodeData ND;

typedef Edge E;
typedef EdgeData ED;

typedef Shortcut S;
typedef ShortcutData SD;

class Graph {

	/* 
	 * Iteratoren für Kanten von Nodes
	 */
	public:
		template <typename T>
		class Andrenator_P {
			private:
				unsigned int max;
				T* start;
			public:
				Andrenator_P() : max(0), start(0) {}

				Andrenator_P(T* strt, unsigned int mx) : 
					max(mx), start(strt) {}

				~Andrenator_P(){
					start = 0;
				}
				
				bool hasNext(){
					return max != 0;
				}

				T* getNext(){
					--max;
					return start++;
				}
		};

	/*
	 * Eigentliche interne Daten des Graphen
	 */
	private:
		/*
		 * statischer teil des graphen
		 */

		unsigned int node_count;
		N* nodes_in_offs;
		N* nodes_out_offs;
		ND* node_data;
		/* 
		 * nodes arrays müssen groesse node_count+1 haben
		 * der letzte ist ein dummy, damit das mit den offsets klappt
		 */

		unsigned int edge_count;
		E* out_edges;
		E* in_edges; 
		ED* edge_data;

		/*
		 * dynamischer teil des graphen
		 */
		
		unsigned int shortcut_count;
		SListExt<S> shortcutlist;
		E* out_shortcuts;
		E* in_shortcuts; 
		SD* shortcut_data;
		/*
		 * wir lassen uns von aussen Shortcuts reingeben
		 * die entstehende Liste ist hoffentlich nicht zu groß
		 *
		 * spätestens, wenn wir die entsprechenden arrays initialisieren,
		 * löschen wir dir liste und unsere darstellung wird kompakt und schnell
		 */

		void writeBinaryGraphFile(std::string graphdata);
		bool readBinaryGraphFile(std::string graphdata);

	/*
	 * Methoden und zeugs
	 */
	public:

		typedef Andrenator_P<E> OutEdgesIterator;
		typedef Andrenator_P<E> InEdgesIterator;
		typedef Andrenator_P<E> OutShortcutsIterator;
		typedef Andrenator_P<E> InShortcutsIterator;

		Graph();
		~Graph();

		bool setGraph(std::string graphdata);
		
		/*
		 * andres stuff
		 *
		unsigned int getLowerEdgeBound(unsigned int id);
		unsigned int getUpperEdgeBound(unsigned int id); */
	
		unsigned int getNodeCount();
		unsigned int getEdgeCount();
		unsigned int getShortcutCount();

		/*
		 * initialisiert die Offsets der Shortcuts
		 * Zeit/Platzaufwand abhängig von größe der Shortcut Liste
		 * wird aber allgemein um Faktor 2 mehr sein, als bei initOffsets
		 */
		void initShortcutOffsets();

		/*
		 * siehe initShortcutOffsets(),
		 * nur wird hier ein array mit den shortcuts entgegen genommen
		 * es wird erwartet, dass die shortcuts im array
		 * nach source-knoten aufsteigend sortiert sind
		 */
		void initShortcutOffsets(S* scarray, unsigned int scc);

		/*
		 * Shortcutliste komplett löschen
		 * impliziert auch löschen der dazugehörigen offsets etc...
		 */
		void clearShortcuts();

		void addShortcut(S sc);

		/*
		 * daten zu strukturen abfragen
		 */
		ND getNodeData(unsigned int id);//TODO TODO
		ED getEdgeData(unsigned int id);//TODO TODO
		SD getShortcutData(unsigned int id);//TODO TODO

		E* getEdge(unsigned int id); // andres stuff

		/*
		 * hier werden iteratoren über kanten nach aussen gegeben
		 * 
		 * ein iterator funktioniert 
		 *		* nur ein mal, wenn man damit alle kanten anschaut
		 *		* für nur einen knoten
		 *		* für genau alle kanten der angeforderten richtung 
		 */
		OutEdgesIterator getOutEdgesIt(unsigned int node){
			return OutEdgesIterator
					(&(out_edges[nodes_out_offs[node].edge_offset])
					,nodes_out_offs[node+1].edge_offset-nodes_out_offs[node].edge_offset);
		}

		InEdgesIterator getInEdgesIt(unsigned int node){
			return InEdgesIterator
				( &(in_edges[nodes_in_offs[node].edge_offset]) 
				 ,nodes_in_offs[node+1].edge_offset-nodes_in_offs[node].edge_offset);
		}
		
		OutShortcutsIterator getOutShortcutsIt(unsigned int node){
			return OutShortcutsIterator
				( &(out_shortcuts[nodes_out_offs[node].shortcut_offset]) 
				 ,nodes_out_offs[node+1].shortcut_offset-nodes_out_offs[node].shortcut_offset);
		}
		
		InShortcutsIterator getInShortcutsIt(unsigned int node){
			return InShortcutsIterator
				( &(in_shortcuts[nodes_in_offs[node].shortcut_offset]) 
				 ,nodes_in_offs[node+1].shortcut_offset-nodes_in_offs[node].shortcut_offset);
		}


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


#endif
