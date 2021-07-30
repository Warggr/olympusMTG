#include "server/gameplay/3player.h"
#include "server/gameplay/4permanents.h"
#include "../HFiles/headR_readfiles.h"
#include "options/8options.h"
#include "oracles/classes/2triggers.h"
#include "../HFiles/12abilities.h"
#include "headU_utilities.h"

#include <memory>

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
