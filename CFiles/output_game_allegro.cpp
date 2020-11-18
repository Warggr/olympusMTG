#include "../HFiles/classes.h"
#include <cstdlib>

void must_init(bool test, const char *description)
{
    if(test) return;

    printf("couldn't initialize %s\n", description);
    exit(1);
}

std::ofstream& externVarContainer::gdebug(char password){
	if(password & wanted_debug) return debug_log;
	else return verbose_debug;
}

void Game::initgraph(){
	must_init(al_init(), "allegro");
	must_init(al_install_keyboard(), "keyboard");
	must_init(al_install_mouse(), "mouse");

	god.queue = al_create_event_queue();
	must_init(god.queue, "queue");

	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
	al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

	god.window = al_create_display(1500, 900);
	must_init(god.window, "display");

	must_init(al_init_font_addon(), "fonts");
	must_init(al_init_ttf_addon(), "TTF");
	god.font = al_load_ttf_font("BelerenBold.ttf", 20, 0);
	must_init(god.font, "font");

	must_init(al_init_primitives_addon(), "primitives");
	must_init(al_init_image_addon(), "images");

	god.images = new ImageHolder;
	al_register_event_source(god.queue, al_get_keyboard_event_source());
	al_register_event_source(god.queue, al_get_display_event_source(god.window));
	al_register_event_source(god.queue, al_get_mouse_event_source());
	al_hide_mouse_cursor(god.window);

	al_draw_bitmap(god.images->wallpaper, 0, 0, 0);
}

void Game::message(const char* mess){
	al_draw_filled_rectangle(30, 0, 500, 25, al_map_rgb(0, 0, 0));
	al_draw_text(god.font, al_map_rgb(255, 255, 255), 30, 0, 0, mess);
}

void Game::disp_log(){
	al_draw_filled_rectangle(0, 0, LEFTBAR_Y, LOGLEN*12, al_map_rgb(0, 0, 0));
	for(int i=0; i<LOGLEN && logbook[i] != 0; i++){
		al_draw_text(god.font, al_map_rgb(255, 255, 255), 0, i*12, 0, logbook[i]);
	}
}

void Game::deletegraph(){
	al_destroy_font(god.font);
    al_destroy_display(god.window);
    al_destroy_timer(god.timer);
    al_destroy_event_queue(god.queue);
}

void Game::disp(){
	al_draw_bitmap(god.images->wallpaper, 0, 0, 0);
	for(int i = 0; i<2; i++){
		players[i]->disp();
	}
	disp_log();
	disp_stack();
	al_flip_display();
}

void Game::disp_stack(){
	al_draw_filled_rectangle(LEFTBAR_Y+BOARD_Y, 0, LEFTBAR_Y+BOARD_Y+CARD_Y, STACK_X, al_map_rgb(0, 0, 0));
	int pos = 0;
	for(Resolvable* iter = stack; iter!=0; iter = iter->next){
		al_draw_text(god.font, al_map_rgb(0, 0, 0), LEFTBAR_Y+BOARD_Y, (pos++)*12, 0, &(iter->describe()[0]));
	}
	al_flip_display();
}

void Game::addtolog(const char* new_entry){
	if(logbook[LOGLEN -1]) delete logbook[LOGLEN -1];
	for(int i = LOGLEN-1; i>0; i--){
		logbook[i] = logbook[i-1];
	}
	char* r = new char[LEFTBAR_Y];
	bool v = true;
	for(int i=0; i<LEFTBAR_Y -1; i++){
		if(v) r[i] = new_entry[i];
		else r[i] = ' ';
		if(r[i] == '\0') v = false;
	}
	r[LEFTBAR_Y] = 0;
	logbook[0] = r;
	disp_log();
}

void Player::disp_header(){
	al_draw_textf(god.font, al_map_rgb(255, 255, 255), LEFTBAR_Y, shown_depth, 0, " <%s> (%d life)", name, life);
	//const char* phases_dcp[] = {"<\\ ", "\\a\\", "u=>", "<=t", ">*<", "/a/", " />"};
	/*char tmp[64];
	wprintw(winzones[0], "%s", m2t(manapool, tmp));*/
	for(int i=0; i<3; i++){
		myzones[i].disp(i*100 + LEFTBAR_Y, shown_depth + 20);
	}
	/*char currentphase = state >> 5; mvwprintw(winzones[0], 0, 27, "%d", currentphase);
	wattron(winzones[0], A_ALTCHARSET);
	for(int i=0; i<7; i++){
		if(i==currentphase) wattron(winzones[0], A_REVERSE);
		mvwprintw(winzones[0], 0, 30+4*i, "%s", phases_dcp[i]);
		wattroff(winzones[0], A_REVERSE);
	}
	wattroff(winzones[0], A_ALTCHARSET);
	wrefresh(winzones[0]);*/
}

void Player::disp(){
	disp_header();
	for(int j=0; j<5; j++){
		disp_zone(j);
	}
	al_flip_display();
}

void Player::disp_zone(int nbzone){
	int pos=0;
	for(Permanent* p = fperm[nbzone]; p!=0; p=p->next){
		p->disp(pos++, nbzone, false);
	}
}

void CardZone::describe(char* tmp){
	//mvwprintw(w, 1, y, "%s (%d)", name, size);
}

void CardZone::disp(int x, int y){
	ALLEGRO_BITMAP* cardback = god.images->card_back;
	al_draw_scaled_bitmap(cardback, 0, 0, 101, 140, (float)x, (float)y, 101/2, 140/2, 0);
	al_draw_textf(god.font, al_map_rgb(255, 255, 255), x + 20, y + 20, 0, "%d", size);
}

void Player::clear_opts(){
	al_draw_filled_rectangle(0, LOGLEN*10, LEFTBAR_Y, IO_X, al_map_rgb(0, 0, 0));
}

ImageHolder::ImageHolder(){
	for(int i=0; i<17; i++){
		ManaNumSym[i] = al_load_bitmap( &("Materials/images/manasymbols/" + std::to_string(i) + ".png")[0]);
	}
	const char* colornames = "cwubrgm";
	for(int i=0; i<5; i++){
		ManaColorSym[i] = al_load_bitmap(&((std::string) "Materials/images/manasymbols/mana_" + colornames[i+1] + ".png")[0]);
	}
	for(int i=0; i<7; i++){
		card_template[i] = al_load_bitmap(&((std::string) "Materials/images/cardtmp_" + colornames[i] + ".jpg")[0]);
		watermarks[i] = al_load_bitmap(&((std::string) "Materials/images/watermark_" + colornames[i] + ".png")[0]);
		pt_box[i] = al_load_bitmap(&((std::string) "Materials/images/" + colornames[i] + "pt.png")[0]);
	}
	card_back = al_load_bitmap("Materials/images/card_back.jpeg");
	wallpaper = al_load_bitmap("Materials/images/wallpapers/Omnath.jpg");
	tapsymbol = al_load_bitmap("Materials/images/tapsymbol.png");
	basiclands = al_load_bitmap("Materials/images/basic_lands.jpg");
	loyalty = al_load_bitmap("Materials/images/loyalty.png");
}

ImageHolder::~ImageHolder(){
	for(int i=0; i<16; i++) al_destroy_bitmap(ManaNumSym[i]);
	for(int i=0; i<5; i++) al_destroy_bitmap(ManaColorSym[i]);
	for(int i=0; i<7; i++) al_destroy_bitmap(card_template[i]);
	al_destroy_bitmap(wallpaper);
	al_destroy_bitmap(card_back);
	al_destroy_bitmap(tapsymbol);
}