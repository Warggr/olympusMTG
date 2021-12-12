#include "lib3_cliUI.h"
#include "gameplay/1general.h"
#include "gameplay/optionwrappers.h"
#include "control/3player.h"
#include "headE_enums.h"
#include "boost/format.hpp"

struct Context {
    zone::zone where;
    Target* what;
    Player* pl;
    unsigned char who : 2;
    constexpr static char const* descriptions[] = {"none", "opponent", "you", "all"};
    Context(Player* pl): where(zone::hand), what(nullptr), pl(pl), who(0b10) { }
    inline std::string prompt() const {
        return (boost::format("@%s:%s>")
                % Context::descriptions[who]
                % zone::descriptions[where]).str();
    }
};

namespace cmd {
    enum command { sel = 0, pass, concede, ls, view, cd, help };
    const char* descriptions[] = {"sel", "pass", "exit", "ls", "view", "cd", "help"};
}
const char* zone::descriptions[] = {"hand", "graveyard", "battlefield", "stack", "exile", "commandzone"};

bool readCommand(const char* input, Context& context, cmd::command& cmd);
void printHelp();

Option* CliUI::chooseOpt(bool sorcerySpeed) {
    using namespace cmd;
    (void) sorcerySpeed; //TODO
    static Context context(pl);
    while(true) {
        Context temporaryContext = context;
        command cmd = cd;
        while(true) {
            std::string input = io.getTextInput(context.prompt().c_str(),false);
            if(readCommand(input.c_str(), temporaryContext, cmd)) break;
        }
        switch(cmd){
            case cd: context = temporaryContext; break;
            case ls: list(temporaryContext.where); break;
            case sel: return dynamic_cast<OptionWrapper*>(temporaryContext.what)->chooseOptionAction();
            case view: temporaryContext.what->disp(&io); break;
            case help: printHelp(); break;
            case concede: throw UIClosedException();
            case pass: return nullptr;
        }
    }
}

Target* readTarget(const char* str, zone::zone zone, Player* pl) {
	std::cout << "Read target '" << str << "'\n";
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
    case zone::hand: if(pos < pl->getHand().size()) {
        auto iter = pl->getHand().begin();
        std::advance(iter, pos);
        return iter.operator->();
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

inline void readContext(const char* input, uint& offset, Context& context) {
    uint k, l;
    if(input[offset] == '#') {
        ++offset;
        if(read(input+offset, ENUMERATE(Context::descriptions), l, k)) {
	        context.who = l;
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

bool readCommand(const char* input, Context& context, cmd::command& cmd) {
    using namespace cmd;
    uint i, j;
    if(!read(input,ENUMERATE(descriptions), i, j)) return false;
    
    while(input[j] == ' ') ++j;
    readContext(input, j, context);
    cmd = static_cast<command>(i);
    if(cmd == view || cmd == sel){
        context.what = readTarget(input+j, context.where, context.pl);
        return (context.what != nullptr);
    }
    return true;
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
