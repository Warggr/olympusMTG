#ifndef maybe_virtual
#define maybe_virtual
#define maybe_undef override
#endif

#ifndef METHOD
#define METHOD(type, x) maybe_virtual type x maybe_undef
#endif

METHOD(void, getResolution(int& posterYSize, int& posterZSize, int& linesize) const);
METHOD(void, harmonize(const Rect& poster, const Rect& message, int nb_winzones));
METHOD(void, declare_window(int& leftY, int& topz, int width, int height));

METHOD(void, setMenuScene());
METHOD(void, setGameScene());

//METHOD(void, fulldisp() const);
METHOD(void, erase_surface(const Rect& rectangle) const);
METHOD(void, draw_rectangle(char color, const Rect& rect, int linewidth) const);
METHOD(void, draw_full_rectangle(char color, const Rect& rectangle) const);

METHOD(void, print_text(const char* text, char color, int x, int y) const);
METHOD(void, print_text(const std::string& text, char color, int x, int y) const);
METHOD(void, draw_boxed_text(const char* text, char color, char backgr_color, int left, int top, int width) const);
METHOD(void, draw_boxed_text(const std::string& text, char color, char backgr_color, int left, int top, int width) const);
METHOD(void, message(const char* message) const);
METHOD(void, message(const std::string& text) const);

METHOD(void, draw_permanent(const Rect& zone, const std::string& name, char color, bool tapped, bool highlight, bool basicImg) const);
METHOD(void, draw(const Player& player, const Rect& zone, bool highlight) const);
METHOD(void, disp_player(const Player& player, int flags) const);
METHOD(void, draw_cardback(const Rect& zone, int oncard_number) const);
METHOD(void, draw(const CardOracle& card, const Rect& zone, bool highlight) const);
METHOD(void, draw(const Permanent& perm, const Rect& zone, bool highlight) const);
METHOD(void, draw(const Resolvable& resolvable, const Rect& zone, bool highlight) const);
METHOD(void, poster(const CardOracle& card) const);

METHOD(bool, attackSwitch(int leftY, int rightY, int topZ, int arrowlength) const);

METHOD(int, getInt(int lowerBound, int upperBound));
METHOD(DirectioL, get_direction_key());
METHOD(std::string, getTextInput(const char* question));
METHOD(bool, simpleChoice(const char* optTrue, const char* optFalse));

METHOD(void, disp_inrow(const Displayable*, int, int, int) const);

METHOD(float, gmouseY());
METHOD(float, gmouseZ());
METHOD(bool, gmouseActive());

#undef METHOD

#undef maybe_undef
#undef maybe_virtual
