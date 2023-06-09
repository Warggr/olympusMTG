#ifndef OLYMPUS_LIB3_UI_CLI
#define OLYMPUS_LIB3_UI_CLI

#include "../frontend.h"
#include "lib3_nanoIO.h"
#include "headE_enums.h"
#include <forward_list>
#include <memory>

struct Context;

class CliUI: public AbstractFrontEnd {
    NanoIO io;
public:
    const Gamer* pl;
    const std::list<const Gamer*>* players;
//    void create(const char* descr);
//    void update(const char* descr);
//    void del(const char* descr);
//    void bulkOp(const char* descr);

    void splitDamage(int power, std::list<std::pair<uint8_t, SpecificTargeter<Creature>>>& blockers) override;

    const Option* chooseOpt(const Option::CastingContext& context) override;

    const Target* chooseTarget(char type) override;

    void registerPlayers(const std::list<const Gamer*>& players) override { this->players = &players; }
    void registerMe(const Gamer* pl) override;

    void chooseblockers(Y_Hashtable<Creature>& defenders, StateTN<Creature>& attackers) override;

    void addCards(const std::list<CardWrapper>& cards) override;

    void disp(const fwdlist<card_ptr>& list, uint size) override;

    BasicIO* getBasicIO() override { return &io; }

    void list(const Context& context, const Option::CastingContext& castingContext);
};

struct Context {
    unsigned int who : 2; //the selected players, described by a bitfield
    std::vector<const Gamer*> players; // this bitfield is transformed into a vector,
    // which we cache so we don't recalculate it when it doesn't change
    enum PlayersBitfield { none = 0b00, you = 0b01, opponent = 0b10, all = you | opponent };
    zone::zone where;
    const Target* what;

    constexpr static std::array descriptions = {"none", "you", "opponent", "all"};
    Context(const Gamer* pl): who(PlayersBitfield::you), players({pl}), where(zone::hand), what(nullptr) { }
    void setWho(uint l, const CliUI& ui);
    inline const std::vector<const Gamer*> getPlayers() const { return players; }
};

#endif //OLYMPUS_LIB3_UI_CLI
