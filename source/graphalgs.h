#ifndef graphalgs_h
#define graphalgs_h

class Graphalgs{
	private:
		Graph<Edge, Node, Shortcut>* g;
	public:
		/*
		 * Konstruktor!
		 */
		Graphalgs(Graph<Edge, Node, Shortcut>* g);
		/*
		 * node_id ist die ID des Knotens, von dem aus Dijkstra gestartet werden soll.
		 *
		 * Macht einen "normalen" Dijkstra auf dem gegebenen Knoten.
		 */
		void Dijkstra(int node_id);
};

#endif
