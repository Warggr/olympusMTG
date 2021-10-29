#include "lib3_cliUI.h"
#include "gameplay/1general.h"
#include "gameplay/optionwrappers.h"
#include "control/3player.h"
#include "headE_enums.h"
#include "boost/format.hpp"

struct Command {
    using zone = zone::zone;
    enum command { cd = 0, ls, sel, view, back, pwd, help, concede, pass } _command;
    static const char* descriptions[9];
    union {
        zone _where;
        Target* _who;
    } _what;
    Command() = default;
    Command(command cmd, zone zn): _command(cmd) { _what._where = zn; }
    Command(command cmd, Target* tgt): _command(cmd) { _what._who = tgt; }
};

const char* zone::descriptions[] = { "hand", "graveyard", "battlefield", "stack", "exile", "commandzone" };
const char* Command::descriptions[] = { "cd", "ls", "sel", "view", "back", "pwd", "help", "exit", "pass" };

bool readCommand(std::string command, Command::zone zone, Command& output, Player* pl);
void help();

Option* CliUI::chooseOpt(bool sorcerySpeed) {
    (void) sorcerySpeed; //TODO
    Command::zone zone = zone::hand;
    while(true) {
        Command cmd;
        while(true) {
            std::string input = io.getTextInput(
                    (boost::format("%s@local:%s>") % pl->getName() % zone::descriptions[zone]).str().c_str(),
                    false);
            if(readCommand(input, zone, cmd, pl)) break;
        }
        switch(cmd._command){
            case Command::cd: zone = cmd._what._where; break;
            case Command::ls: list(cmd._what._where); break;
            case Command::sel: return dynamic_cast<OptionWrapper*>(cmd._what._who)->chooseOptionAction();
            case Command::view: cmd._what._who->disp(&io); break;
            case Command::back: return nullptr;
            case Command::pwd: std::cout << zone::descriptions[zone] << '\n'; break;
            case Command::help: help(); break;
            case Command::concede: throw UIClosedException();
            case Command::pass: return nullptr;
        }
    }
}

Target* readTarget(const char* str, Command::zone zone, Player* pl) {
    int pos = str[0] - '0';
    if(0 <= pos and pos <= 10) {
        switch(zone) {
        case zone::hand: {
            auto iter = pl->getHand().begin();
            std::advance(iter, pos);
            return iter->get();
        }
        default:
            std::cout << "Zone not implemented yet.\n";
        }
    }
    std::cout << "Unrecognized target: '" << str << "'\n";
    return nullptr; //TODO
}

bool read(const char* str, const char** opts, uint nbopts, uint& ret, uint& j) {
    for(ret=0; ret<nbopts; ++ret) {
        for(j=0; opts[ret][j] != '\0'; ++j) if(str[j] != opts[ret][j]) break;
        if(opts[ret][j] == '\0' and (str[j] == ' ' or str[j] == '\n' or str[j] == '\0')) break;
    }
    return (ret != nbopts);
}

bool readZone(const char* str, Command::zone& output) {
    constexpr int nb_commands = sizeof zone::descriptions / sizeof (const char*);
    uint i, j;
    if(! read(str, zone::descriptions, nb_commands, i, j)) {
        std::cout << "Unrecognized zone: '" << str << "'\n";
        return false;
    }
    else output = static_cast<Command::zone>(i);
    return true;
}

bool readCommand(std::string command, Command::zone zone, Command& output, Player* pl) {
    constexpr int nb_commands = sizeof Command::descriptions / sizeof (const char*);
    uint i, j;
    if(!read(command.c_str(), Command::descriptions, nb_commands, i, j)) {
        std::cout << "Unrecognized command: " << command << ". Try 'help'.\n"; return false;
    }
    output._command = static_cast<Command::command>(i);
    switch(output._command) {
        case Command::view:
        case Command::sel:
            output._what._who = readTarget(command.c_str()+j+1, zone, pl);
            return (output._what._who != nullptr);
        case Command::cd:
            return readZone(command.c_str()+j+1, output._what._where);
        case Command::ls:
            output._what._where = zone; [[fallthrough]];
        default:
            return true;
    }
}

void help() {
    std::cout << "Available commands: ";
    for(auto& description : Command::descriptions) std::cout << description << " ";
    std::cout << '\n';
    std::cout << "Available zones: ";
    for(auto& description : zone::descriptions) std::cout << description << " ";
    std::cout << '\n';
}
