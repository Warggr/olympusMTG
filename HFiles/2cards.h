#ifndef OLYMPUS_CLASSES_CARDS_2_H
#define OLYMPUS_CLASSES_CARDS_2_H

#include <memory>
#include <forward_list>

class CardOracle{ //represents an Oracle text. Should never be modified.
private:
	std::string name;
	Mana cost;
	char type; //0 Instant, 1 Land, 2 ArtiEnch, 3 PW, 4 Creature, 5 Sorcery
	char color; //0 Colorless, 1-5 WUBRG, 6 Multicolor
	PreResolvable* on_cast; //fun fact: all Ancestral Recalls have a pointer to the same 'draw three cards' ability. I hope that won't cause bugs, but I can't see how
	int nb_actabs;
	PermOption* first_actab; //as PW (usually) don't have actabs, they will use that field. They'll mask a '+1' effect as an effect that costs [1] and a '-3' effect as one that costs [WWWW].
	int nb_triggers;
	char* typeof_triggers;
	Trigger* triggers;
	char* flavor_text; //First, that will be P/T or starting loyalty. Else that's everything we haven't implemented yet.
	//Static abilities go into that category until we implement them.
	//In the end, it will only be flavor text, once everything will have reAchEd BlEsSed PeRFEcTIoN...
	//BeHOLD OUR GREAT WORK... ALL WILL BE ALL WILL ALL WILL BE ONE LONG LIVE PHYREXIA WSHHSHHHYRXXXHHYWWWYXRIA
public:
	CardOracle(std::ifstream& bFile);
	CardOracle(std::ifstream& myfile, std::string n, Mana c, char t);

	std::string describe() const;
	void poster() const;

	char get_type() const {return type; };
	Mana get_cost() const {return cost; };
	const std::string& get_name() const {return name; };
	const PreResolvable* get_preRes() const {return on_cast; };
	char get_color() const {return color; };
	void getpt(int* pow, int* tou) const ;
	void get_permabs(PermOption** pr, int* nb_opts) const {*pr = first_actab; *nb_opts = nb_actabs; };
	void get_triggers(const char type, TriggerEvent& trigEv) const ;
	const char* get_flavor_text() const {return flavor_text; };
	void write_binary(std::ofstream& bFile) const;
};

class Card: public Target{
	std::shared_ptr<const CardOracle> oracle;
public:
	Card(std::shared_ptr<const CardOracle> orc): oracle(orc) {target_flags = 0x02; };
	std::string describe() const {return oracle->describe(); };
	void poster() const {oracle->poster(); };

	char get_type() const {return oracle->get_type(); };
	Mana get_cost() const {return oracle->get_cost(); };
	const std::string& get_name() const {return oracle->get_name(); };
	const PreResolvable* get_preRes() const {return oracle->get_preRes(); };
	char get_color() const {return oracle->get_color(); };
	void getpt(int* pow, int* tou) const {oracle->getpt(pow, tou); };
	void get_permabs(PermOption** pr, int* nb_opts) const {oracle->get_permabs(pr, nb_opts); };
	void get_triggers(const char type, TriggerEvent& trigEv) const {oracle->get_triggers(type, trigEv); };
	const char* get_flavor_text() const {return oracle->get_flavor_text(); }
	void write_binary(std::ofstream& bFile) const {oracle->write_binary(bFile); }
};

class CardZone{
private:
	char name[10];
	int size;
public:
    CardZone(): name{0}, size(0) {};
	std::forward_list<Card*> cards;

	void init(std::ifstream& textfile, std::ofstream& binaryOut);
	void init(std::ifstream& bFile);
	void init_name(const char* n){ for(int i=0; i<10; i++) name[i] = n[i]; };
	void shuffle();
	int drawto(CardZone* target, int nb_cards);
	void takeonecard(Card* c){size++; cards.push_front(c); };
	void inc_size(int i){ size += i; };
	void describe(char* tmp) const;
	void disp(int x, int y) const;
};

#endif //OLYMPUS_CLASSES_CARDS_3_H