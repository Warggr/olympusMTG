#include "../HFiles/classes.h"
#include <cstdlib>

void disp_mana(Mana m, int x, int y){
	char generic = (char) (m&0xf);
	for(int i=0; i<6; i++){
		m = m >> 4;
		for(int j=0; j < (m&0xf); j++){
			x -= 24;
			generic--;
			al_draw_scaled_bitmap(god.images->ManaColorSym[i], 0, 0, 160, 160, x, y, 24, 24, 0);
		}
	}
	if(generic!=0){
		al_draw_scaled_bitmap(god.images->ManaNumSym[(int) generic], 0, 0, 160, 160, x-24, y, 24, 24, 0);
	}
}

void Card::poster(){
	ALLEGRO_BITMAP* cardfront = god.images->card_template[(int) color];
	al_draw_bitmap(cardfront, LEFTBAR_Y + BOARD_Y, STACK_X, 0);
	const char* nametypes[] = {"Instant", "Land", "Artifact", "Planeswalker", "Creature", "Sorcery"};
	
	al_draw_text(god.font, al_map_rgb(0, 0, 0), LEFTBAR_Y + BOARD_Y + 29, STACK_X + 28, 0, &(name[0]));
	disp_mana(cost, LEFTBAR_Y + BOARD_Y + RIGHTBAR_Y - 28, STACK_X + 30);
	al_draw_text(god.font, al_map_rgb(0, 0, 0), LEFTBAR_Y + BOARD_Y + 29, STACK_X + 295, 0, nametypes[(int) type]);
	if(type == 1){
		al_draw_bitmap(god.images->watermarks[(int) color], LEFTBAR_Y+BOARD_Y+ 120, STACK_X + 340, 0);
	}
	if(fab) al_draw_text(god.font, al_map_rgb(0, 0, 0), LEFTBAR_Y + BOARD_Y +29, STACK_X + 360, 0, &(fab->describe(name)[0]));
	for(int i=0; i<nb_actabs; i++){
		al_draw_text(god.font, al_map_rgb(0, 0, 0), LEFTBAR_Y + BOARD_Y +29, STACK_X + 360 + 15*i, 0, &(first_actab[i].describe(name)[0]));
	}
	int offset = 0;
	#define BOTTOMRIGHTOFFSETX 60
	#define BOTTOMRIGHTOFFSETY 85
	if(type == 4){
		al_draw_bitmap(god.images->pt_box[(int) color], LEFTBAR_Y+BOARD_Y+RIGHTBAR_Y-BOTTOMRIGHTOFFSETY, STACK_X+FULLCARD_X-BOTTOMRIGHTOFFSETX, 0);
		char x[10]; sprintf(x, "%d/%d", flavor_text[0], flavor_text[1]);
		al_draw_text(god.font, al_map_rgb(0, 0, 0), LEFTBAR_Y+BOARD_Y+RIGHTBAR_Y-BOTTOMRIGHTOFFSETY+20, STACK_X+FULLCARD_X-BOTTOMRIGHTOFFSETX + 5, 0, x);
		offset = 2;
	}
	else if(type == 3){
		al_draw_bitmap(god.images->loyalty, LEFTBAR_Y+BOARD_Y+RIGHTBAR_Y-BOTTOMRIGHTOFFSETY, STACK_X+FULLCARD_X-BOTTOMRIGHTOFFSETX, 0);
		char x[3]; sprintf(x, "%d", flavor_text[0]); 
		al_draw_text(god.font, al_map_rgb(255, 255, 255), LEFTBAR_Y+BOARD_Y+RIGHTBAR_Y-BOTTOMRIGHTOFFSETY + 30, STACK_X+FULLCARD_X-BOTTOMRIGHTOFFSETX + 8, 0, x);
		offset = 1;
	}
	if(flavor_text && flavor_text[offset]) al_draw_text(god.font, al_map_rgb(0, 0, 0), LEFTBAR_Y + BOARD_Y +29, STACK_X + 420, 0, flavor_text + offset);
	#undef BOTTOMRIGHTOFFSET
}

void Permanent::disp(int x, int y, bool highlight){
	x = LEFTBAR_Y + CARD_Y*x;
	y = ctrl->shown_depth + ctrl->shown_direction*(y+1)*CARD_X;
	if(source->get_type() == 1){
		al_draw_scaled_bitmap(god.images->basiclands, 384*(color-1), (500-384)/2, 384, 384, x, y, CARD_X, CARD_Y, 0);
	}
	else al_draw_filled_rectangle(x, y, x+CARD_X, y+CARD_Y, al_map_rgb(100, 100, 100));
	if(highlight){
		al_draw_rectangle(x+1, y+1, x+CARD_X-1, y+CARD_Y-1, al_map_rgb(255, 100, 0), 2);
		source->poster();
	}
	if(!(flags&1)) al_draw_bitmap(god.images->tapsymbol, x, y, 0);
	al_draw_text(god.font, al_map_rgb(0, 0, 0), x, y, 0, name);
}

void Creature::disp(int x, int y, bool highlight){
	Permanent::disp(x, y, highlight);
	//mvwprintw(win, 2, y + 7, "(%d/%d)", power, toughness);*/
}

void SpellOption::disp(int pos, bool highlight, bool castable){
	//if(castable) wattron(win, A_BOLD);
	std::string tmp = source->describe();
	ALLEGRO_COLOR clr;
	if(highlight) clr = al_map_rgb(255, 0, 0);
	else clr = al_map_rgb(255, 255, 255);
	al_draw_text(god.font, clr, 0, (LOGLEN + pos)*10, 0, &(tmp[0]));
	if(highlight) source->poster();
}

std::string Card::describe(){
	std::string ret = name;
	if(type != 1){
		ret += "  [" + m2t(cost) + "]";
	}
	return ret;
}

std::string PermOption::describe(std::string cardname){
	std::string ret = "[" + m2t(cost) + "]";
	if(tapsymbol){
		ret += ", [T]";
	}
	ret += ": ";
	ret += fab->describe(cardname);
	return ret;
}

void Permanent::describe(char* tmp){
	//wprintw(w, "%s", name);
}

void Creature::describe(char* tmp){
	//wprintw(w, "%s: %d/%d", name, power, toughness);
}

std::string Ability::describe(std::string cardname){
	std::string descriptions[NB_DIFFERENT_ABILITIES];
	descriptions[0] = "~ deals ? damage to target opponent";
	descriptions[1] = "Target player draws ? cards";
	descriptions[2] = "Target player gains ? life";
	descriptions[3] = "Your life total becomes ?";
	descriptions[4] = "Add one mana of color ?";
	descriptions[5] = "Destroy something";
	descriptions[6] = "Counter target spell or ability";
	std::string ds = descriptions[(int) type];
	std::string ret;
	for(unsigned int i=0; i<ds.length(); i++){
		if(ds[i] == '~') ret += cardname;
		else if(ds[i] == '?') ret += std::to_string(param2);
		else ret += ds[i];
	}
	ret += ".";
	if(next){
		ret += " " + next->describe(cardname);
	}
	return ret;
}