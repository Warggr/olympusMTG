#ifndef OLYMPUS_11_ABSTRACT_UI
#define OLYMPUS_11_ABSTRACT_UI

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

class Default_ui: public Abstract_ui{
private:
	static const int leftbarY;
	static const int boardY;
	static const int rightbarY;
	static const int stackZ;
	static const int optionsZ;
	static const int playerZ;
	static const int iozZ;
	int posterZ;
	static const int permanentZSize;
	static const int permanentYSize;
	static const int optionZSize;
	static const int permanentZMargin;
	_UIElement* playerPerms[2];
	_UIElement* permanentZones[10];
	_UIElement* optionZone;
	_UIElement* logbookZone;
	_UIElement* stackZone;
public:
	Default_ui(Abstract_io* IOLib);
	~Default_ui(){delete playerPerms[0]; delete playerPerms[1]; delete optionZone; };
	UIElement* declare_element(int typeofelement, char owner_id);
	void get_player_coords(char player_id, int* py, int* pz, int* wide, int* high, int* liby, int* libz, int* gravey, int* gravez, int* exily, int* exilz);
	bool chooseattackers(std::list<Creature>& cowards, std::list<Creature>& warriors, char player_id);
	void chooseblockers(std::list<Creature>& defenders, std::list<Creature>& attackers, UIElement* defenderDisplay, UIElement* attackerDisplay);
	void clear_opts();
	UIElement* get_optionzone();
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

#endif //OLYMPUS_11_ABSTRACT_UI