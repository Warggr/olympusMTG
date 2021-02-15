#ifndef OLYMPUS_11_ALLEGRO_IO
#define OLYMPUS_11_ALLEGRO_IO

#include "../../include/.olympus_main.h"
#include "../../include/6abstractIO.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

class Allegro_io: public Abstract_io {
private:
	int posterY, posterZ;
	int messageY, messageZ;
	ALLEGRO_FONT* fonts[4]; //normal - small - enormous - matrixB
	ALLEGRO_COLOR registeredColors[12];
	//ALLEGRO_TIMER* timer;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_DISPLAY* window;
	ALLEGRO_BITMAP* screenFloor;
	ALLEGRO_BITMAP* ManaNumSym[16], * ManaColorSym[5], * card_template[7], * watermarks[7], * pt_box[7];
	ALLEGRO_BITMAP* wallpaper, * card_back, * tapsymbol, * basiclands, * loyalty, * cursor, * negative_cursor;
	float mousey{0}, mousez{0};
	bool mouseActive{false};
	static const int screenY = 1500, screenZ = 900;
	static const int fullcardY = 375, fullcardZ = 523;
	static const int BLACK = 1, WHITE = 0, GREY = 2, HIGH1 = 10, HIGH2 = 11;

	void refresh_display() const;
	void disp_mana(Mana mana, int endy, int topz) const;
public:
	Allegro_io();
	~Allegro_io();

	void getResolution(int& posterYSize, int& posterZSize, bool& hasMouseSupport, int& linesize) const;
	void harmonize(const Rect& poster, const Rect& message, int nb_winzones);
	void declare_window(int& leftY, int& topz, int width, int height){};
	void fulldisp() const;
	void draw_permanent(const Rect& zone, char color, bool tapped, bool highlight, bool basicImg) const;
	void poster(const std::string name, Mana manacost, char color, const char* types,
		const std::vector<std::string> lines, int power, int toughness, char frametype, bool watermark) const;
	void print_text(const char* text, char color, int x, int y) const;
	void print_text(const std::string text, char color, int x, int y) const {print_text(&(text[0]), color, x, y); };
	void draw_boxed_text(const char* text, char color, char backgr_color, int left, int top, int width) const;
	void draw_boxed_text(const std::string text, char color, char backgr_color, int left, int top, int width) const {
		draw_boxed_text(text.c_str(), color, backgr_color, left, top, width); };
	void message(const char* message) const;
	void message(const std::string text) const {message(&(text[0])); };
	void erase_surface(const Rect& rectangle) const;
	void draw_rectangle(char color, const Rect& rect, int linewidth) const;
	bool attack_switch(int leftY, int rightY, int topZ, int arrowlength) const;
	void disp_header(const Rect& zone, const char* name, int life, char phase, bool highlight, Mana pool) const;
	void draw_full_rectangle(char color, const Rect& rectangle) const;
	void disp_cardback(const Rect& zone, int oncard_number) const;
	int getInt(int lowerBound, int upperBound);

	DirectioL get_direction_key();
	float gmouseY(){return mousey; };
	float gmouseZ(){return mousez; };
	bool gmouseActive(){return mouseActive; };
};

#endif //OLYMPUS_11_IMPLEMENT_IO