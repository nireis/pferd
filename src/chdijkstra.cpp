#include "chdijkstra.h"

CHDijkstra::CHDijkstra(SCGraph* g):
	nr_of_nodes(g->getNodeCount()),
	dist(nr_of_nodes),
	o_found_by(2,vector<int> (nr_of_nodes,-1)),
	marked(g->getEdgeCount() + g->getShortcutCount(), false),
	m_found_by(nr_of_nodes,-1),
	o_reset_found_by(2){
	this->g = g;
	o_reset_found_by[0].reserve(nr_of_nodes);
	o_reset_found_by[1].reserve(nr_of_nodes);
}

unsigned int CHDijkstra::oneToOne(unsigned int node_id0, unsigned int node_id1,
		unsigned int weight){
	// Wegen Codeschönheit: Array für Source und Target anlegen.
	vector<unsigned int> node_id(2);
	node_id[0] = node_id0;
	node_id[1] = node_id1;
	// Die priotiry_queue, welche der Menge U im Dijkstra entspricht
	std::priority_queue<U_element_bi, std::vector<U_element_bi>, Compare_U_element_bi> U;
	unsigned int min_path_length = numeric_limits<unsigned int>::max();
	unsigned int min_path_center;

	//unsigned int numEdges = 0;
	//unsigned int numNodes = 2;

	// Die ersten Knoten abarbeiten
	if(node_id0 == node_id1){
		min_path_center = node_id0;
		return 0;
	}
	// Die Kanten der ersten beiden Knoten anschauen und die targets zu
	// U hinzufügen, wenn das Level größer ist.
	for(int i=0; i<2; i++){
		o_found_by[i][node_id[i]] = (int)node_id[i];
		o_reset_found_by[i].push_back(node_id[i]);
		EdgesIterator it = g->getEdgesIt(i, node_id[i]);
		while(it.hasNext()){
			Edge* currentEdge = it.getNext();
			if(currentEdge->other_lvl > g->getNodeLVL(node_id[i])){
				unsigned int currentEdgeTarget = currentEdge->other_node;
				// Wenn wir schon den anderen Knoten gefunden haben...
				if(currentEdgeTarget == node_id[(i+1)%2]){
					min_path_length = currentEdge->value;
					// Die Werte für das Backtracing setzen.
					min_path_center = node_id[(i+1)%2];
					o_found_by[i][node_id[(i+1)%2]] = (int)currentEdge->id;
					o_reset_found_by[i].push_back(node_id[(i+1)%2]);
				}
				else{
					// ...sonst tu sie in U
					U.push(U_element_bi(
						currentEdge->value,currentEdge->other_node,currentEdge->id,i));
				}
			}
			else{
				break;
			}
			// numEdges++;
		}
	}
	dist[node_id0] = 0;
	dist[node_id1] = 0;

	// Die restlichen Knoten abarbeiten
	while(!U.empty() && min_path_length > U.top().distance){
		int i = U.top().found_by;
		unsigned int tmpid = U.top().id;
		if(o_found_by[i][tmpid] == -1){
			// numNodes++;
			o_found_by[i][tmpid] = (int)U.top().eid;
			o_reset_found_by[i].push_back(tmpid);
			unsigned int tmp_min_path_length = dist[tmpid] + U.top().distance;
			// Wir prüfen ob ein minimaler Pfad gefunden wurde.
			if(o_found_by[(i+1)%2][tmpid] != (-1) && tmp_min_path_length < min_path_length){
				min_path_length = tmp_min_path_length;
				min_path_center = tmpid;
			}
			dist[tmpid] = U.top().distance;
			EdgesIterator it = g->getEdgesIt(i, tmpid);
			while(it.hasNext()){
				Edge* currentEdge = it.getNext();
				unsigned int currentEdgeTarget = currentEdge->other_node;
				// Wir laufen nur bergauf...
				if(currentEdge->other_lvl > g->getNodeLVL(tmpid)){
					// ...und tun die Knoten in U, wenn sie noch nicht abgearbeitet wurden.
					if(o_found_by[i][currentEdgeTarget] == -1){
						U.push(U_element_bi(
							currentEdge->value+dist[tmpid],currentEdge->other_node,currentEdge->id,i));
					}
				}
				else{
					break;
				}
				// numEdges++;
			}
		}
		U.pop();
	}

	// Backtracing.
	// Nur, wenn ein Pfad existiert...
	if(min_path_length != numeric_limits<unsigned int>::max()){
		// ...gehe von dem Treffpunkt jeweils zurück zum Anfangsknoten
		// und speichere die Kanten-IDs.
		for(int i=0; i<2; i++){
			unsigned int tmpid = min_path_center;
			while(tmpid != node_id[i]){
//				cout << tmpid << endl;
				int takenedge = o_found_by[i][tmpid];
				// path->push_front(takenedge);
				g->addEdgeLoad(takenedge, weight);
				/*TODO
				 * this is slow. we know 'other_node' by the time 
				 * we are about to touch the edge with id "tmpid"
				 * so instead of looking up other_node in the graph, 
				 * wen can just throw it in a struct in o_fond_by[i][...]
				 * together with the edge id.
				 */
				tmpid = g->getEdge(i, (unsigned int)takenedge)->other_node;
			}
		}
	}

	//cout << "numNodes: " << numNodes << endl;
	//cout << "numEdges: " << numEdges << endl;
	//cout << "===" << endl;

	resetOneToOne();
	return min_path_length;
}

