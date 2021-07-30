#ifndef WARGGRS_OLYMPIC_DICTIONARY_H
#define WARGGRS_OLYMPIC_DICTIONARY_H

#include <iostream>

template<typename T>
struct dict_iterator_tpl {
    const T* value;
    constexpr explicit dict_iterator_tpl(const T* value): value(value) {};
    constexpr explicit dict_iterator_tpl(const T& value): value(&value) {};
    const T& operator*() { return *value; }
    bool operator==(const dict_iterator_tpl<T>& other) { return value == other.value; }
};

template<typename T>
class SearchTreeNode{
private:
	T identifier;
	uint nnamelen; //length of the (partial) name, without ending NULL
	char* nodename;
	struct SearchTreeNode* children[28];

	void set_child(int pos, SearchTreeNode* child){
		children[pos] = child;
	};
	SearchTreeNode* existing_append(const char* name, int namelen, T id);
	SearchTreeNode(const char* name, int namelen, T id);
public:
	~SearchTreeNode();

	static void append(SearchTreeNode** root, const char* name, int namelen, T id);
	dict_iterator_tpl<T> find(const char* key) const;

	void show(int depth);
};

template<typename T>
class Dictionary_tpl {
protected:
	SearchTreeNode<T>* root;
	bool is_a_copy;
public:
    typedef dict_iterator_tpl<T> iterator;
    static constexpr iterator not_found = iterator(nullptr);

	Dictionary_tpl(): root(nullptr), is_a_copy(false){};
	Dictionary_tpl(int nbinserts, const char* const * inserts);
	Dictionary_tpl(Dictionary_tpl const& copy): root(copy.root), is_a_copy(true){};
	~Dictionary_tpl(){if(root && !is_a_copy) delete root; };

	iterator find(const char* key) const{
		if(root == 0) return not_found;
		else return root->find(key);
	};

	void insert(const char* key, int namelen, int id){
		SearchTreeNode<T>::append(&root, key, namelen, id);
	};

	void show() const { if(root) root->show(0); else std::cout << "No values!" << std::endl; };
};

using Dictionary = Dictionary_tpl<int>;

#include "regularExp.cpp"

#endif //WARGGRS_OLYMPIC_DICTIONARY_H
