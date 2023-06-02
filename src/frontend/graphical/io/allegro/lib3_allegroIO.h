#ifndef OLYMPUS_11_ALLEGRO_IO
#define OLYMPUS_11_ALLEGRO_IO

#include "../6abstractIO.h"
#include "frontend/headQ_rect.h"
#include "build_types.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>

class AllegroIO: public AbstractIO {
private:
    Rect posterZone;
    int messageY, messageZ;
    ALLEGRO_FONT* fonts[4]; //normal - small - enormous - matrixB
    ALLEGRO_COLOR registeredColors[12];
    ALLEGRO_EVENT_QUEUE* queue;
    ALLEGRO_DISPLAY* window;
    ALLEGRO_BITMAP* screen;
    ALLEGRO_BITMAP* ManaNumSym[16], * ManaColorSym[5], * card_template[7], * watermarks[7], * pt_box[7];
    ALLEGRO_BITMAP* wallpaper, * card_back, * tapsymbol, * basiclands, * loyalty, * cursor_img;
    ALLEGRO_MOUSE_CURSOR* cursor;
    float mousey{0}, mousez{0};
    bool mouseActive{false};
    static constexpr int screenY = 1500, screenZ = 900;
    static constexpr int fullcardY = 375, fullcardZ = 523;
    static constexpr int UNLIMITED = 0;

    void refresh_display() const;
    void disp_mana(Mana mana, int endy, int topz) const;
public:
    AllegroIO();
    ~AllegroIO();

#include "../iomethods.cpp"

    void focus() override;
};

inline float AllegroIO::gmouseY() { return mousey; }
inline float AllegroIO::gmouseZ() { return mousez; }
inline bool AllegroIO::gmouseActive(){ return mouseActive; }

inline void AllegroIO::declare_window(int&, int&, int, int) {}
inline void AllegroIO::print_text(const std::string &text, char color, int x, int y) const {
    print_text(text.c_str(), color, x, y);
}
inline void AllegroIO::draw_boxed_text(const std::string &text, char color, char backgr_color, int left, int top,
                                int width) const {
    draw_boxed_text(text.c_str(), color, backgr_color, left, top, width);
}

struct AllegroCanvas: public Canvas {
private:
    const AllegroIO* io;
    const Rect rect;
public:
    AllegroCanvas(const AllegroIO* io, const Rect& rect): io(io), rect(rect) {};
    void disp(const CardOracle& oracle, int flags) const {
        io->draw(oracle, rect, flags);
    }
    void disp_player(const Gamer& player, int flags) const {
        io->draw(player, rect, flags);
    }
};

#endif //OLYMPUS_11_ALLEGRO_IO
