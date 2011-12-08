#ifndef graph_h
#define graph_h

#include <list>
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
				unsigned int pos;
				T* start;
			public:
				Andrenator_P(){
					pos = 0;
					max = 0;
					start = 0;
				}

				Andrenator_P(unsigned int node, T* strt, unsigned int mx){
					start = strt;
					pos = 0;
					max = mx;
				}

				~Andrenator_P(){
					start = 0;
				}
				
				bool hasNext(){
					return (pos < max);
				}

				T* getNext(){
					return &start[pos++];
				}
		};

	/*
	 * Eigentliche interne Daten des Graphen
	 */
	private:

		unsigned int node_count;
		unsigned int edge_count;
	
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
		
		S** shortcuts;
		/*
		 * eine Idee wäre, die Shortcutliste
		 * in nen Array umzusetzen, falls uns das was bringt.
		 * mal schauen
		 */
		SListExt<S> shortcutlist;
		//std::list<S> shortcuts; 
		/*
		 * alle shortscuts
		 * TODO schauen, ob vector mehr platz braucht
		 *	     oder flexibler und schneller ist
		 *
		 *	     alternativ eine eigene listenstruktur, 
		 *	     welche updates aus neu zugefügten SC's 
		 *	     zulässt?
		 */

		S** in_shortcuts; 
		S** out_shortcuts;
		/*
		 * arrays mit pointern auf shortcuts
		 * die pointer verweisen auf shortcuts, welche
		 * in der liste eingetragen sind
		 *
		 */


	/*
	 * Methoden und zeugs
	 */
	public:

		typedef Andrenator_P<E> OutEdgesIterator;
		typedef Andrenator_DP<E> InEdgesIterator;
		typedef Andrenator_DP<S> OutShortcutsIterator;
		typedef Andrenator_DP<S> InShortcutsIterator;

		Graph();
		Graph(unsigned int nc, unsigned int ec, 
				N* n, E* e,
				E** ie, E** oe,
				S** is, S** os);
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
		 * initialisiert die Offsets der Shortcuts
		 * Zeit/Platzaufwand abhängig von größe der Shortcut Liste
		 * wird aber allgemein um Faktor 2 mehr sein, als bei initOffsets
		 */
		void initShortcutOffsets();

		/*
		 * Shortcutliste komplett löschen
		 * 
		 * impliziert auch löschen der dazugehörigen offsets etc...
		 */
		void clearShortcuts();

		void addShortcut(S sc);

		unsigned int getNodeCount();
		unsigned int getEdgeCount();

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
					(node
					, &(edges[nodes[node].out_edge_offset])
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
				(node
				 ,&(in_edges[nodes[node].in_edge_offset]) 
				 ,nodes[node+1].in_edge_offset-nodes[node].in_edge_offset);
		}
		
		InShortcutsIterator getInShortcutsIt(unsigned int node){
			return InEdgesIterator
				(node
				 ,&(in_edges[nodes[node].in_edge_offset]) 
				 ,nodes[node+1].in_edge_offset-nodes[node].in_edge_offset);
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
