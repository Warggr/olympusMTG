#include "11allegroIO.h"
#include <list>
#include <iostream>

Abstract_io* new_IOLib(){return new Allegro_io; }

void Allegro_io::draw_permanent(int left, int top, int width, int height, char color, bool tapped, bool highlight, bool basicImg) const {
	if(basicImg) al_draw_scaled_bitmap(basiclands, 384*(main_color(color)-1), (500-384)/2, 384, 384, left, top, width, height, 0);
	else al_draw_filled_rectangle(left, top, left+width, top+height, registeredColors[main_color(color) + 3]);

	if(tapped) al_draw_bitmap(tapsymbol, left, top, 0);
	const int lw = 10;
	if(highlight) al_draw_rectangle(left+lw/2, top+lw/2, left+width-lw/2, top+height-lw/2, registeredColors[Abstract_io::HIGH2], lw);
}

int Allegro_io::getInt(int lowerBound, int upperBound){
	if(lowerBound >= upperBound) return lowerBound;
	int ret = lowerBound;
	while(1){
		char tmp[3]; std::sprintf(tmp, "%d", ret);
		al_draw_text(fonts[1], registeredColors[Abstract_io::BLACK], messageY, messageZ + 30, 0, tmp);
		al_flip_display();
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		switch(event.type){
			case ALLEGRO_EVENT_KEY_CHAR:
				switch(event.keyboard.keycode){
				    case ALLEGRO_KEY_DOWN: ret--; break;
					case ALLEGRO_KEY_UP: ret++; break;
					case ALLEGRO_KEY_ENTER: return ret;
					case ALLEGRO_KEY_RIGHT: ret += 5; break;
					case ALLEGRO_KEY_LEFT: ret -= 5; break;
					default: return 0;
				} break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				god.call_ragnarok();
		}
		if(ret < lowerBound) ret = lowerBound;
		if(ret > upperBound) ret = upperBound;
	}
}

void Allegro_io::getResolution(int& y, int& z, bool& hasMouseSupport) const {
	y = fullcardY;
	z = fullcardZ;
	hasMouseSupport = true;
}

void Allegro_io::erase_surface(int left, int top, int width, int height) const {
	al_draw_scaled_bitmap(wallpaper, left, top, width, height, left, top, width, height, 0);
}

void Allegro_io::disp_header(int y, int z, int width, int height, const char* name, int life, char state, bool highlight, Mana pool) const {
	if(life >= 1000) raise_error("Life total too high to be shown, most likely a bug");
	int x = highlight ? HIGH2 : WHITE;
	al_draw_text(fonts[0], registeredColors[x], y + width/2 - 20, z + 3, 0, name);
	char lifec[4]; std::sprintf(lifec, "%d", life);
	al_draw_text(fonts[1], registeredColors[x], y + width/2 - 15, z + 23, 0, lifec);
	disp_mana(pool, y+width, height);
}

void Allegro_io::disp_mana(Mana mana, int endy, int topz) const {
	int m = mana.m2i();
	char generic = (char) (m&0xf);
	for(int i=0; i<6; i++){
		m = m >> 4;
		for(int j=0; j < (m&0xf); j++){
			endy -= 24;
			generic--;
			al_draw_scaled_bitmap(ManaColorSym[i], 0, 0, 160, 160, endy, topz, 24, 24, 0);
		}
	}
	if(generic!=0){
		al_draw_scaled_bitmap(ManaNumSym[(int) generic], 0, 0, 160, 160, endy-24, topz, 24, 24, 0);
	}
}