void CHDijkstra::resetOneToOne(){
	for(int i=0; i<2; i++){
		while(!o_reset_found_by[i].empty()){
			o_found_by[i][o_reset_found_by[i].back()] = -1;
			o_reset_found_by[i].pop_back();
		}
	}
}

void CHDijkstra::oneToMany(unsigned int node_id0, vector<unsigned int>* targets,
		unsigned int weight){
	// Von den targets alle aufsteigenden Kanten besuchen und markieren.
	/* !TODO! wenn die CH fertig ist, gibt es
	 *
	 * g->getEdgeCount() + g->getShortcutCount()
	 * viele Kanten insgesammt, die maximale Edge-ID ist
	 * (g->getEdgeCount() + g->getShortcutCount()) -1
	 */
	markBackEdges(targets, &marked);

	// Von node_id0 aus einen "normalen" Dijkstra machen und dabei aufsteigende
	// und markierte Kanten benutzen. Sobald wir alle targets gefunden haben
	// brechen wir ab.
	std::priority_queue<U_element, std::vector<U_element>, Compare_U_element> U;
	unsigned int tmpnode;

	// Den ersten Knoten abarbeiten
	U.push(U_element(0,node_id0,0));

	// Die restlichen Knoten abarbeiten
	for(unsigned int i=0; i<targets->size(); i++){
		unsigned int target = (*targets)[i];
		if(m_found_by[target] == -1){
			while(!U.empty() && U.top().id != target){
				// Die Distanz Eintragen, wenn der kürzeste gefunden wurde (und weiter suchen)
				tmpnode = U.top().id;
				if(m_found_by[tmpnode] == -1){
					dist[tmpnode] = U.top().distance;
					m_found_by[tmpnode] = (int)U.top().eid;
					// Die ausgehenden Kanten durchgehen und wenn sie aufwärts gehen oder
					// markiert sind auf ihnen weitersuchen.
					EdgesIterator it = g->getOutEdgesIt(tmpnode);
					while(it.hasNext()){
						Edge* tmpedge = it.getNext();
						// Wenn der Knoten noch nicht gefunden wurde UND (sein Level größer ist
						// ODER die Kante markiert ist).
						if(m_found_by[tmpedge->other_node] == -1
								&& (tmpedge->other_lvl > g->getNodeLVL(tmpnode) || marked[tmpedge->id])){
							// ...tu sie in U
							U.push(U_element(
										tmpedge->value+U.top().distance,tmpedge->other_node,tmpedge->id));
						}
					}
				}
				U.pop();
			}
		}
		int tmpfoundby = -1;
		// Die Distanz des Knotens setzen, je nachdem ob er...
		if(m_found_by[target] != -1){
			//...schon gefunden wurde...
			tmpfoundby = m_found_by[target];
		}
		else{
			if(!U.empty() && U.top().id == target){
				tmpfoundby = (int)U.top().eid;
				//...erst gerade gefunden wurde...
				if(target == targets->back()){
					dist[target] = U.top().distance;
					m_found_by[target] = (int)U.top().eid;
				}
			}
			else{
				//...oder garnicht gefunden wurde.
				dist[target] = numeric_limits<unsigned int>::max();
			}
		}
		// Backtracing der Knoten.
		if(dist[target] != numeric_limits<unsigned int>::max()){
			// cout << "=== Node " << target << " ===" << endl;
			// ersten Knoten
			tmpnode = target;
			if(tmpnode != node_id0){
				// cout << tmpnode << endl;
				int takenedge = tmpfoundby;
				// path->push_front(takenedge);
				g->addEdgeLoad(takenedge, weight);
				tmpnode = g->getInEdge((unsigned int)takenedge)->other_node;
			}
			// restlichen Knoten
			while(tmpnode != node_id0){
				// cout << tmpnode << endl;
				int takenedge = m_found_by[tmpnode];
				// path->push_front(takenedge);
				g->addEdgeLoad(takenedge, weight);
				tmpnode = g->getInEdge((unsigned int)takenedge)->other_node;
			}
		}
	}
	resetOneToMany();
}

