#ifndef WARGGRS_OLYMPIC_DICTIONARY_H
#define WARGGRS_OLYMPIC_DICTIONARY_H

#include <iostream>

class SearchTreeNode{
private:
	int identifier;
	int nnamelen; //length of the (partial) name, without ending NULL
	char* nodename;
	struct SearchTreeNode* children[28];

	void set_child(int pos, SearchTreeNode* child){
		children[pos] = child;
	};
	SearchTreeNode* existing_append(const char* name, int namelen, int id);
	SearchTreeNode(const char* name, int namelen, int id);
public:
	~SearchTreeNode();

	static void append(SearchTreeNode** root, const char* name, int namelen, int id);
	int find(const char* key) const;

	void show(int depth);
};

class Dictionary{
protected:
	SearchTreeNode* root;
	bool is_a_copy;
public:
	Dictionary(): root(0), is_a_copy(false){};
	Dictionary(int nbinserts, const char* const * inserts);
	Dictionary(Dictionary const& copy): root(copy.root), is_a_copy(true){};
	~Dictionary(){if(root && !is_a_copy) delete root; };

	int find(const char* key) const{
		if(root == 0) return -1;
		else return root->find(key);
	};

	void insert(const char* key, int namelen, int id){
		SearchTreeNode::append(&root, key, namelen, id);
	};

	void show() const {if(root) root->show(0); else std::cout << "No values!" << std::endl; };
};

#endif //WARGGRS_OLYMPIC_DICTIONARY_H