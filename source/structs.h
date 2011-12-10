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
	unsigned int type;
	//type nur bei nicht-shortcuts richtiger type
	unsigned int id; 
};

/*
 * Shortcuts sind für uns ersmtal Edges, 
 * denen wir IDs geben, die groößer sind,
 * als wir maximal statische Kanten haben
 * => rausfinden, ob Kante ein SC sehr leicht
 *
 * wenn wir in Edges noch mindestens
 * einen unsigned int reinnehmen, der dringend
 * nötig wird für edges, können wir uns die
 * Shortcut-Sctruct komplett sparen;
 * und type und "dringend notwendige zusätzl. information"
 * als IDs für die beiden abgekürzten Kanten nutzen
 */
struct Shortcut : public Edge {
	Shortcut() : Edge() {
		edge_s = 0;
		edge_t = 0;
	}
	unsigned int edge_s;
	unsigned int edge_t;
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
		typename SList<T>::SListData* end;
	public:
		SListExt(){
			siz = 0;
			end = 0;
		}
		~SListExt(){
			end = 0;
		}

		void push(T d){
			siz++;
			typename SList<T>::SListData* t = new typename SList<T>::SListData;
			if(SList<T>::root == 0){
				SList<T>::root = t;
				SList<T>::root->next = 0;
				SList<T>::root->data = d;
				end = SList<T>::root;
			} else {
				t->next = 0;
				t->data = d;
				end->next = t;
				end = t;
			}
			t = 0;
		}
		T pop(){
			siz--;
			T d = T();
			if(SList<T>::root != 0){
				typename SList<T>::SListData* t = SList<T>::root->next;
				if(t == 0)
					end = 0;
				d = SList<T>::root->data;
				delete SList<T>::root;
				SList<T>::root = t;
			}// wenn Liste leer,
			// gibt es für T hoffentlich
			// sinnvole default belegungen
			return d;
		}
		void clear(){
			siz = 0;
			end = 0;
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
				T& getNext(){
					typename SList<T>::SListData* t = position;
					position = position->next;
					return t->data;
				}
		};
		Iterator getIterator(){
			Iterator i = Iterator(this->root);
			return i;
		}
};

#endif
