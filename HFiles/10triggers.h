#ifndef OLYMPUS_HEADER_TRIGGERS_10_H
#define OLYMPUS_HEADER_TRIGGERS_10_H
//REMINDER: class TriggerEvent is declared in 1general.h header, not here

class Trigger{
private:
	//the exact trigger type (such as 'deals damage') is implicitly contained by the Trigger_event
	PreResolvable* effects; //and then the effects. The PreResolvable contains everything necessary to choose targets and put ability onto the stack.
public:
	void setEffects(PreResolvable* preRes) { effects = preRes; };
	void trigger(Player* pl, Target* origin) const;
	std::string describe(int typeoftrig, std::string name) const;
	void write_binary(std::ofstream& bFile) const;
	void read_binary(std::ifstream& bFile);
};

extern const int PermETB;
extern const int PermLTB;
extern const int PermStateChange;
extern const int PermBecomes;
extern const int DamageablePinged;

/*A loses/gains B life
A wins/loses the game
A creates B tokens (because C)
A gains ability B (cf. Theros Archetypes)
A creates B tokens (because C) "a PINK effect causes YOU to create BEEBLE tokens"
A sacrifices B (because C)
A prevents damage (dealt by B)
A shuffles their library (because C)
A searches their library (because C)
A does special action B, i.e. investigate, explore, clash, scry, crew, regenerate, mutate, on C
A plays a land
A is destroyed (by B) (cf. Karmic Justice)
A is countered (by B) (cf. Lullmage Mentor)
A 's mana pool is emptied of mana B
and last but not least:
you control [no artifacts (cf. covetous dragon), a dwarf (cf. goblins of the flarg)]; this will be the most tricky (perhaps see it as a LTB/ETB trigger?)
*/

#endif //OLYMPUS_HEADER_TRIGGERS_11_H