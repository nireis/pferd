/* Aus einigen Ueberlegungen heraus stellen wir fest,
 * dass die ganzen Structs nicht wuerdig sind, 
 * in der Naehe anderen Codes zu verweilen
 */

#ifndef structs_h
#define structs_h

struct Simple_Edge {
	unsigned int distance;
	unsigned int source;
	unsigned int target;
};

struct Edge : public Simple_Edge {
	Edge(){
		type = 0;
		id = 0;
		source = 0;
		target = 0;
		distance = 0;
	}

	unsigned int type; //stellt sich raus, char bringt uns nix
	unsigned int id; // damit man kanten im kantenarray finden kann
	// alternativ, statt ID, die differenz des zeigers auf Kante und
	// des zeigers auf anfang des arrays benutzen...
};

struct Shortcut : public Simple_Edge {
	Simple_Edge* srcEdg;
	bool from_is_sc; // damit wir wissen, ob dies ein shortcut ist,
	Simple_Edge* trgtEdg; // der weiter aufgemacht gehoert
	bool to_is_sc;
};

struct Simple_Node {
	unsigned int in_edge_offset; 
	unsigned int out_edge_offset;
};

struct Node : public Simple_Node {
	unsigned int in_shortcut_offset;
	unsigned int out_shortcut_offset;

	float lat;
	float lon;
	int elevation;
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
		virtual ~SList(){
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
};
 
template <typename T>
/*
 * Erweiterung der push/pop Liste 
 * um Mitzählen der Größe
 * und peek()
 * und einen Iterator durch die Liste
 */
class SListExt : public SList<T> {
	protected:
		unsigned int siz;
	public:
		SListExt(){
			siz = 0;
		}
		~SListExt(){
		}

		void push(T d){
			siz++;
			SList<T>::push(d);
		}
		T pop(){
			siz--;
			return SList<T>::pop();
		}
		void clear(){
			siz = 0;
			SList<T>::clear();
		}

		bool peek(T* t){
			if(siz != 0){
				t = & SList<T>::root->data;
				return true;
			}

			return false;
		}
		unsigned int size(){
			return siz;
		}
		class Iterator {
			private:
				typename SList<T>::SListData* position;
			public:
				Iterator(typename SList<T>::SListData* r){
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
				void getNext(T* t){
					t = & (position->data);
					position = position->next;
				}
		};
		Iterator getIterator(){
			Iterator i = Iterator(this->root);
			return i;
		}
};

#endif
