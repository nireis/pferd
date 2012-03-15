#ifndef structs_h
#define structs_h

/*
 * vom Parser zu setzende Strukturen, 
 * wie sie aus der Datei gelesen werden können
 */
struct ParserNode {
	ParserNode() : id(0), elevation(0), lat(0), lon(0) {}
	ParserNode(unsigned int i, double la, double lo, int e) :
		id(i), elevation(e), lat(la), lon(lo) {}
	
	unsigned int id;
	int elevation;
	double lat;
	double lon;
};
struct ParserEdge {
	ParserEdge() : source(0), target(0), distance(0), type(0) {}
	ParserEdge(unsigned int s, unsigned int t, unsigned int d, unsigned int ty) : 
		source(s), target(t), distance(d), type(ty) {}
	
	unsigned int source;
	unsigned int target;
	unsigned int distance;
	unsigned int type;
};

/*
 * "kleine" Strukturen, die optionale Daten
 * von wichtigen trennen
 *
 * eine Kante hat einen Wert value, der nicht zwingend der Distanz
 * entsprechen muss. Da man eine Kante von einem bestimmten Knoten aus
 * aufruft, interessiert nur der "andere Knoten"
 * die ID verweist auf die zugehörige Stelle im Daten-Array
 */
struct Edge {
	Edge() : id(0), value(0), other_node(0) {}
	Edge(unsigned int i, unsigned int v, unsigned int o) : 
		id(i), value(v), other_node(o) {}
	
	unsigned int id; 
	unsigned int value;
	unsigned int other_node;
};
struct EdgeData {
	EdgeData() : out_index(0), in_index(0), distance(0), type(0), load(0) {}
	EdgeData(unsigned int o, unsigned int i, unsigned int d, unsigned int t, unsigned int l) : 
		out_index(o), in_index(i), distance(d), type(t), load(l) {}
	
	unsigned int out_index;
	unsigned int in_index;
	unsigned int distance;
	unsigned int type;
	unsigned int load;
};
/*struct EdgeData2 {
	EdgeData2() : out_index(0), in_index(0), distance(0), type(0), load(0) {}
	EdgeData2(Edge* o, Edge* i, unsigned int d, unsigned int t, unsigned int l) : 
		out_index(o), in_index(i), distance(d), type(t), load(l) {}
	
	Edge* out_index;
	Edge* in_index;
	unsigned int distance;
	unsigned int type;
	unsigned int load;
};*/

/*struct Node {
	Node() : count(0), edges(0) {} //edge_offset(0), shortcut_offset(0) {}
	
	unsigned int count;
	Edge* edges;
	//unsigned int edge_offset; 
	//unsigned int shortcut_offset;
};*/

struct NodeData {
	NodeData() : id(0), elevation(0), lat(0), lon(0) {}
	NodeData(unsigned int i, int e, double la, double lo) : 
		id(i), elevation(e), lat(la), lon(lo) {}
	
	unsigned int id;
	int elevation;
	double lat;
	double lon;
};

/*
 * Shortcut, wie der Graph ihn als Eingabe erwartet
 *
 * die ID muss >= edge_count gewählt sein,
 * damit später Shortcuts und Edges unterschieden werden können
 */
struct Shortcut {
	unsigned int value;
	unsigned int source;
	unsigned int target;
	unsigned int papa_edge;
	unsigned int mama_edge;

	Shortcut()
		:value(0),source(0),target(0),papa_edge(0),mama_edge(0){}

	Shortcut(unsigned int v, unsigned int s, unsigned int t, unsigned int p, unsigned int m)
		:value(v),source(s),target(t),papa_edge(p),mama_edge(m){}

	Shortcut& operator=(Shortcut const& sc){
		value = sc.value;
		source = sc.source;
		target = sc.target;
		papa_edge = sc.papa_edge;
		mama_edge = sc.mama_edge;
		return *this;
	}
};

/*
 * die einzigen Informationen eines Shortcuts, 
 * die ausgelagert werden sollten.
 */
struct ShortcutData {
	ShortcutData() : out_index(0), in_index(0), papa_edge(0), mama_edge(0), load(0) {}
	ShortcutData(unsigned int o, unsigned int i, unsigned int p, unsigned int m, unsigned int l) :
		out_index(o), in_index(i), papa_edge(p), mama_edge(m), load(l) {}

	unsigned int out_index;
	unsigned int in_index;
	unsigned int papa_edge;
	unsigned int mama_edge;
	unsigned int load;
};

/*
 * eine LIFO mit Iterator
 */
template <typename T>
class SList_Base {
	protected:
		struct SListData;
		SListData* root;
		
