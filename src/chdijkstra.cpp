#include "chdijkstra.h"

CHDijkstras::CHDijkstras(SCGraph* g):
	nr_of_nodes(g->getNodeCount()),
	o_found_by(2,vector<int> (nr_of_nodes,-1)),
	marked(g->getEdgeCount() + g->getShortcutCount(), false),
	m_found_by(nr_of_nodes,-1),
	o_reset_found_by(2){
	this->g = g;
}

unsigned int CHDijkstras::oneToOne(unsigned int node_id0, unsigned int node_id1){
	// Wegen Codeschönheit: Array für Source und Target anlegen.
	vector<unsigned int> node_id(2);
	node_id[0] = node_id0;
	node_id[1] = node_id1;
	// Die priotiry_queue, welche der Menge U im Dijkstra entspricht
	std::priority_queue<U_element_bi, std::vector<U_element_bi>, Compare_U_element_bi> U;
	unsigned int min_path_length = numeric_limits<unsigned int>::max();
	unsigned int min_path_center;
	vector<unsigned int> dist(nr_of_nodes);

	unsigned int numEdges = 0;
	unsigned int numNodes = 2;

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
			numEdges++;
		}
	}
	dist[node_id0] = 0;
	dist[node_id1] = 0;

	// Die restlichen Knoten abarbeiten
	while(!U.empty() && min_path_length > U.top().distance){
		numNodes++;
		int i = U.top().found_by;
		unsigned int tmpid = U.top().id;
		if(o_found_by[i][tmpid] == -1){
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
				numEdges++;
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
				g->addEdgeLoad(takenedge);
				tmpid = g->getEdge(i, (unsigned int)takenedge)->other_node;
			}
		}
	}

//	cout << "numNodes: " << numNodes << endl;
//	cout << "numEdges: " << numEdges << endl;

	resetOneToOne();
	return min_path_length;
}

void CHDijkstras::resetOneToOne(){
	for(int i=0; i<2; i++){
		while(!o_reset_found_by[i].empty()){
			o_found_by[i][o_reset_found_by[i].back()] = -1;
			o_reset_found_by[i].pop_back();
		}
	}
}

void CHDijkstras::oneToMany(unsigned int node_id0, vector<unsigned int>* targets){
	// Von den targets alle aufsteigenden Kanten besuchen und markieren.
	/* !TODO! wenn die CH fertig ist, gibt es
	 *
	 * g->getEdgeCount() + g->getShortcutCount()
	 * viele Kanten insgesammt, die maximale Edge-ID ist
	 * (g->getEdgeCount() + g->getShortcutCount()) -1
	 */
	markAscEdges(targets, &marked);

	// Von node_id0 aus einen "normalen" Dijkstra machen und dabei aufsteigende
	// und markierte Kanten benutzen. Sobald wir alle targets gefunden haben
	// brechen wir ab.
	std::priority_queue<U_element, std::vector<U_element>, Compare_U_element> U;
	unsigned int tmpnode;
	// TODO zum Testen ist hier noch ein dist-vector drin.
	vector<unsigned int> dist(nr_of_nodes);

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
					m_reset_found_by.push_back(tmpnode);
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
			(*targets)[i] = dist[target];
			tmpfoundby = m_found_by[target];
		}
		else{
			if(!U.empty() && U.top().id == target){
				(*targets)[i] = U.top().distance;
				tmpfoundby = (int)U.top().eid;
				//...erst gerade gefunden wurde...
				if(target == targets->back()){
					dist[target] = U.top().distance;
					m_found_by[target] = (int)U.top().eid;
					m_reset_found_by.push_back(target);
				}
			}
			else{
				//...oder garnicht gefunden wurde.
				(*targets)[i] = numeric_limits<unsigned int>::max();
				dist[target] = numeric_limits<unsigned int>::max();
			}
		}
		// Backtracing der Knoten.
		if((*targets)[i] != numeric_limits<unsigned int>::max()){
			cout << "=== Node " << target << " ===" << endl;
			// ersten Knoten
			tmpnode = target;
			if(tmpnode != node_id0){
				cout << tmpnode << endl;
				int takenedge = tmpfoundby;
				// path->push_front(takenedge);
				tmpnode = g->getInEdge((unsigned int)takenedge)->other_node;
			}
			// restlichen Knoten
			while(tmpnode != node_id0){
				cout << tmpnode << endl;
				int takenedge = m_found_by[tmpnode];
				// path->push_front(takenedge);
				tmpnode = g->getInEdge((unsigned int)takenedge)->other_node;
			}
		}
	}
	resetOneToMany();
}

void CHDijkstras::resetOneToMany(){
	while(!m_reset_found_by.empty()){
		m_found_by[m_reset_found_by.back()] = -1;
		m_reset_found_by.pop_back();
	}
	while(!m_reset_marked.empty()){
		marked[m_reset_marked.back()] = false;
		m_reset_marked.pop_back();
	}
}

void CHDijkstras::markAscEdges(vector<unsigned int>* nodes, vector<unsigned int>* marked){
	vector<unsigned int> todo;
	// Erstmal alle Startknoten einfürgen. TODO man könnte auch
	// vllt direkt den nodes Vektor benutzen, je nach Implementierung
	// des Rests.
	// TODO Der marked Vektor muss im Moment noch komplett mit false initialisiert
	// übergeben werden.
	todo.assign(nodes->begin(), nodes->end());
	while(!todo.empty()){
		unsigned int tmpnode = todo.back();todo.pop_back();
		EdgesIterator it = g->getInEdgesIt(tmpnode);
		while(it.hasNext()){
			Edge* tmpedge = it.getNext();
			// Wenn wir nicht schon hier waren und es nach oben geht.
			if(!((*marked)[tmpedge->id]) && tmpedge->other_lvl > g->getNodeLVL(tmpnode)){
				(*marked)[tmpedge->id] = true;
				m_reset_marked.push_back(tmpedge->id);
				todo.push_back(tmpedge->other_node);
			}
		}
	}
}
