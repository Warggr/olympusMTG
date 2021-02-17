#include "lib1_dictionary.h"

SearchTreeNode::~SearchTreeNode(){
	delete[] nodename;
	for(int i=0; i<27; i++)
		if(children[i])
			delete children[i];
}

Dictionary::Dictionary(int nbinserts, const char* const* inserts): root(0), is_a_copy(false){
	for(int i=0; i<nbinserts; i++){
		int j;
		for(j=0; inserts[i][j] != 0; j++);
		insert(inserts[i], j, i);
	}
}

SearchTreeNode::SearchTreeNode(const char* name, int namelength, int id): identifier(id){ //makes a deep copy of name
	nnamelen = namelength;
	nodename = new char[nnamelen+1];
	for(int i=0; i<nnamelen; i++) nodename[i] = name[i];
	nodename[nnamelen] = 0;
	for(int i=0; i<28; i++) children[i] = 0;
}

SearchTreeNode* SearchTreeNode::existing_append(const char* name, int namelen, int id){
	char tmp[20]; //common etymologic root of both words
	int x = 0; //position of the first divergence (we know the first letter is the same)
	while(1){
		//if((name[x] < '_' || name[x] > 'z') && name[x] != 0) throw std::invalid_argument(name[x] + " is invalid: Keys should consists of lowercase letters, ` or _");
		//std::cout << "COMPARING " << name[x] << " with " << nodename[x] << std::endl;
		if(name[x] == 0 && nodename[x] == 0){ //if we found the exact word in the tree
			identifier = id; //the placeholder node becomes an answer node
			return this;
		}
		else if(nodename[x] == 0){ //new node is a child of old node
			char pos = name[x] - '_';
			append(&(children[(int) pos]), name+x+1, namelen-x-1, id); //full name, minus common characters, minus one character which is given by the position in the tree
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

			SearchTreeNode* new_child = new SearchTreeNode(name, namelen, id);
			new_child->set_child(last_char_of_old_name - '_', this);
			return new_child;
		}
		else if(name[x] != nodename[x]){ //both nodes are siblings
			//std::cout << nodename << " and " << name << " are siblings!\n";
			SearchTreeNode* new_child = new SearchTreeNode(name+x+1, namelen-x-1, id);

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

			SearchTreeNode* common_parent = new SearchTreeNode(tmp, x, -1);
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

void SearchTreeNode::append(SearchTreeNode** root, const char* name, int namelen, int id){
	//As a user, you can ignore the return value. This method will: return this; when used on the tree root
	//You should also be aware that -1 is used for values which are not in the table. Thus giving your key the id -1 is not a good idea.
	if(*root == 0){
		SearchTreeNode* new_child = new SearchTreeNode(name, namelen, id);
		*root = new_child;
	}
	else{
		*root = (*root)->existing_append(name, namelen, id);
	}
}

int SearchTreeNode::find(const char* key) const{
	int i = 0;
	while(1){
		if(nodename[i] == 0 && key[i] == 0){ //values equal; key found!
			return identifier;
		}
		else if(nodename[i] == 0){ //need to go deeper
			char pos = key[i] - '_';
			if(pos < 0 || pos >= 28) return -1; //invalid character
			if(children[(int) pos] == 0) return -1; //value not found
			else return children[(int) pos]->find(key+i+1); //recursive call with the last part of the key
		}
		else if(key[i] != nodename[i]){ //disagreement on a value
			return -1;
		}
		i++;
	}
}

void SearchTreeNode::show(int depth){
	for(int i=0; i<depth; i++) std::cout << " ";
	for(int i=0; i<nnamelen; i++) std::cout << nodename[i];
	if(nodename[nnamelen] != 0) std::cout << "[!error: not NULL-terminated or wrong length]";
	else std::cout << "0";
	std::cout << "(with value " << identifier << ")" << std::endl;
	for(int i=0; i<28; i++){
		if(children[i] != 0) children[i]->show(depth+1);
	}
}