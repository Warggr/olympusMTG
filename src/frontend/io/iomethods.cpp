#ifndef maybe_virtual
#define maybe_virtual
#define maybe_undef override
#endif
#ifndef BOOLEAN
#define BOOLEAN(x) bool x
#define INTEGER(x) int x
#define DEVOID(x) void x
#define FLOATING(x) float x
#define STRING(x) std::string x
#define DIRECTIONENUM(x) DirectioL x
#endif

maybe_virtual DEVOID(getResolution(int& posterYSize, int& posterZSize, bool& hasMouseSupport, int& linesize) const maybe_undef);
maybe_virtual DEVOID(harmonize(const Rect& poster, const Rect& message, int nb_winzones) maybe_undef);
maybe_virtual DEVOID(declare_window(int& leftY, int& topz, int width, int height) maybe_undef);
maybe_virtual DEVOID(fulldisp() const maybe_undef);
maybe_virtual DEVOID(draw_permanent(const Rect& zone, const std::string& name, char color, bool tapped, bool highlight, bool basicImg) const maybe_undef);
maybe_virtual DEVOID(poster(const std::string& name, Mana manacost, char color, const char* types,
      const std::vector<std::string>& lines, int power, int toughness, char frametype, bool watermark) const maybe_undef);
maybe_virtual DEVOID(print_text(const char* text, char color, int x, int y) const maybe_undef);
maybe_virtual DEVOID(print_text(const std::string& text, char color, int x, int y) const maybe_undef);
maybe_virtual DEVOID(draw_boxed_text(const char* text, char color, char backgr_color, int left, int top, int width) const maybe_undef);
maybe_virtual DEVOID(draw_boxed_text(const std::string& text, char color, char backgr_color, int left, int top, int width) const maybe_undef);
maybe_virtual DEVOID(message(const char* message) const maybe_undef);
maybe_virtual DEVOID(message(const std::string& text) const maybe_undef);
maybe_virtual DEVOID(erase_surface(const Rect& rectangle) const maybe_undef);
maybe_virtual DEVOID(draw_rectangle(char color, const Rect& rect, int linewidth) const maybe_undef);
maybe_virtual BOOLEAN( attack_switch(int leftY, int rightY, int topZ, int arrowlength) const maybe_undef);
maybe_virtual DEVOID(disp_header(const Rect& zone, const char* name, int life, char phase, bool highlight, Mana pool) const maybe_undef);
maybe_virtual DEVOID(draw_full_rectangle(char color, const Rect& rectangle) const maybe_undef);
maybe_virtual DEVOID(disp_cardback(const Rect& zone, int oncard_number) const maybe_undef);
maybe_virtual INTEGER( getInt(int lowerBound, int upperBound) maybe_undef);

maybe_virtual DIRECTIONENUM(get_direction_key() maybe_undef);
maybe_virtual FLOATING( gmouseY() maybe_undef);
maybe_virtual FLOATING( gmouseZ() maybe_undef);
maybe_virtual BOOLEAN( gmouseActive() maybe_undef);
maybe_virtual DEVOID(setMenuScene() maybe_undef);
maybe_virtual DEVOID(setGameScene() maybe_undef);
maybe_virtual STRING( getTextInput(const char* question) maybe_undef);

maybe_virtual BOOLEAN( simpleChoice(const char* optTrue, const char* optFalse) maybe_undef);

#undef BOOLEAN
#undef INTEGER
#undef FLOATING
#undef DIRECTIONENUM
#undef STRING
#undef DEVOID

#undef maybe_undef
#undef maybe_virtual
