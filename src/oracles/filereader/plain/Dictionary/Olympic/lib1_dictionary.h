#ifndef WARGGRS_OLYMPIC_DICTIONARY_H
#define WARGGRS_OLYMPIC_DICTIONARY_H

#include <iostream>

template<typename T>
struct dict_iterator_tpl {
    const T* value;
    constexpr explicit dict_iterator_tpl(const T* value): value(value) {};
    constexpr explicit dict_iterator_tpl(const T& value): value(&value) {};
    const T& operator*() const { return *value; }
    bool operator==(const dict_iterator_tpl<T>& other) const { return value == other.value; }
};

template<typename T>
class SearchTreeNode{
private:
	T identifier;
	bool valid;
	uint nnamelen; //length of the (partial) name, without ending NULL
	char* nodename;
	SearchTreeNode* children[28]{nullptr};

	SearchTreeNode* existing_append(const char* name, int namelen, T id);
	SearchTreeNode(const char* name, int namelen, T id): identifier(id) { SearchTreeNode<T>(name, namelen, true); }
	SearchTreeNode(const char* name, int namelen, bool valid);
public:
	~SearchTreeNode();

	static void append(SearchTreeNode*& root, const char* name, int namelen, T id);
	dict_iterator_tpl<T> find(const char* key) const;

	void show(int depth) const;
};

template<typename T>
class Dictionary_tpl {
protected:
	SearchTreeNode<T>* root {nullptr};
	bool is_a_copy;
public:
    typedef dict_iterator_tpl<T> iterator;
    static constexpr iterator not_found {nullptr};

	Dictionary_tpl(): is_a_copy(false){};
	Dictionary_tpl(int nbinserts, const char* const * inserts);
	Dictionary_tpl(Dictionary_tpl const& copy): root(copy.root), is_a_copy(true){};
	~Dictionary_tpl(){ if(root && !is_a_copy) delete root; };

	iterator find(const char* key) const {
		if(root == nullptr) return not_found;
		else return root->find(key);
	};

	void insert(const char* key, int namelen, T id){
		SearchTreeNode<T>::append(root, key, namelen, id);
//		show();
	};

	void show() const { if(root) root->show(0); else std::cout << "No values!" << std::endl; };
};

using Dictionary = Dictionary_tpl<int>;

template<typename T>
constexpr dict_iterator_tpl<T> Dictionary_tpl<T>::not_found;

#include "regularExp.cpp"

#endif //WARGGRS_OLYMPIC_DICTIONARY_H