		struct SListData{
			T data;
			SListData* next;
		};
	public:
		SList_Base() : root(0) {
		}
		~SList_Base(){
			//clear();
			root = 0;
		}
		bool empty() const {
			return (root == 0);
		}
		void push(T d){
				SListData* t = new SListData;
			if(root == 0){
				root = t;
				root->next = 0;
				root->data = d;
			} else {
				t->next = root;
				root = t;
				root->data = d;
			}
			t = 0;
		}
		T pop(){
			T d = T();
			if(root != 0){
				SListData* t = root->next;
				d = root->data;
				delete root;
				root = t;
			} // wenn Liste leer,
			// gibt es für T hoffentlich
			// sinnvole default belegungen
			return d;
		}
		void clear(){
			SListData* t;
			while(root != 0){
				t = root->next;
				delete root;
				root = t;
			}
		}
		class Iterator {
			private:
				SListData* position;
			public:
				Iterator(SListData* r) : position(r) {
				}
				~Iterator(){
					position = 0;
				}
				bool hasNext() const {
					return (position != 0);
				}
				T/*T&*/ getNext(){
					SListData* t = position;
					position = position->next;
					return t->data;
				}
		};
		Iterator getIterator(){
			Iterator i = Iterator(this->root);
			return i;
		}
};
 
template <typename T>
/*
 * Erweiterung der LIFO
 * um Mitzählen der Größe
 * und "peek()"
 */
class SList_Inner {
	protected:
		unsigned int siz;
		SList_Base<T> list;
	public:
		SList_Inner() : siz(0), list() {
		}
		~SList_Inner() {
			//list.clear();
		}

		void push(T d){
			siz++;
			list.push(d);
		}
		T pop(){
			siz--;
			T d = list.pop();
			return d;
		}
		void clear(){
			siz = 0;
			list.clear();
		}

		T peek(){
			T d = list.pop();
			list.push(d);
			return d;
		}
		unsigned int size() const {
			return siz;
		}

		bool empty() const {
			return list.empty();
		}

		typedef typename SList_Base<T>::Iterator Iterator;

		Iterator getIterator(){
			return list.getIterator();
		}
};

template <typename T>
class SList {
	protected:
		SList_Inner<T> list;
	public:
		SList() : list() {
		}
		~SList() {
			list.clear();
		}

		void push(T d){
			list.push(d);
		}
		T pop(){
			return list.pop();
		}
		void clear(){
			list.clear();
		}

		T peek(){
			return list.peek();
		}
		unsigned int size() const {
			return list.size();
		}

		bool empty() const {
			return list.empty();
		}

		typedef typename SList_Inner<T>::Iterator Iterator;

		Iterator getIterator(){
			return list.getIterator();
		}

		SList_Inner<T> getInnerList(){
			return list;
		}
};


/*
 * Ein Iterator für gewisse Strukturen
 */
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
		
		bool hasNext() const {
			return max != 0;
		}

		T* getNext() {
			--max;
			return start++;
		}
};

class bitvec {
	private:
		typedef unsigned int classtype;
		classtype* vector;
		unsigned int bit_size;
		unsigned int int_size;
		//static const classtype zero = 0;
		//static const classtype one = 1;
		static const int mult = (8*sizeof(classtype));
	public:
		bitvec() : vector(0) , bit_size(0), int_size(0) {}
		bitvec(unsigned int s){
			vector = 0;
			init(s);
		}
		~bitvec(){ del(); }

		void init(unsigned int s){
			delete[] vector; vector = 0;

			bit_size = s;
			if( s % (8*sizeof(classtype)) == 0 ){
				int_size = (s / mult);
			} else {
				int_size = (s / mult) + 1;
			}
			vector = new classtype[int_size];
		}

		bool get(unsigned int index){
			unsigned int rest = index % mult;
			unsigned int div = index / mult;

			return vector[div] & ( 1 << rest);
		}

		void set(unsigned int index){
			unsigned int rest = index % mult;
			unsigned int div = index / mult;

			vector[div] |= ( 1 << rest );
		}

		void reset(){
			for(unsigned int i = 0; i < int_size; i++){
				vector[i] = 0;
			}
		}

		void del(){
			delete[] vector; vector = 0; 
		}
};

class uintvec {
	private:
		unsigned int* vector;
		unsigned int size;
	public:
		uintvec() : vector(0), size(0) {}
		uintvec(unsigned int s){
			vector = 0;
			init(s);
		}
		~uintvec(){ delete[] vector; vector = 0; }

		void init(unsigned int s){
			vector = new unsigned int[s];
			size = s;
		}

		unsigned int& operator[](unsigned int const& index){
			return vector[index];
		}

		void reset(){
			for(unsigned int i = 0; i < size; i++){
				vector[i] = 0;
			}
		}
		void reset(unsigned int r){
			for(unsigned int i = 0; i < size; i++){
				vector[i] = r;
			}
		}

};
#endif
