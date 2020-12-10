#ifndef OLYMPUS_HEAD_3_READFILES
#define OLYMPUS_HEAD_3_READFILES

void check_safepoint(std::ifstream& myfile, char c, const char* message);
void check_safepoint(std::ifstream& myfile, char c, const char* message);

void raise_error(std::string message);

bool read_section_onresolve(std::ifstream& myfile, PreResolvable** preRes);
bool read_section_abils(std::ifstream& myfile, int* nb_actabs, PermOption** first_actab);
bool read_section_flavor(std::ifstream& myfile, char** flavor_text, char offset_text);
bool read_section_triggers(std::ifstream& myfile, int* nb_triggers, char** typeof_triggers, Trigger** first_trigger);

char read_typeof_target(const char* txt);
//player 80 - permanent 40 - spell (well, resolvable) 20 - creature 10 - planeswalker 08 - ? - card 02 - ?

int read_ability_type(std::ifstream& myfile);
char read_ability_parameter(std::ifstream& myfile, char* allassignedvariables, int* nbassignedparams);

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

#endif //OLYMPUS_HEADER_9_READFILES