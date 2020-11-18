#ifndef OLYMPUS_CLASSES_CARDS_3_H
#define OLYMPUS_CLASSES_CARDS_3_H

class Card: virtual public Target{
private:
	char name[30];
	Mana cost;
	char type; //0 Instant, 1 Land, 2 ArtiEnch, 3 PW, 4 Creature, 5 Sorcery
	char color; //0 Colorless, 1-5 WUBRG, 6 Multicolor
	Ability* fab; //fun fact: all Ancestral Recalls have a pointer to the same 'draw three cards' ability. I hope that won't cause bugs, but I can't see how
	char nb_parameters;
	char* parameters; //that will also be shared from all Ancestral Recalls: they will have a pointer to the same list [3].
	int nb_actabs;
	PermOption* first_actab;
	char* flavor_text; //First, that will be P/T or starting loyalty. Else that's everything we haven't implemented yet. Triggered or static abilities go into that category until we implement them.
	//In the end, it will only be flavor text, once everything will have reAchEd BlEsSed PeRFEcTIoN...
	//BeHOLD OUR GREAT WORK... ALL WILL BE ALL WILL ALL WILL BE ONE LONG LIVE PHYREXIA WSHHSHHHYRXXXHHYWWWYXRIA
public:
	Card* next;

	Card(char n[], Mana c, char t, Ability* fa, char nb_params, char* params, int nb_act, PermOption* first_act, char* flavor, Card* ptr);
	~Card(){};
	void ragnarok();
	std::string describe();
	void poster();
	char get_type() {return type; };
	Mana get_cost(){return cost; };
	std::string get_name() {return name; };
	char get_color(){return color; };
	void eject() {next = 0; };
	Ability* get_ability(){return fab; };
	void getpt(int* pow, int* tou);
	char get_nb_params(){return nb_parameters; };
	char* get_parameters(){return parameters; };
	void get_permabs(PermOption** pr, int* nb_opts){*pr = first_actab; *nb_opts = nb_actabs; };
	char* get_flavor_text(){return flavor_text; };
	char target_type(){return 0x02; };
};

class CardZone{
private:
	char name[10];
	int size;
	Card* first;
public:
	~CardZone();
	void init(FILE* myfile);
	void init_name(const char* n){ size=0; first=0; for(int i=0; i<10; i++) name[i] = n[i]; };
	void shuffle();
	int drawto(CardZone* target, int nb_cards);
	void inc_size(int i){ size += i; };
	void describe(char* tmp);
	void disp(int x, int y);
	void set_first(Card* f){first = f; };
	Card* get_first(){return first; };
};

#endif //OLYMPUS_CLASSES_CARDS_3_H