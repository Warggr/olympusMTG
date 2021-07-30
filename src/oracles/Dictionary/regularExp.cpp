#ifndef WARGGRS_OLYMPIC_DICTIONARY_H
#include "lib1_dictionary.h"
#endif

template<typename T>
SearchTreeNode<T>::~SearchTreeNode(){
	delete[] nodename;
	for(int i=0; i<27; i++)
		if(children[i])
			delete children[i];
}

template<typename T>
Dictionary_tpl<T>::Dictionary_tpl(int nbinserts, const char* const* inserts): root(0), is_a_copy(false) {
	for(int i=0; i<nbinserts; i++){
		int j;
		for(j=0; inserts[i][j] != 0; j++);
		insert(inserts[i], j, i);
	}
}

template<typename T>
SearchTreeNode<T>::SearchTreeNode(const char* name, int namelength, T id): identifier(id){ //makes a deep copy of name
	nnamelen = namelength;
	nodename = new char[nnamelen+1];
	for(int i=0; i<nnamelen; i++) nodename[i] = name[i];
	nodename[nnamelen] = 0;
	for(auto & i : children) i = 0;
}

template<typename T>
SearchTreeNode<T>* SearchTreeNode<T>::existing_append(const char* name, int namelen, T id){
	char tmp[20]; //common etymologic root of both words
	int x = 0; //position of the first divergence (we know the first letter is the same)
	while(true){
		//if((name[x] < '_' || name[x] > 'z') && name[x] != 0) throw std::invalid_argument(name[x] + " is invalid: Keys should consists of lowercase letters, ` or _");
		//std::cout << "COMPARING " << name[x] << " with " << nodename[x] << std::endl;
		if(name[x] == 0 && nodename[x] == 0){ //if we found the exact word in the tree
			identifier = id; //the placeholder node becomes an answer node
			return this;
		}
		else if(nodename[x] == 0){ //new node is a child of old node
			int pos = name[x] - '_';
			append(&(children[pos]), name+x+1, namelen-x-1, id); //full name, minus common characters, minus one character which is given by the position in the tree
			return this;
		}
		else if(name[x] == 0){ //old node is a child of new node
			char* this_new_name = new char[nnamelen-x]; //same: full minus common chars minus one, plus one extra character for the NULL
			for(int i=x+1; nodename[i]!=0; i++){ //the new name is starting at position x+1
				this_new_name[i-x-1] = nodename[i];
			}
			char last_char_of_old_name = nodename[x];
			this_new_name[nnamelen-x-1] = 0;
			delete[] nodename;
			nodename = this_new_name;

			auto* new_child = new SearchTreeNode(name, namelen, id);
			new_child->set_child(last_char_of_old_name - '_', this);
			return new_child;
		}
		else if(name[x] != nodename[x]){ //both nodes are siblings
			//std::cout << nodename << " and " << name << " are siblings!\n";
			auto* new_child = new SearchTreeNode(name+x+1, namelen-x-1, id);

			char last_char_of_old_name = nodename[x]; //the last differenciating char will be lost when updating the node's name,
			//since it will be considered to be included in the position of the node. So we must save it before that.
			char* this_new_name = new char[nnamelen-x]; //full name minus common chars minus one (given by pos in children) plus one for NULL
			for(int i = x+1; nodename[i] != 0; i++){
				this_new_name[i-x-1] = nodename[i];
			}
			nnamelen -= x + 1; //x chars are in common word root and one is guessed from position
			this_new_name[nnamelen] = 0;
			delete[] nodename;
			nodename = this_new_name;

			auto* common_parent = new SearchTreeNode(tmp, x, -1);
			common_parent->set_child(name[x] - '_', new_child);
			common_parent->set_child(last_char_of_old_name - '_', this);

			return common_parent;
		}
		else{
			tmp[x] = name[x]; //extending common word root
			x++;
		}
	}
}

template<typename T>
void SearchTreeNode<T>::append(SearchTreeNode<T>** root, const char* name, int namelen, T id){
	//As a user, you can ignore the return value. This method will: return this; when used on the tree root
	//You should also be aware that -1 is used for values which are not in the table. Thus giving your key the id -1 is not a good idea.
	if(*root == 0){
		auto* new_child = new SearchTreeNode(name, namelen, id);
		*root = new_child;
	}
	else{
		*root = (*root)->existing_append(name, namelen, id);
	}
}

template<typename T>
dict_iterator_tpl<T> SearchTreeNode<T>::find(const char* key) const {
	int i = 0;
	while(true){
		if(nodename[i] == 0 && key[i] == 0){ //values equal; key found!
			return dict_iterator_tpl<T>(identifier);
		}
		else if(nodename[i] == 0){ //need to go deeper
			int pos = key[i] - '_';
			if(pos < 0 || pos >= 28) return Dictionary_tpl<T>::not_found; //invalid character
			if(children[pos] == 0) return Dictionary_tpl<T>::not_found; //value not found
			else return children[pos]->find(key+i+1); //recursive call with the last part of the key
		}
		else if(key[i] != nodename[i]){ //disagreement on a value
			return Dictionary_tpl<T>::not_found;
		}
		i++;
	}
}

template<typename T>
void SearchTreeNode<T>::show(int depth){
	for(int i=0; i<depth; i++) std::cout << " ";
	for(int i=0; i<nnamelen; i++) std::cout << nodename[i];
	if(nodename[nnamelen] != 0) std::cout << "[!error: not NULL-terminated or wrong length]";
	else std::cout << "0";
	std::cout << "(with value " << identifier << ")" << std::endl;
	for(auto & i : children){
		if(i != 0) i->show(depth+1);
	}
}
