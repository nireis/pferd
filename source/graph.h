#ifndef graph_h
#define graph_h

#include "structs.h"

// welche kanten/knoten der graph nutzt soll variabel sein
// wir wollen ein wenig rumexperimentieren, wie die datenstrukturen
// sich im platzverbrauch verhalten, ob uns also auch ne groessere 
// struktur wehtun wuerde
// E ~ Edges , N  ~ Nodes , S ~ Shortcuts
template <typename E, typename N, typename S> 
class Graph {

	/* 
	 * Iteratoren für Kanten von Nodes
	 */
	public:
		template <typename T>
		class Andrenator_DP {
			private:
				unsigned int max;
				unsigned int pos;
				T** start;
			public:
				Andrenator_DP(){
					pos = 0;
					max = 0;
					start = 0;
				}

				Andrenator_DP(unsigned int node, T** strt, unsigned int mx){
					start = strt;
					pos = 0;
					max = mx;
				}

				~Andrenator_DP(){
					start = 0;
				}
				
				bool hasNext(){
					return (pos < max);
				}

				T* getNext(){
					return start[pos++];
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

		/* 
		 * arrays aller kanten und knoten 
		 * */

		N* nodes; 
		/* 
		 * nodes muss groesse node_count+1 haben! 
		 * der letzte ist ein dummy, damit das mit den offsets klappt
		 */

		E* edges;
		E** in_edges; 
		/* 
		 * arrays von pointern auf kanten
		 * hierrin suchen wir mit den offsets nach kanten
		 * anhand der offsets in den nodes kann man direkt 
		 * in E* und E** die ausgehenden/eingehenden kanten finden
		 */

		/*
		 * dynamischer teil des graphen
		 * ( Shortcuts anders verwaltbar als in Listen? )
		 */
		
		SListExt<S> shortcutlist;
		S* shortcuts;
		S** in_shortcuts; 
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
		typedef Andrenator_P<S> OutShortcutsIterator;
		typedef Andrenator_DP<S> InShortcutsIterator;

		Graph();
		Graph(unsigned int nc, unsigned int ec, unsigned int sc, 
				N* n, E* e,
				S* s);
		/*
		 * Dies ist der für uns interessante Konstruktor
		 * Merke: es gibt NodeCount 'nc' viele Nodes, aber
		 * das Nodearray ist n+1 lang. 
		 * Der Dummy am Ende darf nie nach aussen gelangen.
		 */
		Graph(unsigned int nc, unsigned int ec, 
				N* n, E* e);
		
		/*
		 * falls jemand von uns ableitet
		 * wer auch immer sowas wollen würde...
		 */
		virtual ~Graph();
		
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
		void initShortcutOffsets(S* scarray, unsigned int scc);

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
		//E getEdge(unsigned int id);
		E* getEdge(unsigned int id); // andres stuff
		S getShortcut(unsigned int id);

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
				(node
				 ,&(in_edges[nodes[node].in_edge_offset]) 
				 ,nodes[node+1].in_edge_offset-nodes[node].in_edge_offset);
		}
		
		OutShortcutsIterator getOutShortcutsIt(unsigned int node){
			return InEdgesIterator
				(&(shortcuts[nodes[node].out_shortcut_offset]) 
				 ,nodes[node+1].out_shortcut_offset-nodes[node].out_shortcut_offset);
		}
		
		InShortcutsIterator getInShortcutsIt(unsigned int node){
			return InEdgesIterator
				(node
				 ,&(in_shortcuts[nodes[node].in_shortcut_offset]) 
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

#include "graph.hpp"

#endif
