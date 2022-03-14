#include "lib3_cliUI.h"
#include "gameplay/1general.h"
#include "gameplay/optionwrappers.h"
#include "control/3player.h"
#include "control/7game.h"
#include "headE_enums.h"
#include "boost/format.hpp"
#include <vector>

struct Context {
    zone::zone where;
    Target* what;
    std::vector<const Player*> players;
    unsigned int who : 2;
    constexpr static char const* descriptions[] = {"none", "you", "opponent", "all"};
    Context(Player* pl): where(zone::hand), what(nullptr), players({pl}), who(0b01) { }
    inline std::string prompt() const {
        return (boost::format("@%s:%s>")
                % Context::descriptions[who]
                % zone::descriptions[where]).str();
    }
    void setWho(uint l, const CliUI& ui);
    inline const std::vector<const Player*> getPlayers() const { return players; }
};

namespace cmd {
    enum command { sel = 0, pass, concede, ls, view, cd, help, profile };
    const char* descriptions[] = {"sel", "pass", "exit", "ls", "view", "cd", "help", "profile" };
}
const char* zone::descriptions[] = {"hand", "graveyard", "battlefield", "stack", "exile", "commandzone"};

bool readCommand(const char* input, Context& context, cmd::command& cmd, const CliUI& ui);
void printHelp();
void dispPlayers(const std::vector<const Player*> vec, const NanoIO& io);

Option* CliUI::chooseOpt(bool sorcerySpeed) {
    using namespace cmd;
    (void) sorcerySpeed; //TODO
    static Context context(pl);
    while(true) {
        Context temporaryContext = context;
        command cmd = cd;
        while(true) {
            std::string input = io.getTextInput(context.prompt().c_str(),false);
            if(readCommand(input.c_str(), temporaryContext, cmd, *this)) break;
        }
        switch(cmd){
            case cd: context = temporaryContext; break;
            case ls: list(temporaryContext.where); break;
            case sel: return dynamic_cast<OptionWrapper*>(temporaryContext.what)->chooseOptionAction();
            case view: temporaryContext.what->disp(&io); break;
            case help: printHelp(); break;
            case concede: throw UIClosedException();
            case pass: return nullptr;
            case profile: dispPlayers( temporaryContext.getPlayers(), io ); break;
        }
    }
}

Target* readTarget(const char* str, zone::zone zone, const std::vector<const Player*>& pl) {
//    std::cout << "Read target '" << str << "'\n";
    unsigned int pos = 0;
    for(unsigned int i = 0; '0' <= str[i] and str[i] <= '9'; i++) {
        pos = 10 * pos + str[i] - '0';
    }
    if(pos == 0) { //no recognized character
        std::cout << "Unrecognized target: '" << str << "'\n";
        return nullptr;
    }
    pos -= 1; //'1' refers to the object at position 0

    switch(zone) {
    case zone::hand: if(pos < pl[0]->getHand().size()) {
        auto iter = pl[0]->getHand().begin();
        std::advance(iter, pos);
        return const_cast<CardWrapper*>(iter.operator->());
    } else {
        std::cout << "No such card\n";
    } break;
    default:
        std::cout << "Zone not implemented yet.\n";
    }
    return nullptr;
}

bool read(const char* str, const char* const* opts, uint nbopts, uint& ret, uint& j) {
    for(ret=0; ret<nbopts; ++ret) {
        for(j=0; opts[ret][j] != '\0'; ++j) if(str[j] != opts[ret][j]) break;
        if(opts[ret][j] == '\0' and (str[j] < 'A' or 'z' < str[j] )) break;
    }
    if(ret == nbopts){
        j = 0;
        std::cout << "Unrecognized value: " << str << ". Try 'help'.\n";
        return false;
    } else {
        return true;
    }
}

#define ENUMERATE(list) list, sizeof(list) / sizeof(const char*)

inline void readContext(const char* input, uint& offset, Context& context, const CliUI& ui) {
    uint k, l;
    if(input[offset] == '#') {
        ++offset;
        if(read(input+offset, ENUMERATE(Context::descriptions), l, k)) {
            context.setWho(l, ui);
            offset += k;
            while(input[offset] == ' ') ++offset;
        }
    }
    if(input[offset] == ':') {
        ++offset;
        if(read(input+offset, ENUMERATE(zone::descriptions), l, k)){
            context.where = static_cast<zone::zone>(l);
            offset += k;
            while(input[offset++] == ' ');
        }
    }
}

bool readCommand(const char* input, Context& context, cmd::command& cmd, const CliUI& ui) {
    using namespace cmd;
    uint i, j;
    if(!read(input,ENUMERATE(descriptions), i, j)) return false;
    
    while(input[j] == ' ') ++j;
    readContext(input, j, context, ui);
    cmd = static_cast<command>(i);
    if(cmd == view || cmd == sel){
        context.what = readTarget(input+j, context.where, context.getPlayers());
        return (context.what != nullptr);
    }
    return true;
}

void dispPlayers(const std::vector<const Player*> vec, const NanoIO& io) {
    for(auto player: vec) {
        io.disp_player(*player, NanoIO::INLINE);
    }
}

void Context::setWho(uint l, const CliUI& ui){
    if(l == who) return;
    who = l;
    players = std::vector<const Player*>();
    if(l & 0b01){
        players.push_back(ui.getPl());
    }
    if(l & 0b10){
        for(const auto& opp : Game::god->players){
            if(&opp != ui.getPl()) players.push_back(&opp);
        }
    }
}

void printHelp() {
    std::cout << "Syntax: [COMMAND] [#PLAYERS][:ZONE] [ARGUMENTS]" << "\n";
    std::cout << "Available commands: ";
    for(auto& description : cmd::descriptions) std::cout << description << " ";
    std::cout << '\n';
    std::cout << "Available zones: ";
    for(auto& description : zone::descriptions) std::cout << description << " ";
    std::cout << '\n';
    std::cout << "Available players: ";
    for(auto& description : Context::descriptions) std::cout << description << " ";
    std::cout << '\n';
}