void Allegro_io::poster(const std::string name, Mana manacost, char color, const char* types,
	const std::vector<std::string> lines, int power, int toughness, char frametype, bool watermark) const {
	al_draw_bitmap(card_template[(int) color], posterY, posterZ, 0);
	al_draw_text(fonts[0], al_map_rgb(0,0,0), posterY+29, posterZ+28, 0, &(name[0]));
	disp_mana(manacost, posterY + fullcardY - 28, posterZ + 30);
	al_draw_text(fonts[0], al_map_rgb(0,0,0), posterY+29, posterZ+295, 0, &(types[0]));
	if(watermark){
		al_draw_bitmap(watermarks[(int) color], posterY + 120, posterZ + 340, 0);
	}
	int i=0;
	for(auto iter = lines.begin(); iter!=lines.end(); iter++){
		al_draw_text(fonts[3], al_map_rgb(0,0,0), posterY+29, posterZ+360+ (i++)*20, 0, &((*iter)[0]));
	}
	const int yOffset = 85;
	const int zOffset = 60;
	if(frametype == 1){
		al_draw_bitmap(pt_box[(int) color], posterY+fullcardY-yOffset, posterZ+fullcardZ-zOffset, 0);
		char x[10]; sprintf(x, "%d/%d", power, toughness);
		al_draw_text(fonts[0], al_map_rgb(0, 0, 0), posterY+fullcardY-yOffset + 20, posterZ+fullcardZ-zOffset + 5, 0, x);
	}
	else if(frametype == 2){
		al_draw_bitmap(loyalty, posterY+fullcardY-yOffset, posterZ+fullcardZ-zOffset, 0);
		char x[3]; sprintf(x, "%d", power);
		al_draw_text(fonts[0], al_map_rgb(255, 255, 255), posterY+fullcardY-yOffset + 30, posterZ+fullcardZ-zOffset + 8, 0, x);
	}
}

void Allegro_io::message(const char* text) const {
	al_draw_filled_rectangle(messageY, messageZ, messageY+500, messageZ+25, registeredColors[0]);
	al_draw_text(fonts[0], registeredColors[1], messageY, messageZ, 0, text);
	al_flip_display();
}

void Allegro_io::message(const std::string text) const {
	message(&(text[0]));
}

DirectioL Allegro_io::get_direction_key(){
	while(1){
		refresh_display();
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		switch(event.type){
			case ALLEGRO_EVENT_KEY_CHAR:
				mouseActive = false;
				switch(event.keyboard.keycode){
					case ALLEGRO_KEY_DOWN: return DOWN;
					case ALLEGRO_KEY_UP: return UP;
					case ALLEGRO_KEY_ENTER: return ENTER;
					case ALLEGRO_KEY_SPACE: return BACK;
					case ALLEGRO_KEY_RIGHT: return RIGHT;
					case ALLEGRO_KEY_LEFT: return LEFT;
					default: return NOT_RECOGNIZED;
				} break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				god.call_ragnarok();
			case ALLEGRO_EVENT_MOUSE_AXES:
				mouseActive = true;
				mousey = event.mouse.x; mousez = event.mouse.y;
				return MOUSE;
			case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
				mouseActive = true;
				return ENTER;
		}
	}
}

void must_init(bool test, const char *description){
    if(test) return;

    std::cout << "couldn't initialize " << description << std::endl;
    exit(1);
}

