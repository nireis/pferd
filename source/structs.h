#ifndef structs_h
#define structs_h

struct ParserNode {
	unsigned int id;
	unsigned int distance;
	int elevation;
	double lan;
	double lon;
};
struct ParserEdge {
	unsigned int source;
	unsigned int target;
	unsigned int distance;
	unsigned int type;
};


struct Edge {
	unsigned int id; 
	unsigned int value;
	unsigned int other_node;
};
struct EdgeData {
	unsigned int out_index;
	unsigned int in_index;
	unsigned int distance;
	unsigned int type;
	unsigned int load;
};


struct Shortcut {
	unsigned int id;
	unsigned int distance;
	unsigned int source;
	unsigned int target;
	unsigned int papa_edge_id;
	unsigned int mama_edge_id;
};
struct ShortcutData {
	unsigned int papa_edge;
	unsigned int mama_edge;
};


struct Node {
	unsigned int edge_offset; 
	unsigned int shortcut_offset;
};
struct NodeData {
	unsigned int id;
	int elevation;
	double lat;
	double lon;
};


template <typename T>
class SList {
	protected:
		struct SListData;
		SListData* root;
		
		struct SListData{
			T data;
			SListData* next;
		};
	public:
		SList(){
			root = 0;
		}
		~SList(){
			clear();
		}
		bool empty(){
			if(root == 0)
				return true;

			return false;
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
			while(root != 0){
				SListData* t = root->next;
				delete root;
				root = t;
			}
		}
		class Iterator {
			private:
				SListData* position;
			public:
				Iterator(SListData* r){
					position = r;
				}
				~Iterator(){
					position = 0;
				}
				bool hasNext(){
					if(position == 0)
						return false;
					
					return true;
				}
				T& getNext(){
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
 * Erweiterung der push/pop Liste 
 * um Mitzählen der Größe
 * und peek()
 */
class SListExt {
	protected:
		unsigned int siz;
		SList<T> list;
	public:
		SListExt() : list() {
			siz = 0;
		}
		~SListExt(){
			list.clear();
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
		unsigned int size(){
			return siz;
		}

		bool empty(){
			return list.empty();
		}

		typedef typename SList<T>::Iterator Iterator;

		Iterator getIterator(){
			return list.getIterator();
		}
};


/* TODO überflüssig */
class Heap {
	private:
		struct HeapElement;
		
		unsigned int max_element_count;
		unsigned int max_heap_element;
		bool emty;
	
		HeapElement* heap_array;
		unsigned int* element_ids;
	
		struct HeapElement {
			unsigned int  key;
			unsigned int id;
			unsigned int data;
		};

		void heapify(unsigned int key_id){
			unsigned int lc, rc;
			lc = key_id*2 + 1;
			rc = key_id*2 + 2;

			if( rc>max_heap_element ){
				if( lc> max_heap_element ){
					return ; // schon ganz unten
				} else {
					if(heap_array[key_id].key > heap_array[lc].key)  // hat linkes kind
						swap(key_id, lc);
				}
			} else { // beide kinder vorhanden
				if(heap_array[rc].key > heap_array[lc].key){
					if(heap_array[key_id].key > heap_array[lc].key){
						swap(key_id, lc);
						heapify(lc);
					}
				} else {
					if(heap_array[key_id].key > heap_array[rc].key){
						swap(key_id, rc);
						heapify(rc);
					}
				}
			}  
		}

		void reheap(unsigned int key_id){
			if(key_id > 0){
				unsigned int p = (key_id - 1)/2;
				if(heap_array[key_id].key < heap_array[p].key){
					swap(key_id, p);
					reheap(p);
				}
			}
		}

		void find_new_min(){
		}


		
//			unsigned int lc, rc, akt;
//			akt = 0;
//			lc = 1;
//			rc = 2;
//			bool on_bottom = false;
//			// element ganz nach unten schieben
//			while( !on_bottom ){
//				// wenn es kein rechtes kind gibt
//				if( (rc>max_heap_element) ){
//					//gibt es ein linkes?
//					if( (lc>max_heap_element) ){
//						on_bottom = true; // wenn nein=>ende
//					} else {
//						on_bottom = true; // ja => letztes mal tauschen
//						swap(lc, akt); // dann ende
//						akt = lc;
//					}
//				} else { // es gibt ein rechtes kind 
//					// => es gibt ein linkes 
//					// => weiter absteigen
//					if( (heap_array[lc].key < heap_array[rc].key) ){
//						swap(lc, akt);	
//						akt = lc;
//						lc = akt*2 + 1;
//						rc = akt*2 + 2;
//					} else {
//						swap(rc, akt);
//						akt = rc;
//						lc = akt*2 + 1;
//						rc = akt*2 + 2;
//					}
//				}
//			}
//			// nun sind wir ganz unten:
//			// hole neues element hervor 
//			// und schiebe dieses hoch
//
//			swap(akt, max_heap_element);
//			max_heap_element--;
//			
//			reheap(akt);
//		}

		void swap(unsigned int ido, unsigned int idt){
			HeapElement temp;
			unsigned int temp_id;

			temp = heap_array[ido];
			heap_array[ido] = heap_array[idt];
			heap_array[idt] = temp;

			temp_id = element_ids[heap_array[ido].id];
			element_ids[heap_array[ido].id] = element_ids[heap_array[idt].id];
			element_ids[heap_array[idt].id] = temp_id;
		}

	public:
		Heap(){
			max_element_count = 0;
			max_heap_element = 0;
			heap_array = 0;
			element_ids = 0;
			emty = true;
		}

		~Heap(){
			delete[] heap_array; heap_array = 0;
			delete[] element_ids; element_ids = 0;
		}

		Heap(unsigned int max_elements){
			max_element_count = max_elements;
			heap_array = new HeapElement[max_element_count];
			element_ids = new unsigned int[max_element_count];
			emty = true;

			for(unsigned int i = 0; i < max_element_count; i++){
				element_ids[i] = max_element_count;
			}
			max_heap_element = 0;
		}

		Heap(unsigned int max_elements, unsigned int first_key,unsigned int eid, unsigned int id){
			max_element_count = max_elements;
			heap_array = new HeapElement[max_element_count];
			element_ids = new unsigned int[max_element_count];
			emty = false;

			for(unsigned int i = 0; i < max_element_count; i++){
				element_ids[i] = max_element_count;
			}
			max_heap_element = 0;

			HeapElement t;
			t.key = first_key;
			t.id = id;
			t.data = eid;
			heap_array[0] = t;
			element_ids[id] = 0;
		}

		/*
		 * bitte bitte bitte
		 * nur maximal so viele elemente einfügen, 
		 * wie vorher angegeben
		 * die bereichsprüfung sparen wir uns nämlich!
		 */
		void push(unsigned int key, unsigned int  eid, unsigned int id){
			if(emty){
				HeapElement t;
				t.key = key;
				t.id = id;
				t.data = eid;
				heap_array[0] = t;
				element_ids[id] = 0;
				emty = false;		
			} else {
				max_heap_element++;
				
				HeapElement t;
				t.key = key;
				t.id = id;
				t.data = eid;
				heap_array[max_heap_element] = t;
				element_ids[id] = max_heap_element;
				reheap(max_heap_element);
			}
		}

		void pop(){
			//find_new_min();
			if(max_heap_element > 0){
				swap(0, max_heap_element);
				max_heap_element--;
				heapify(0);
			} else {
				emty = true;
			}
		}

		void decrease_key(unsigned int newkey, unsigned int key_id){
			heap_array[ element_ids[key_id] ].key = newkey;
			reheap( element_ids[key_id] );
		}

		bool empty(){
			return emty;
		}
		
		unsigned int top_eid(){
			return heap_array[0].data;
		}

		unsigned int top_key(){
			return heap_array[0].key;
		}

		unsigned int top_id(){
			return heap_array[0].id;
		}
		
//		void print(){
//			cout << "PRINTING HEAP: " << endl;
//			for(unsigned int i = 0; i <= max_heap_element; i++){
//				cout << heap_array[i].key << endl;
//			}
//			cout << "HEAP PRINTED !"  << endl;
//		}
		
//		unsigned int top(){
//			return heap_array[0].data;
//		//	return heap_array[0].key;
//		}
};













#endif
