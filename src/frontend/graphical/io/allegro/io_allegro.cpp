#include "lib3_allegroIO.h"
#include <list>
#include <iostream>
#include "headQ_rect.h"
#include "control/3player.h"
#include "gameplay/resolvables/5resolvables.h"

void al_draw_scaled_bitmap(ALLEGRO_BITMAP* bitmap, int picY, int picZ, int picW, int picH, const Rect& zone, char flags){
	al_draw_scaled_bitmap(bitmap, picY, picZ, picW, picH, zone.y, zone.z, zone.width, zone.height, flags);
}

void AllegroIO::disp_mana(Mana mana, int endy, int topz) const {
	ExplicitMana ex = mana.m2x();
	for(int i=Mana::positions::FIRST_COLOR; i<=Mana::positions::COLORLESS; ++i){
		for(uint j=0; j < ex[i]; ++j){
			endy -= 24;
			al_draw_scaled_bitmap(ManaColorSym[i], 0, 0, 160, 160, endy, topz, 24, 24, 0);
		}
	}
	if(ex[Mana::positions::GENERIC]!=0 || mana.cmc() == 0){
		al_draw_scaled_bitmap(ManaNumSym[ex[Mana::positions::GENERIC]], 0, 0, 160, 160, endy-24, topz, 24, 24, 0);
	}
}

void AllegroIO::disp(const CardOracle &oracle, int flags) const { disp_inrow(&oracle, 1, 1, flags); }

void AllegroIO::disp_inrow(const Displayable* disp, int number, int total, int flags) const {
    (void) disp; (void) number; (void) total; (void) flags; //TODO
}

void AllegroIO::draw_permanent(const Rect& zone, const std::string& name, char color, bool tapped, bool highlight, bool basicImg) const {
	if(basicImg) al_draw_scaled_bitmap(basiclands, 384*(main_color(color)-1), (500-384)/2, 384, 384, zone, 0);
	else draw_full_rectangle(main_color(color) + 3, zone);

	al_draw_text(fonts[0], registeredColors[AbstractIO::BLACK], zone.y+4, zone.z+4, 0, name.c_str());

	if(tapped) al_draw_bitmap(tapsymbol, zone.y, zone.z, 0);
	const int lw = 10;
	if(highlight) al_draw_rectangle(zone.y+lw/2, zone.z+lw/2, zone.y+zone.width-lw/2, zone.z+zone.height-lw/2, registeredColors[AbstractIO::HIGH2], lw);
}

void AllegroIO::getResolution(int& y, int& z, int& linesize) const {
	y = screenY;
	z = screenZ;
	linesize = 20;
}

void AllegroIO::erase_surface(const Rect& zone) const {
	al_draw_scaled_bitmap(wallpaper, zone.y, zone.z, zone.width, zone.height, zone.y, zone.z, zone.width, zone.height, 0);
}

void AllegroIO::draw(const Permanent& perm, const Rect& zone, bool highlight) const {
    char color = main_color(perm.getManaCost().m2color());
    draw_full_rectangle(color, zone);
    draw_boxed_text(perm.getName(), highlight ? HIGH1 : BLACK, highlight ? BLACK : WHITE, zone.y, zone.z, zone.width);
}

void AllegroIO::draw(const Resolvable& resolvable, const Rect& zone, bool highlight) const {
    draw_full_rectangle(GREY, zone);
    draw_boxed_text(resolvable.getName(), highlight ? HIGH1 : BLACK, highlight ? BLACK : WHITE, zone.y, zone.z, zone.width);
}

void AllegroIO::draw(const Player& player, const Rect& zone, bool highlight) const {
	int x = highlight ? HIGH2 : WHITE;
	al_draw_text(fonts[0], registeredColors[x], zone.y + zone.width/2 - 20, zone.z + 3, 0, player.getName().c_str());
	char lifec[4]; std::sprintf(lifec, "%3d", player.getLife());
	al_draw_text(fonts[1], registeredColors[x], zone.y + zone.width/2 - 15, zone.z + 23, 0, lifec);
	disp_mana(player.manapool, zone.y+zone.width, zone.height);
}

void AllegroIO::poster(const CardOracle &card) const { draw(card, posterZone, false); }

