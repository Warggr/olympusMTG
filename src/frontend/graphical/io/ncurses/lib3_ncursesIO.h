#ifndef OLYMPUS_11_NCURSES_IO
#define OLYMPUS_11_NCURSES_IO

#include "../6abstractIO.h"
#include <ncurses.h>

class NcursesIO: public AbstractIO, public Canvas {
private:
	WINDOW** winzones{nullptr};
	WINDOW* message_zone, *poster_zone;
	static const int fullcardY = 20, fullcardZ = 25;
	bool mouseActive{false};
	int mousey, mousez;
	int nb_winzones{0};
	int RIGHTBAR_Y;
	int FULLCARD_Z;

	static constexpr const char* manasymbols = " - \0,O+\0 . \0mXl\0qOq\0jXk\0 k \0lj)\0(sj\0lqk\0OwO\0 - \0 k-\0o u\0(gz\0(Ok\0cO)\0 a";
public:
	static constexpr int MESSAGE = 0;
	static constexpr int POSTER = 1;

	NcursesIO();
	~NcursesIO();

#include "../iomethods.cpp"

	void disp(const CardOracle& oracle, int flags) const override;
    void disp_player(const Gamer& player, int flags) const override;
};

inline float NcursesIO::gmouseY() { return mousey; }
inline float NcursesIO::gmouseZ() {return mousez; }
inline bool NcursesIO::gmouseActive(){ return false; } //TODO

inline void NcursesIO::draw_boxed_text(const std::string& text, char color, char backgr_color, int left, int top, int width) const {
    draw_boxed_text(text.c_str(), color, backgr_color, left, top, width);
}

inline void NcursesIO::print_text(const std::string& text, char color, int x, int y) const {
    print_text(text.c_str(), color, x, y);
}

#endif //OLYMPUS_11_NCURSES_IO
