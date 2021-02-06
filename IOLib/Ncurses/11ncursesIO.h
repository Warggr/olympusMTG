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

class Ncurses_io: public Abstract_io {
private:
	ALLEGRO_FONT* fonts[4]; //normal - small - enormous - matrixB
	ALLEGRO_COLOR registeredColors[12];
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_DISPLAY* window;
	ALLEGRO_BITMAP* screenFloor;
	ALLEGRO_BITMAP* ManaNumSym[16], * ManaColorSym[5], * card_template[7], * watermarks[7], * pt_box[7];
	ALLEGRO_BITMAP* wallpaper, * card_back, * tapsymbol, * basiclands, * loyalty, * cursor, * negative_cursor;
	float mousey{0}, mousez{0};
	bool mouseActive{false};
	static const int fullcardY = 375, fullcardZ = 523;

	void refresh_display() const;
public:
	Ncurses_io();
	~Ncurses_io();

	void fulldisp() const;
	void draw_permanent(int left, int top, int width, int height, char color, bool tapped, bool highlight, bool basicImg) const;
	void poster(const std::string name, Mana manacost, char color, const char* types,
		const std::vector<std::string> lines, int power, int toughness, char frametype, bool watermark) const;
	void print_text(std::string const text, const char color, const int x, const int y) const;
	void erase_surface(int left, int top, int width, int height) const;
	void disp_mana(Mana mana, const int topz, const int endy) const;
	void draw_rectangle(char color, int y, int z, int dy, int dz, int linewidth) const;
	bool attack_switch(int leftY, int rightY, int topZ, int arrowlength) const;
	Creature* blocker_switch(const Creature& blocker, int blockerY, int blockerZ,
		PContainer<Creature>& attackers, UIElement* attacker_io,
		int creatureWidth, int creatureDepth) const;
	void getResolution(int& posterYSize, int& posterZSize, bool& hasMouseSupport) const;
	void disp_header(int y, int z, int width, int height, const char* name, int life, char phase, bool highlight, Mana pool) const;
	void draw_full_rectangle(char color, int left, int top, int width, int height) const;
	void disp_cardback(int y, int z) const;
	int getInt(int lowerBound, int upperBound);

	DirectioL get_direction_key();

	void message(const char* message) const;
	void message(std::string message) const;
	float gmouseY(){return mousey; };
	float gmouseZ(){return mousez; };
	bool gmouseActive(){return mouseActive; };
};

#endif //OLYMPUS_11_IMPLEMENT_IO