void AllegroIO::draw(const CardOracle& card, const Rect& rect, bool highlight) const {
    (void) highlight; //TODO
    int card_color = main_color(card.getCost().mana.m2color());

	al_draw_bitmap(card_template[card_color], rect.y, rect.z, 0);
	al_draw_text(fonts[0], al_map_rgb(0,0,0), rect.y+29, rect.z+28, 0,
                 card.getName().c_str());
	disp_mana(card.getCost().mana, rect.right() - 28, rect.z + 30);

	al_draw_text(fonts[0], al_map_rgb(0,0,0), rect.y+29, rect.z+295, 0, card.getType().toString().c_str());

	if(card.getType().land and card.getType().shift){
		al_draw_bitmap(watermarks[card_color],rect.y + 120, rect.z + 340,0);
	}
	int i=0;
	int frameType, power, toughness;
	for(const auto & line : card.allText(power, toughness, frameType)){
		al_draw_text(fonts[3], al_map_rgb(0,0,0),
                     rect.y+29, rect.z+360+(i++)*20, 0, line.c_str());
	}

	const int yOffset = 85, zOffset = 60;
	if(frameType == 1){
		al_draw_bitmap(pt_box[card_color], rect.right()-yOffset, rect.bottom()-zOffset, 0);
		char x[10]; sprintf(x, "%d/%d", power, toughness);
		al_draw_text(fonts[0], al_map_rgb(0, 0, 0),
                     rect.right()-yOffset + 20, rect.bottom()-zOffset + 5, 0, x);
	}
	else if(frameType == 2){
		al_draw_bitmap(loyalty, rect.right()-yOffset, rect.bottom()-zOffset, 0);
		char x[3]; sprintf(x, "%d", power);
		al_draw_text(fonts[0], al_map_rgb(255, 255, 255), rect.right()-yOffset + 30, rect.bottom()-zOffset + 8, 0, x);
	}
}

void AllegroIO::message(const char* text) const {
	al_draw_filled_rectangle(messageY, messageZ, messageY+500, messageZ+25, registeredColors[0]);
	al_draw_text(fonts[0], registeredColors[1], messageY, messageZ, 0, text);
}

void must_init(bool test, const char *description){
    if(test) return;

    std::cout << "couldn't initialize " << description << std::endl;
    exit(1);
}

void AllegroIO::harmonize(const Rect& poster, const Rect& message, int){
	posterZone = poster;
	posterZone.width = fullcardY; posterZone.height = fullcardZ;
	messageY = message.y; messageZ = message.z;
}

AllegroIO::AllegroIO() {
	must_init(al_init(), "allegro");
	must_init(al_install_keyboard(), "keyboard");
	must_init(al_install_mouse(), "mouse");

	queue = al_create_event_queue();
	must_init(queue, "queue");

	//al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	al_set_new_display_option(ALLEGRO_SINGLE_BUFFER, 1, ALLEGRO_SUGGEST);
	al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

	window = al_create_display(screenY, screenZ);
	must_init(window, "display");
	al_set_window_title(window, "MTG Olympus");

	must_init(al_init_font_addon(), "fonts");
	must_init(al_init_ttf_addon(), "TTF");
	fonts[0] = al_load_ttf_font("../material/fonts/BelerenBold.ttf", 20, 0); must_init(fonts[0], "beleren");
	fonts[1] = al_load_ttf_font("../material/fonts/BelerenBold.ttf", 15, 0);
	fonts[2] = al_load_ttf_font("../material/fonts/BelerenBold.ttf", 30, 0);
	fonts[3] = al_load_ttf_font("../material/fonts/matrixb.ttf", 15, 0); must_init(fonts[3], "matrixB");

	must_init(al_init_primitives_addon(), "primitives");
	must_init(al_init_image_addon(), "images");

	for(int i=0; i<17; i++){
		ManaNumSym[i] = al_load_bitmap( &("../material/images/manasymbols/" + std::to_string(i) + ".png")[0]);
	}
	const char* colornames = "cwubrgm";
	for(int i=0; i<5; i++){
		ManaColorSym[i] = al_load_bitmap(&((std::string) "../material/images/manasymbols/mana_" + colornames[i+1] + ".png")[0]);
	}
	for(int i=0; i<7; i++){
		card_template[i] = al_load_bitmap(((std::string) "../material/images/cardtmp_" + colornames[i] + ".jpg").c_str());
		watermarks[i] = al_load_bitmap(((std::string) "../material/images/watermark_" + colornames[i] + ".png").c_str());
		pt_box[i] = al_load_bitmap(((std::string) "../material/images/" + colornames[i] + "pt.png").c_str());
	}
	card_back = al_load_bitmap("../material/images/card_back.jpeg"); must_init(card_back, "Card back image");
	ALLEGRO_BITMAP* wallpaper_full = al_load_bitmap("../material/images/wallpapers/default.jpeg");
	wallpaper = al_create_bitmap(screenY, screenZ);
    al_set_target_bitmap(wallpaper);
    al_draw_scaled_bitmap(wallpaper_full, 0, 0, al_get_bitmap_width(wallpaper_full), al_get_bitmap_height(wallpaper_full),
                          0, 0, screenY, screenZ, 0);
    al_set_target_bitmap(al_get_backbuffer(window));
    al_destroy_bitmap(wallpaper_full);
	tapsymbol = al_load_bitmap("../material/images/tapsymbol.png");
	basiclands = al_load_bitmap("../material/images/basic_lands.jpg");
	loyalty = al_load_bitmap("../material/images/loyalty.png");
	cursor_img = al_load_bitmap("../material/interface/Cursor_Fang.png");

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
    cursor = al_create_mouse_cursor(cursor_img, 0, 0);
    al_set_mouse_cursor(window, cursor);
	//screenFloor = al_create_bitmap(al_get_display_width(window), al_get_display_height(window));
	//al_set_target_bitmap(screenFloor);
}

