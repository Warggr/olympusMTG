#include "../HFiles/olympus_main.h"

void Allegro_io::disp_mana(const Mana m, const int endy, const int topz){
	char generic = (char) (m&0xf);
	for(int i=0; i<6; i++){
		m = m >> 4;
		for(int j=0; j < (m&0xf); j++){
			endy -= 24;
			generic--;
			al_draw_scaled_bitmap(images->ManaColorSym[i], 0, 0, 160, 160, endy, topz, 24, 24, 0);
		}
	}
	if(generic!=0){
		al_draw_scaled_bitmap(images->ManaNumSym[(int) generic], 0, 0, 160, 160, endy-24, topz, 24, 24, 0);
	}
}

void Allegro_io::poster(const int y, const int z,
	const std::string name, const Mana manacost, const char color, const std::string types,
	const int nb_lines, const std::string* lines, const int power, const char frametype, const int toughness, const bool watermark){
	al_draw_bitmap(card_template[(int) color], z, y, 0);
	al_draw_text(font, al_map_rgb(0,0,0), y+29, z+28, 0, &(name[0]));
	disp_mana(cost, y - 28, z + 30);
	al_draw_text(font, al_map_rgb(0,0,0), y+29, z+295, 0, &(types[0]));
	if(watermark){
		al_draw_bitmap(watermarks[(int) color], y + 120, z + 340, 0);
	}
	for(int i=0; i<nb_lines; i++){
		al_draw_text(font, al_map_rgb(0,0,0), y+29, z+360+ i*15);
	}
	const int yOffset = 60;
	const int zOffset = 85;
	if(frametype == 1){
		al_draw_bitmap(pt_box[(int) color], y+fullcardY-yOffset, z+fullcardZ-zOffset, 0);
		char x[10]; sprintf(x, "%d/%d", power, toughness);
		al_draw_text(font, al_map_rgb(0, 0, 0), y+fullcardY-yOffset + 20, z+fullcardZ-zOffset + 5, 0, x);
	}
	else if(frametype == 2){
		al_draw_bitmap(loyalty, y+fullcardY-yOffset, z+fullcardZ-zOffset, 0);
		char x[3]; sprintf(x, "%d", power);
		al_draw_text(font, al_map_rgb(255, 255, 255), y+fullcardY-yOffset + 30, z+fullcardZ-zOffset + 8, 0, x);
	}
}

void Card::poster(){	
	const char* nametypes[] = {"Instant", "Land", "Artifact", "Planeswalker", "Creature", "Sorcery"};
	
	std::string all_text[20];
	int nb_textlines = 0;

	if(on_cast) all_text[nb_textlines++] = on_cast->describe(name);
	for(int i=0; i<nb_actabs; i++){
		all_text[nb_textlines++] = first_actab[i].describe(name);
	}
	for(int i=0; i<nb_triggers; i++){
		all_text[nb_textlines++] = triggers[i].describe((int) typeof_triggers[i], name);
	}
	int power = 0, toughness = 0, frametype = 0;
	if(type == 2){
		offset = 2;
		frametype = 1;
	}
	else if(type == 3){
		offset = 1;
		frametype = 2;
	}
	if(flavor_text && flavor_text[offset]) all_text[nb_textlines++] = flavor_text + offset;
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
	al_draw_text(god.font, al_map_rgb(0, 0, 0), x, y, 0, name.c_str());
}

void Creature::disp(int x, int y, bool highlight, bool displace){
	Permanent::disp(x, y, highlight);
	y = ctrl->shown_depth + ctrl->shown_direction*(y+1)*CARD_X;
	if(displace){
		int oldx = 7-x;
		al_draw_filled_rectangle(oldx*CARD_Y + LEFTBAR_Y, y, (oldx+1)*CARD_Y + LEFTBAR_Y, y+CARD_Y, al_map_rgb(255, 255, 255));
	}
	x = LEFTBAR_Y + CARD_Y*x;
	char pt[6]; sprintf(pt, "%d/%d", power, toughness);
	al_draw_text(god.font, al_map_rgb(0, 0, 0), x+CARD_X -28, y+CARD_Y-20, 0, pt);
}

void Allegro_io::print_text(std::string const text, const char color, const float x, const float y){
	al_draw_text(mainFont, registeredColors[(int) color], x, y, 0, text);
}

void must_init(bool test, const char *description){
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

/*void Ncurses_io::Ncurses_io(){
	int row, col;
	initscr();
	noecho();
	cbreak();
	start_color();
	init_color(COLOR_BLACK, 100, 100, 100);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_BLUE, COLOR_BLACK);
	init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(5, COLOR_RED, COLOR_BLACK);
	init_pair(6, COLOR_GREEN, COLOR_BLACK);
	init_pair(7, COLOR_BLACK, COLOR_YELLOW);

	getmaxyx(stdscr, row, col);
	if(row < LOGLEN+IO_X || row < STACK_X+FULLCARD_X || row<2*BOARD_X || col < RIGHTBAR_Y + LEFTBAR_Y + BOARD_Y){
		deletegraph();
		printf("Terminal needs dimensions %dx%d - possibly higher!\n", STACK_X+FULLCARD_X, RIGHTBAR_Y + LEFTBAR_Y + BOARD_Y);
		exit(1);
	}
	curs_set(FALSE);
	winzones[0] = newwin(LOGLEN, LEFTBAR_Y, 0, 0);
	winzones[1] = newwin(STACK_X, RIGHTBAR_Y, 0, LEFTBAR_Y+BOARD_Y+1);
	winzones[2] = newwin(IO_X, LEFTBAR_Y, LOGLEN+2, 0);
	winzones[3] = newwin(FULLCARD_X, RIGHTBAR_Y, STACK_X + 1, LEFTBAR_Y+BOARD_Y+1);
}

Ncurses_io::~Ncurses_io(){
	al_destroy_font(god.font);
    al_destroy_display(god.window);
    al_destroy_timer(god.timer);
    al_destroy_event_queue(god.queue);

	for(int i=0; i<16; i++) al_destroy_bitmap(ManaNumSym[i]);
	for(int i=0; i<5; i++) al_destroy_bitmap(ManaColorSym[i]);
	for(int i=0; i<7; i++) al_destroy_bitmap(card_template[i]);
	al_destroy_bitmap(wallpaper);
	al_destroy_bitmap(card_back);
	al_destroy_bitmap(tapsymbol);
}*/