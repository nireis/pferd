#ifndef graph_h
#define graph_h

#include "structs.h"

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
		class Andrenator_DP {
			private:
				unsigned int max;
				T** start;
			public:
				Andrenator_DP(){
					max = 0;
					start = 0;
				}

				Andrenator_DP(T** strt, unsigned int mx){
					start = strt;
					max = mx;
				}

				~Andrenator_DP(){
					start = 0;
				}
				
				bool hasNext(){
					return max != 0;
				}

				T* getNext(){
					max--;
					return *start++;
				}
		}; 
		template <typename T>
		class Andrenator_P {
			private:
				unsigned int max;
				T* start;
			public:
				Andrenator_P(){
					max = 0;
					start = 0;
				}

				Andrenator_P(T* strt, unsigned int mx){
					start = strt;
					max = mx;
				}

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

		unsigned int node_count;
		unsigned int edge_count;
		unsigned int shortcut_count;
	
		/*
		 * statischer teil des graphen
		 */

		N* nodes_in_offs;
		N* nodes_out_offs;
		ND* node_data;
		/* 
		 * nodes arrays müssen groesse node_count+1 haben! 
		 * der letzte ist ein dummy, damit das mit den offsets klappt
		 */

		E* out_edges;
		E* in_edges; 
		ED* edge_data;

		/*
		 * dynamischer teil des graphen
		 */
		
		SListExt<S> shortcutlist;
		S* out_shortcuts;
		S* in_shortcuts; 
		SD* shortcut_data;
		/*
		 * wir lassen uns von aussen Shortcuts reingeben
		 * die entstehende Liste ist hoffentlich nicht zu groß
		 *
		 * spätestens, wenn wir die entsprechenden arrays initialisieren,
		 * löschen wir dir liste und unsere darstellung wird kompakt und schnell
		 */

	/*
	 * Methoden und zeugs
	 */
	public:

		typedef Andrenator_P<E> OutEdgesIterator;
		typedef Andrenator_DP<E> InEdgesIterator;
		typedef Andrenator_P<E> OutShortcutsIterator;
		typedef Andrenator_DP<E> InShortcutsIterator;

		Graph();
		Graph(unsigned int nc, unsigned int ec,
				N* n, E* oe, E* ie, ED* ed);
		/*
		 * Dies ist der für uns interessante Konstruktor
		 * Merke: es gibt NodeCount 'nc' viele Nodes, aber
		 * das Nodearray ist n+1 lang. 
		 * Der Dummy am Ende darf nie nach aussen gelangen.
		 */
		
		~Graph();
		
		/*
		 * initialisiert die Offsets für out/in edges der nodes
		 * das ganze passiert in O( 2*edge_count + node_count ) Zeit
		 * dynamischer Platzverbrauch liegt etwa in O( 4*edge_count )
		 */
		void initOffsets();

		/*
		 * andres stuff
		 */
		unsigned int getLowerEdgeBound(unsigned int id);

		unsigned int getUpperEdgeBound(unsigned int id);

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
		void initShortcutOffsets(S* osc, S* isc, SD* sd, unsigned int scc);

		/*
		 * Shortcutliste komplett löschen
		 * 
		 * impliziert auch löschen der dazugehörigen offsets etc...
		 */
		void clearShortcuts();

		void addShortcut(S sc);

		unsigned int getNodeCount();
		unsigned int getEdgeCount();
		unsigned int getShortcutCount();

		/*
		 * Knoten und Kanten abfragen
		 *
		 * bei uns hat alles eine id,
		 * nur die nodes wissen ihre eigene nicht
		 */
		N getNode(unsigned int id);
		ND getNodeData(unsigned int id);//TODO TODO
		//E getEdge(unsigned int id);
		E* getEdge(unsigned int id); // andres stuff
		ED getEdgeData(unsigned int id);//TODO TODO
		S getShortcut(unsigned int id);
		SD getShortcutData(unsigned int id);//TODO TODO

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
					(&(edges[nodes[node].out_edge_offset])
					,nodes[node+1].out_edge_offset-nodes[node].out_edge_offset);
		}

		InEdgesIterator getInEdgesIt(unsigned int node){
			return InEdgesIterator
				( &(in_edges[nodes[node].in_edge_offset]) 
				 ,nodes[node+1].in_edge_offset-nodes[node].in_edge_offset);
		}
		
		OutShortcutsIterator getOutShortcutsIt(unsigned int node){
			return OutShortcutsIterator
				( &(shortcuts[nodes[node].out_shortcut_offset]) 
				 ,nodes[node+1].out_shortcut_offset-nodes[node].out_shortcut_offset);
		}
		
		InShortcutsIterator getInShortcutsIt(unsigned int node){
			return InShortcutsIterator
				( &(in_shortcuts[nodes[node].in_shortcut_offset]) 
				 ,nodes[node+1].in_shortcut_offset-nodes[node].in_shortcut_offset);
		}

		/*
		 *		von hier an gibt es nichts zu sehen
		 */
/*
		void printGraph(){

			
			Graph::OutEdgesIterator it = getOutEdges(0);

			std::cout << node_count << std::endl;
			std::cout << edge_count << std::endl;
			for(unsigned int i = 0; i < node_count; i++){
				std::cout << i<< " " << nodes[i].lat<< " "  << nodes[i].lon<< " "  << nodes[i].elevation << std::endl;
			}
			for(unsigned int i = 0; i < edge_count; i++){
				std::cout << edges[i].source << " " << edges[i].target << " "  << edges[i].distance << " "  << edges[i].type << std::endl;
			}
		}
		void printGraphIt(){

			
			std::cout << node_count << std::endl;
			std::cout << edge_count << std::endl;
			for(unsigned int i = 0; i < node_count; i++){
				std::cout << i<< " " << nodes[i].lat<< " "  << nodes[i].lon<< " "  << nodes[i].elevation << std::endl;
			}
			E* e;
			for(unsigned int i = 0; i < node_count; i++){
				OutEdgesIterator it = getOutEdges(i);
				while (it.hasNext()){
					e = it.getNext();
					std::cout << e->source << " " << e->target << " "  << e->distance << " "  << e->type << std::endl;
				}
			}
		}
*/
};


#endif
