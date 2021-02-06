#include ".header_link.h"
#include "../HFiles/head3_readfiles.h"
#include "../HFiles/8options.h"
#include "../HFiles/10triggers.h"
#include "head1_utilities.h"

void set_canary(char canary, std::ofstream& bFile){
	bFile.write(&canary, sizeof(char));
}

void check_canary(char canary, std::ifstream& bFile){
	char test;
	bFile.read(&test, sizeof(char));
	if(test != canary) raise_error("Canary error while reading binary file");
	//else std::cout << "Canary test for canary " << canary << "passed!" << std::endl;
}

void PreResolvable::write_binary(std::ofstream& bFile) const {
	set_canary('b', bFile);
	fab->write_binary(bFile);
	bFile.write(&nb_parameters, sizeof(char));
	bFile.write(parameters, nb_parameters*sizeof(char));
	set_canary('y', bFile);
}

PreResolvable::PreResolvable(std::ifstream& bFile){
	check_canary('b', bFile);
	fab = std::make_unique<Ability>(bFile);
	bFile.read(&nb_parameters, sizeof(char));
	if(nb_parameters < 0) raise_error("Found negative number of parameters in PreRes binary");
	parameters = new char[nb_parameters];
	bFile.read(parameters, nb_parameters*sizeof(char));
	check_canary('y', bFile);
}

void Ability::write_binary(std::ofstream& bFile) const {
	set_canary('c', bFile);
	bFile.write(&type, 3*sizeof(char)); //copying everything except the unique_ptr
	if(next != nullptr){
		char x = 1;
		bFile.write(&x, sizeof(char));
		next->write_binary(bFile);
	}
	else{
		char x = 0;
		bFile.write(&x, sizeof(char));
	}
	set_canary('d', bFile);
}

Ability::Ability(std::ifstream& bFile){
	check_canary('c', bFile);
	bFile.read(&type, 3*sizeof(char));
	char x; bFile.read(&x, sizeof(char));
	if(x != 0) next = std::make_unique<Ability>(bFile);
	check_canary('d', bFile);
}

void Trigger::write_binary(std::ofstream& bFile) const {
	set_canary('g', bFile);
	effects->write_binary(bFile);
	set_canary('h', bFile);
}

void Trigger::read_binary(std::ifstream& bFile){
	check_canary('g', bFile);
	effects = new PreResolvable(bFile);
	check_canary('h', bFile);
}

void CardZone::init(std::ifstream& bFile){
	check_canary('o', bFile);
	size = 0;
	while(true){
		char nb_cards;
		bFile.read(&nb_cards, sizeof(char));
		if(nb_cards == 0) break; //0 cards means end of file
		std::shared_ptr<CardOracle> cardrules = std::make_shared<CardOracle>(bFile);
		for(char i=0; i<nb_cards; i++){
			Card* first = new Card(cardrules);
			cards.push_front(first);
			size++;
		}
		god.gdebug(DBG_X_READFILE) << nb_cards << "cards named" << cards.front()->get_name() << "created" << std::endl;
	}
	check_canary('m', bFile);
}

void CardOracle::write_binary(std::ofstream& bFile) const {
	set_canary('a', bFile);
	char namesize = (char) name.size();
	bFile.write(&namesize, sizeof(char));
	bFile.write(&(name[0]), namesize * sizeof(char));

	bFile.write((char*) &cost, sizeof(Mana));
	bFile.write((char*) &type, 2*sizeof(char)); //writing type and color
	char x = 0;
	if(on_cast){
		x = 1;
		bFile.write(&x, sizeof(char));
		on_cast->write_binary(bFile);
	} else {
		bFile.write(&x, sizeof(char));
	}
	bFile.write((char*) &nb_actabs, sizeof(int));
	for(int i = 0; i<nb_actabs; i++){
		first_actab[i].write_binary(bFile);
	}
	bFile.write((char*) &nb_triggers, sizeof(int));
	bFile.write(typeof_triggers, nb_triggers*sizeof(char));
	for(int i=0; i<nb_triggers; i++){
		triggers[i].write_binary(bFile);
	}
	if(flavor_text == nullptr){
		int y = 0;
		bFile.write((char*) &y, sizeof(int));
	} else {
		int i;
		for(i = 0; flavor_text[i] != '\0'; i++);
		bFile.write((char*) &i, sizeof(int));
		bFile.write(flavor_text, (i+1) * sizeof(char));
	}
	set_canary('z', bFile);
}

CardOracle::CardOracle(std::ifstream& bFile){
	check_canary('a', bFile);
	char namesize;
	bFile.read(&namesize, sizeof(char));
	if(namesize >= 100) raise_error("Read a name 100 chars long, this must be an error.");
	char name_tmp[100];
	bFile.read(name_tmp, namesize * sizeof(char));
	name_tmp[(int) namesize] = 0;
	name = std::string(name_tmp);

	bFile.read((char*) &cost, sizeof(Mana));
	bFile.read((char*) &type, 2*sizeof(char)); //writing type and color
	char x;
	bFile.read(&x, sizeof(char));
	if(x == 1){
		on_cast = new PreResolvable(bFile);
	} else {
		on_cast = nullptr;
	}
	bFile.read((char*) &nb_actabs, sizeof(int));
	if(nb_actabs){
		first_actab = new PermOption[nb_actabs];
		for(int i = 0; i<nb_actabs; i++){
			first_actab[i].read_binary(bFile);
		}
	} else {
		first_actab = nullptr;
	}
	bFile.read((char*) &nb_triggers, sizeof(int));
	if(nb_triggers){
		typeof_triggers = new char[nb_triggers];
		triggers = new Trigger[nb_triggers];
		bFile.read(typeof_triggers, nb_triggers*sizeof(char));
		for(int i=0; i<nb_triggers; i++){
			triggers[i].read_binary(bFile);
		}
	} else {
		typeof_triggers = nullptr;
		triggers = nullptr;
	}
	int y;
	bFile.read((char*) &y, sizeof(int));
	if(y == 0){
		flavor_text = nullptr;
	} else {
		flavor_text = new char[y + 1];
		bFile.read(flavor_text, (y+1) * sizeof(char));
	}
	check_canary('z', bFile);
}