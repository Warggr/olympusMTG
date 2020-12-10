#ifndef OLYMPUS_11_ABSTRACT_UI
#define OLYMPUS_11_ABSTRACT_UI

class _UIElement{
private:
	int topZ;
	int leftY;
	int yOffset;
	int zOffset;
	int ySize;
	int zSize;
	int maxItems;
public:
	static const bool horizontal = true;
	static const bool vertical = false;

	_UIElement(int left, int top, int ySize, int zSize, int offset, int maxItems, bool direction);
	void get_coordinates(int itemNb, int* y, int* z) const;
	void get_coordinates(int* y, int* z, int* yOffset, int* zOffset) const;
	void get_coordinates(int* y, int* z, int* yOff, int* zOff, int* yWidth, int* zHeight) const;
	void erase_background(Abstract_io* io) const;
	template <class T>
	void disp_all(const std::list<T>& objects) const{
		erase_background(god.myIO);
		int y = leftY;
		int z = topZ;
		for(auto iter = objects.begin(); iter != objects.end(); iter++){
			iter->disp(y, z, ySize, zSize, false);
			y += yOffset;
			z += zOffset;
		}
	}
};

class Abstract_ui{
protected:
	Abstract_io* myIO;
public:
	static const int ELTYPE_STACK = 0;
	static const int ELTYPE_LOGBOOK = 1;
	static const int ELTYPE_OPTIONS = 2;
	static const int ELTYPE_PERMANENTS = 3;

	Abstract_ui(Abstract_io* IO): myIO(IO){};
	virtual ~Abstract_ui(){};
	virtual UIElement* declare_element(const int typeofelement, char player_id) = 0;

	virtual void get_player_coords(char player_id, int* py, int* pz, int* wide, int* high, int* liby, int* libz, int* gravey, int* gravez, int* exily, int* exilz) = 0;
	//different zones like the stack, the battlefield, the hand a.s.o. will be handled by team members aka _UIElements
	//while displaying the player is the personal responsibility of the UI
	//virtual void resize_zones(); //in later versions, zone might change position and size to occupy the whole space, as in MTGArena.
	virtual bool chooseattackers(std::list<Creature>& cowards, std::list<Creature>& warriors, char player_id) = 0;
	virtual void chooseblockers(std::list<Creature>& defenders, std::list<Creature>& attackers, UIElement* defenderDisplay, UIElement* attackerDisplay) = 0;
	virtual void clear_opts() = 0;
	virtual UIElement* get_optionzone() = 0;
};

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

#endif //OLYMPUS_11_ABSTRACT_UI