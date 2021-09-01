#ifndef maybe_virtual
#define maybe_virtual
#define maybe_undef override
#endif

#ifndef BOOLEAN
#define BOOLEAN(x) maybe_virtual bool x maybe_undef
#define INTEGER(x) maybe_virtual int x maybe_undef
#define DEVOID(x) maybe_virtual void x maybe_undef
#define FLOATING(x) maybe_virtual float x maybe_undef
#define STRING(x) maybe_virtual std::string x maybe_undef
#define DIRECTIONENUM(x) maybe_virtual DirectioL x maybe_undef
#endif

DEVOID(getResolution(int& posterYSize, int& posterZSize, bool& hasMouseSupport, int& linesize) const);
DEVOID(harmonize(const Rect& poster, const Rect& message, int nb_winzones));
DEVOID(declare_window(int& leftY, int& topz, int width, int height));
DEVOID(fulldisp() const);
DEVOID(draw_permanent(const Rect& zone, const std::string& name, char color, bool tapped, bool highlight, bool basicImg) const);
DEVOID(poster(const std::string& name, Mana manacost, char color, const char* types,
              const std::vector<std::string>& lines, int power, int toughness, char frametype, bool watermark) const);
DEVOID(poster(const Rect& position, bool highlight, const std::string& name, Mana manacost, char color, const char* types,
              const std::vector<std::string>& lines, int power, int toughness, char frametype, bool watermark) const);
DEVOID(print_text(const char* text, char color, int x, int y) const);
DEVOID(print_text(const std::string& text, char color, int x, int y) const);
DEVOID(draw_boxed_text(const char* text, char color, char backgr_color, int left, int top, int width) const);
DEVOID(draw_boxed_text(const std::string& text, char color, char backgr_color, int left, int top, int width) const);
DEVOID(message(const char* message) const);
DEVOID(message(const std::string& text) const);
DEVOID(erase_surface(const Rect& rectangle) const);
DEVOID(draw_rectangle(char color, const Rect& rect, int linewidth) const);
BOOLEAN( attack_switch(int leftY, int rightY, int topZ, int arrowlength) const);
DEVOID(disp_header(const Rect& zone, const char* name, int life, char phase, bool highlight, Mana pool) const);
DEVOID(draw_full_rectangle(char color, const Rect& rectangle) const);
DEVOID(disp_cardback(const Rect& zone, int oncard_number) const);
INTEGER(getInt(int lowerBound, int upperBound));

DIRECTIONENUM(get_direction_key());
FLOATING( gmouseY());
FLOATING( gmouseZ());
BOOLEAN( gmouseActive());
DEVOID(setMenuScene());
DEVOID(setGameScene());
STRING( getTextInput(const char* question));

BOOLEAN( simpleChoice(const char* optTrue, const char* optFalse));

#undef BOOLEAN
#undef INTEGER
#undef FLOATING
#undef DIRECTIONENUM
#undef STRING
#undef DEVOID

#undef maybe_undef
#undef maybe_virtual
