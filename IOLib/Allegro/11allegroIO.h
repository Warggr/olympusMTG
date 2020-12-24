#ifndef OLYMPUS_11_ALLEGRO_IO
#define OLYMPUS_11_ALLEGRO_IO

#define MANUAL_INCLUDE_OF_OLYMPUS_HEADERS
#include "../../include/.olympus_main.h"
#include "../../include/6abstractIO.h"
#include "../../include/head1_constants.h"
#include "../../HFiles/head3_readfiles.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

class Allegro_io: public Abstract_io {
private:
	ALLEGRO_FONT* fonts[4]; //normal - small - enormous - matrixB
	ALLEGRO_COLOR registeredColors[12];
	//ALLEGRO_TIMER* timer;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_DISPLAY* window;
	
	ALLEGRO_BITMAP* screenFloor;
	ALLEGRO_BITMAP* ManaNumSym[16], * ManaColorSym[5], * card_template[7], * watermarks[7], * pt_box[7];
	ALLEGRO_BITMAP* wallpaper, * card_back, * tapsymbol, * basiclands, * loyalty, * cursor, * negative_cursor;

	static const int fullcardY = 375, fullcardZ = 523;
	int mousey, mousez;
	void refresh_display();
public:
	Allegro_io();
	~Allegro_io();

	void fulldisp();
	void draw_permanent(int left, int top, int width, int height, char color, bool tapped, bool highlight, bool basicImg);
	void poster(const std::string name, Mana manacost, char color, const char* types,
		const std::vector<std::string> lines, int power, int toughness, char frametype, bool watermark);
	void print_text(std::string const text, const char color, const int x, const int y);
	void erase_surface(int left, int top, int width, int height);
	void disp_mana(Mana mana, const int topz, const int endy);
	void draw_rectangle(char color, int y, int z, int dy, int dz, int linewidth);
	bool attack_switch(int leftY, int rightY, int topZ, int arrowlength);
	Creature* blocker_switch(const Creature& blocker, int blockerY, int blockerZ,
		std::list<Creature>& attackers, UIElement* attacker_io,
		int creatureWidth, int creatureDepth);
	void getResolution(int* posterYSize, int* posterZSize);
	void disp_header(int y, int z, int width, int height, const char* name, int life, char phase, bool highlight, Mana pool);
	void draw_full_rectangle(char color, int left, int top, int width, int height);
	void disp_cardback(int y, int z);
	int getInt(int lowerBound, int upperBound);

	char get_direction_key();

	void message(const char* message);
	void message(std::string message);
};

#endif //OLYMPUS_11_IMPLEMENT_IO