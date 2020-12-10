#ifndef OLYMPUS_9_ABSTRACT_IO
#define OLYMPUS_9_ABSTRACT_IO

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

class Abstract_io{
protected:
	int posterY;
	int posterZ;
	int messageY;
	int messageZ;
public:
	static const int BLACK;
	static const int WHITE;
	static const int GREY;
	static const int HIGH1;
	static const int HIGH2;

	virtual ~Abstract_io(){};
	//virtual void getcoordinates(int* maxX, int* maxY, char* resolution) = 0;
	virtual void getResolution(int* posterYSize, int* posterZSize) = 0;
	virtual void harmonize(int _posterY, int _posterZ, int _messageY, int _messageZ){posterY = _posterY; posterZ = _posterZ; messageY = _messageY; messageZ = _messageZ; };

	virtual void fulldisp() = 0;
	virtual void draw_permanent(int left, int top, int width, int height, char color, bool tapped, bool highlight, bool basicImg) = 0;
	virtual void refresh_display() = 0;
	virtual void print_text(std::string const text, char color, int y, int z) = 0;
	virtual void disp_mana(Mana mana, int endy, int topz) = 0;
	virtual void draw_rectangle(char color, int y, int z, int dy, int dz, int linewidth) = 0;
	virtual bool attack_switch(int leftY, int rightY, int topZ, int arrowlength) = 0;
	virtual void draw_full_rectangle(char color, int left, int top, int width, int height) = 0;
	virtual void erase_surface(int left, int top, int width, int height) = 0;
	virtual void disp_header(int y, int z, int width, int height, const char* name, int life, char phase, bool highlight, Mana pool) = 0;
	virtual char get_direction_key() = 0;
	virtual int getInt(int lowerBound, int upperBound) = 0;

	virtual Creature* blocker_switch(const Creature& blocker, int blockerY, int blockerZ,
		std::list<Creature>& attackers, UIElement* attacker_io,
		int creatureWidth, int creatureDepth) = 0;
	virtual void poster(const std::string name, Mana manacost, char color, const char* types,
		const std::vector<std::string> lines, int power, int toughness, char frametype, bool watermark) = 0;
	virtual void disp_cardback(int y, int z) = 0;
	virtual void message(const char* message) = 0;
	virtual void message(std::string message) = 0;
};

class Allegro_io: public Abstract_io {
private:
	ALLEGRO_FONT* fonts[4]; //normal - small - enormous - matrixB
	ALLEGRO_COLOR registeredColors[12];
	ALLEGRO_TIMER* timer;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_DISPLAY* window;
	
	ALLEGRO_BITMAP* ManaNumSym[16];
	ALLEGRO_BITMAP* ManaColorSym[5];
	ALLEGRO_BITMAP* wallpaper;
	ALLEGRO_BITMAP* card_back;
	ALLEGRO_BITMAP* card_template[7];
	ALLEGRO_BITMAP* tapsymbol;
	ALLEGRO_BITMAP* basiclands;
	ALLEGRO_BITMAP* watermarks[7];
	ALLEGRO_BITMAP* pt_box[7];
	ALLEGRO_BITMAP* loyalty;

	static const int fullcardY = 375;
	static const int fullcardZ = 523;
public:
	Allegro_io();
	~Allegro_io();

	void fulldisp();
	void draw_permanent(int left, int top, int width, int height, char color, bool tapped, bool highlight, bool basicImg);
	void poster(const std::string name, Mana manacost, char color, const char* types,
		const std::vector<std::string> lines, int power, int toughness, char frametype, bool watermark);
	void print_text(std::string const text, const char color, const int x, const int y);
	void refresh_display(){ al_flip_display(); };
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

void must_init(bool test, const char *description);

/*class Ncurses_io: public Abstract_io {
private:
	WINDOW* winzones[4];
public:
	Ncurses_io();
	~Ncurses_io();
	void disp_permanent(const int type, int position);
	void poster(int y, int z, std::string name, Mana manacost, int color, std::string types, int nb_lines, std::string* lines, int power, int toughness);
	void print_text(std::string const text, const char color, const int x, const int y);
	void refresh_display();
	void disp_mana(const Mana mana, const int topz, const int endy);
};*/

#endif //OLYMPUS_9_ABSTRACT_IO