AllegroIO::~AllegroIO(){
	al_destroy_font(fonts[0]);
    al_destroy_display(window);
    //al_destroy_timer(timer);
    al_destroy_event_queue(queue);

	for(auto & i : ManaNumSym) al_destroy_bitmap(i);
	for(auto & i : ManaColorSym) al_destroy_bitmap(i);
	for(auto & i : card_template) al_destroy_bitmap(i);
	al_destroy_bitmap(wallpaper);
	al_destroy_bitmap(card_back);
	al_destroy_bitmap(tapsymbol);
	al_destroy_bitmap(basiclands);
	al_destroy_bitmap(loyalty);
	al_destroy_bitmap(cursor_img);
    al_destroy_mouse_cursor(cursor);
}

void AllegroIO::draw_cardback(const Rect& zone, int oncard_number) const {
	int zfactor = 140/zone.height; int yfactor = 101/zone.width;
	int factor = (zfactor > yfactor) ? yfactor : zfactor;
	if(factor == 0) factor = 1;
	al_draw_scaled_bitmap(card_back, 0, 0, 101, 140, (float)zone.y, (float)zone.z, 101/factor, 140/factor, 0);
	std::string text = std::to_string(oncard_number);
	al_draw_text(fonts[0], registeredColors[AllegroIO::BLACK], zone.y+20, zone.z+20, 0, &(text[0]));
}

void AllegroIO::draw_boxed_text(const char* text, char color, char backgrd_color, int y, int z, int width) const {
	al_draw_filled_rectangle(y, z, y+width, z+40, registeredColors[(int) backgrd_color]);
	al_draw_text(fonts[0], registeredColors[(int) color], y, z, 0, text);
}

void AllegroIO::draw_full_rectangle(char color, const Rect& zone) const {
	al_draw_filled_rectangle(zone.y, zone.z, zone.y+zone.width, zone.z+zone.height, registeredColors[(int) color]);
}

void AllegroIO::draw_rectangle(char color, const Rect& zone, int linewidth) const {
	al_draw_rectangle(zone.z, zone.y, zone.bottom(), zone.right(), registeredColors[(int) color], linewidth);
}

void AllegroIO::print_text(const char* text, char color, int y, int z) const {
    al_draw_text(fonts[0], registeredColors[(int) color], y, z, 0, text);
}

void AllegroIO::refresh_display() const {
//	al_set_target_bitmap(al_get_backbuffer(window));
//	al_draw_bitmap(screenFloor, 0, 0, 0);
//	al_draw_bitmap(cursor, mousey, mousez, 0);
	al_flip_display();
//	al_set_target_bitmap(screenFloor);
}

void AllegroIO::fulldisp() const {
	al_draw_bitmap(wallpaper, 0, 0, 0);
}

void AllegroIO::setMenuScene() {
    al_draw_bitmap(wallpaper, 0, 0, 0);
    al_flip_display();
}

void AllegroIO::setGameScene() {

}