Allegro_io::Allegro_io(){
	must_init(al_init(), "allegro");
	must_init(al_install_keyboard(), "keyboard");
	must_init(al_install_mouse(), "mouse");

	queue = al_create_event_queue();
	must_init(queue, "queue");

	//al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	al_set_new_display_option(ALLEGRO_SINGLE_BUFFER, 1, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

	window = al_create_display(1500, 900);
	must_init(window, "display");
	al_set_window_title(window, "MTG Olympus");

	must_init(al_init_font_addon(), "fonts");
	must_init(al_init_ttf_addon(), "TTF");
	fonts[0] = al_load_ttf_font("Materials/fonts/BelerenBold.ttf", 20, 0); must_init(fonts[0], "beleren");
	fonts[1] = al_load_ttf_font("Materials/fonts/BelerenBold.ttf", 15, 0);
	fonts[2] = al_load_ttf_font("Materials/fonts/BelerenBold.ttf", 30, 0);
	fonts[3] = al_load_ttf_font("Materials/fonts/matrixb.ttf", 15, 0); must_init(fonts[3], "matrixB");

	must_init(al_init_primitives_addon(), "primitives");
	must_init(al_init_image_addon(), "images");

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
	card_back = al_load_bitmap("Materials/images/card_back.jpeg"); must_init(card_back, "Card back image");
	wallpaper = al_load_bitmap("Materials/images/wallpapers/Omnath.jpg");
	tapsymbol = al_load_bitmap("Materials/images/tapsymbol.png");
	basiclands = al_load_bitmap("Materials/images/basic_lands.jpg");
	loyalty = al_load_bitmap("Materials/images/loyalty.png");
	cursor = al_load_bitmap("Materials/interface/Cursor_Fang.png");

	registeredColors[0] = al_map_rgb(255, 255, 255); //white
	registeredColors[1] = al_map_rgb(0, 0, 0); //black;
	registeredColors[2] = al_map_rgb(100, 100, 100); //grey
	registeredColors[3] = al_map_rgb(150, 150, 200); //Artifact color
	registeredColors[4] = al_map_rgb(200, 200, 50); //Peace through Order
	registeredColors[5] = al_map_rgb(0, 90, 150); //Perfection through Knowledge
	registeredColors[6] = al_map_rgb(70, 70, 100); //Power through Ambition
	registeredColors[7] = al_map_rgb(175, 0, 0); //Freedom through Action
	registeredColors[8] = al_map_rgb(20, 200, 150); //Growth through Harmony
	registeredColors[9] = al_map_rgb(255, 255, 50); //Multicolor
	registeredColors[10] = al_map_rgb(100, 0, 50); //Attacker dark red (high1)
	registeredColors[11] = al_map_rgb(175, 100, 0); //orange (high2)

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(window));
	al_register_event_source(queue, al_get_mouse_event_source());
	al_hide_mouse_cursor(window);
	screenFloor = al_create_bitmap(al_get_display_width(window), al_get_display_height(window));
	al_set_target_bitmap(screenFloor);

	al_draw_bitmap(wallpaper, 0, 0, 0);
}

Allegro_io::~Allegro_io(){
	al_destroy_font(fonts[0]);
    al_destroy_display(window);
    //al_destroy_timer(timer);
    al_destroy_event_queue(queue);

	for(int i=0; i<16; i++) al_destroy_bitmap(ManaNumSym[i]);
	for(int i=0; i<5; i++) al_destroy_bitmap(ManaColorSym[i]);
	for(int i=0; i<7; i++) al_destroy_bitmap(card_template[i]);
	al_destroy_bitmap(wallpaper);
	al_destroy_bitmap(card_back);
	al_destroy_bitmap(tapsymbol);
	al_destroy_bitmap(basiclands);
	al_destroy_bitmap(loyalty);
	al_destroy_bitmap(cursor);
}

void Allegro_io::disp_cardback(int y, int z) const {
	al_draw_scaled_bitmap(card_back, 0, 0, 101, 140, (float)y, (float)z, 101/2, 140/2, 0);
}

void Allegro_io::draw_full_rectangle(char color, int left, int top, int width, int height) const {
	al_draw_filled_rectangle(left, top, left+width, top+height, registeredColors[(int) color]);
}

void Allegro_io::draw_rectangle(char color, int left, int top, int width, int height, int linewidth) const {
	al_draw_rectangle(top, left, top+height, left+width, registeredColors[(int) color], linewidth);
}

void Allegro_io::print_text(const char* text, char color, int y, int z) const {
    al_draw_text(fonts[0], registeredColors[(int) color], y, z, 0, text);
}

void Allegro_io::refresh_display() const {
	al_set_target_bitmap(al_get_backbuffer(window));
	al_draw_bitmap(screenFloor, 0, 0, 0);
	al_draw_bitmap(cursor, mousey, mousez, 0);
	al_flip_display();
	al_set_target_bitmap(screenFloor);
}

void Allegro_io::fulldisp() const {
	al_draw_bitmap(wallpaper, 0, 0, 0);
}

const int Abstract_io::BLACK = 1;
const int Abstract_io::WHITE = 0;
const int Abstract_io::GREY = 2;
const int Abstract_io::HIGH1 = 10;
const int Abstract_io::HIGH2 = 11;