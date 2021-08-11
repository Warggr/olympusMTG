#ifndef maybe_virtual
#define maybe_virtual
#define maybe_undef override
#endif

maybe_virtual void getResolution(int& posterYSize, int& posterZSize, bool& hasMouseSupport, int& linesize) const maybe_undef;
maybe_virtual void harmonize(const Rect& poster, const Rect& message, int nb_winzones) maybe_undef;
maybe_virtual void declare_window(int& leftY, int& topz, int width, int height) maybe_undef;
maybe_virtual void fulldisp() const maybe_undef;
maybe_virtual void draw_permanent(const Rect& zone, const std::string& name, char color, bool tapped, bool highlight, bool basicImg) const maybe_undef;
maybe_virtual void poster(const std::string& name, Mana manacost, char color, const char* types,
      const std::vector<std::string>& lines, int power, int toughness, char frametype, bool watermark) const maybe_undef;
maybe_virtual void print_text(const char* text, char color, int x, int y) const maybe_undef;
maybe_virtual void print_text(const std::string& text, char color, int x, int y) const maybe_undef;
maybe_virtual void draw_boxed_text(const char* text, char color, char backgr_color, int left, int top, int width) const maybe_undef;
maybe_virtual void draw_boxed_text(const std::string& text, char color, char backgr_color, int left, int top, int width) const maybe_undef;
maybe_virtual void message(const char* message) const maybe_undef;
maybe_virtual void message(const std::string& text) const maybe_undef;
maybe_virtual void erase_surface(const Rect& rectangle) const maybe_undef;
maybe_virtual void draw_rectangle(char color, const Rect& rect, int linewidth) const maybe_undef;
maybe_virtual bool attack_switch(int leftY, int rightY, int topZ, int arrowlength) const maybe_undef;
maybe_virtual void disp_header(const Rect& zone, const char* name, int life, char phase, bool highlight, Mana pool) const maybe_undef;
maybe_virtual void draw_full_rectangle(char color, const Rect& rectangle) const maybe_undef;
maybe_virtual void disp_cardback(const Rect& zone, int oncard_number) const maybe_undef;
maybe_virtual int getInt(int lowerBound, int upperBound) maybe_undef;

maybe_virtual DirectioL get_direction_key() maybe_undef;
maybe_virtual float gmouseY() maybe_undef;
maybe_virtual float gmouseZ() maybe_undef;
maybe_virtual bool gmouseActive() maybe_undef;
maybe_virtual void setMenuScene() maybe_undef;
maybe_virtual void setGameScene() maybe_undef;
maybe_virtual std::string getTextInput(const char* question) maybe_undef;

#undef maybe_undef
#undef maybe_virtual
