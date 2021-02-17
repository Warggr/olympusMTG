#ifndef OLYMPUS_11_ALLEGRO_IO
#define OLYMPUS_11_ALLEGRO_IO

#include "../../include/.olympus_main.h"
#include "../../include/6abstractIO.h"

#include <iostream>

/*Nano will try to reverse-engineer iteration and in-game mouse movement*/
class Nano_io: public Abstract_io {
	bool is_iterating;
public:
	Nano_io();
	~Nano_io();

	void getResolution(int& posterYSize, int& posterZSize, bool& hasMouseSupport, int& linesize) const;
	void harmonize(const Rect& poster, const Rect& message, int nb_winzones);
	void declare_window(int& leftY, int& topz, int width, int height);
	void fulldisp() const {};
	void draw_permanent(const Rect& zone, const std::string name, char color, bool tapped, bool highlight, bool basicImg) const;
	void poster(const std::string name, Mana manacost, char color, const char* types,
		const std::vector<std::string> lines, int power, int toughness, char frametype, bool watermark) const;
	void print_text(const char* text, char color, int x, int y) const;
	void print_text(const std::string text, char color, int x, int y) const {print_text(&(text[0]), color, x, y); };
	void draw_boxed_text(const char* text, char color, char backgr_color, int left, int top, int width) const;
	void draw_boxed_text(const std::string text, char color, char backgr_color, int left, int top, int width) const {
		draw_boxed_text(text.c_str(), color, backgr_color, left, top, width); }
	void message(const char* message) const;
	void message(const std::string text) const {message(text.c_str()); }
	void erase_surface(const Rect& rectangle) const;
	void draw_rectangle(char color, const Rect& rect, int linewidth) const;
	bool attack_switch(int leftY, int rightY, int topZ, int arrowlength) const;
	void disp_header(const Rect& zone, const char* name, int life, char phase, bool highlight, Mana pool) const;
	void draw_full_rectangle(char color, const Rect& rectangle) const;
	void disp_cardback(const Rect& zone, int oncard_number) const;
	int getInt(int lowerBound, int upperBound);

	DirectioL get_direction_key();
	float gmouseY(){return 0; };
	float gmouseZ(){return 0; };
	bool gmouseActive(){return false; }; //TODO
};

#endif //OLYMPUS_11_IMPLEMENT_IO