void CHDijkstra::manyToOne(vector<unsigned int>* sources, unsigned int node_id0,
		unsigned int weight){
	// Von den sources alle aufsteigenden Kanten besuchen und markieren.
	/* !TODO! wenn die CH fertig ist, gibt es
	 *
	 * g->getEdgeCount() + g->getShortcutCount()
	 * viele Kanten insgesammt, die maximale Edge-ID ist
	 * (g->getEdgeCount() + g->getShortcutCount()) -1
	 */
	markForwEdges(sources, &marked);

	// Von node_id0 aus einen "normalen" Dijkstra machen und dabei aufsteigende
	// und markierte Kanten benutzen. Sobald wir alle sources gefunden haben
	// brechen wir ab.
	std::priority_queue<U_element, std::vector<U_element>, Compare_U_element> U;
	unsigned int tmpnode;

	// Den ersten Knoten abarbeiten
	U.push(U_element(0,node_id0,0));

	// Die restlichen Knoten abarbeiten
	for(unsigned int i=0; i<sources->size(); i++){
		unsigned int target = (*sources)[i];
		if(m_found_by[target] == -1){
			while(!U.empty() && U.top().id != target){
				// Die Distanz Eintragen, wenn der kürzeste gefunden wurde (und weiter suchen)
				tmpnode = U.top().id;
				if(m_found_by[tmpnode] == -1){
					dist[tmpnode] = U.top().distance;
					m_found_by[tmpnode] = (int)U.top().eid;
					// Die ausgehenden Kanten durchgehen und wenn sie aufwärts gehen oder
					// markiert sind auf ihnen weitersuchen.
					EdgesIterator it = g->getInEdgesIt(tmpnode);
					while(it.hasNext()){
						Edge* tmpedge = it.getNext();
						// Wenn der Knoten noch nicht gefunden wurde UND (sein Level größer ist
						// ODER die Kante markiert ist).
						if(m_found_by[tmpedge->other_node] == -1
								&& (tmpedge->other_lvl > g->getNodeLVL(tmpnode) || marked[tmpedge->id])){
							// ...tu sie in U
							U.push(U_element(
										tmpedge->value+U.top().distance,tmpedge->other_node,tmpedge->id));
						}
					}
				}
				U.pop();
			}
		}
		int tmpfoundby = -1;
		// Die Distanz des Knotens setzen, je nachdem ob er...
		if(m_found_by[target] != -1){
			//...schon gefunden wurde...
			tmpfoundby = m_found_by[target];
		}
		else{
			if(!U.empty() && U.top().id == target){
				tmpfoundby = (int)U.top().eid;
				//...erst gerade gefunden wurde...
				if(target == sources->back()){
					dist[target] = U.top().distance;
					m_found_by[target] = (int)U.top().eid;
				}
			}
			else{
				//...oder garnicht gefunden wurde.
				dist[target] = numeric_limits<unsigned int>::max();
			}
		}
		// Backtracing der Knoten.
		if(dist[target] != numeric_limits<unsigned int>::max()){
			// ersten Knoten
			tmpnode = target;
			if(tmpnode != node_id0){
				int takenedge = tmpfoundby;
				g->addEdgeLoad(takenedge, weight);
				tmpnode = g->getOutEdge((unsigned int)takenedge)->other_node;
			}
			// restlichen Knoten
			while(tmpnode != node_id0){
				int takenedge = m_found_by[tmpnode];
				g->addEdgeLoad(takenedge, weight);
				tmpnode = g->getOutEdge((unsigned int)takenedge)->other_node;
			}
		}
	}
	resetOneToMany();
}

void CHDijkstra::resetOneToMany(){
	for(unsigned int i=0, s=m_found_by.size(); i<s; i++){
		m_found_by[i] = -1;
	}
	for(unsigned int i=0, s=g->getEdgeCount() + g->getShortcutCount(); i<s; i++){
		marked[i] = false;
	}
}

void CHDijkstra::markBackEdges(vector<unsigned int>* nodes, vector<unsigned int>* marked){
	vector<unsigned int> todo;
	// Erstmal alle Startknoten einfügen. Man könnte auch
	// vllt direkt den nodes Vektor benutzen, je nach Implementierung
	// des Rests.
	todo.assign(nodes->begin(), nodes->end());
	while(!todo.empty()){
		unsigned int tmpnode = todo.back();todo.pop_back();
		EdgesIterator it = g->getInEdgesIt(tmpnode);
		while(it.hasNext()){
			Edge* tmpedge = it.getNext();
			// Wenn wir nicht schon hier waren und es nach oben geht.
			if(tmpedge->other_lvl > g->getNodeLVL(tmpnode)){
				if(!((*marked)[tmpedge->id])){
					(*marked)[tmpedge->id] = true;
					todo.push_back(tmpedge->other_node);
				}
			}
			else{
				break;
			}
		}
	}
}

void CHDijkstra::markForwEdges(vector<unsigned int>* nodes, vector<unsigned int>* marked){
	vector<unsigned int> todo;
	// Erstmal alle Startknoten einfügen. Man könnte auch
	// vllt direkt den nodes Vektor benutzen, je nach Implementierung
	// des Rests.
	todo.assign(nodes->begin(), nodes->end());
	while(!todo.empty()){
		unsigned int tmpnode = todo.back();todo.pop_back();
		EdgesIterator it = g->getOutEdgesIt(tmpnode);
		while(it.hasNext()){
			Edge* tmpedge = it.getNext();
			// Wenn wir nicht schon hier waren und es nach oben geht.
			if(tmpedge->other_lvl > g->getNodeLVL(tmpnode)){
				if(!((*marked)[tmpedge->id])){
					(*marked)[tmpedge->id] = true;
					todo.push_back(tmpedge->other_node);
				}
			}
			else{
				break;
			}
		}
	}
}
