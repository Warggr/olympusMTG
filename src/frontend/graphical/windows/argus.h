#ifndef OLYMPUS_ARGUS_H
#define OLYMPUS_ARGUS_H

#include "window.h"
#include "templates.h"

/* local ()	poster		option ()
   permanents 	stack ()	message
   local ()				logbook ()*/

class Player; class Planeswalker; class Resolvable; class Option;

struct PlayerHeader: public Sprite<Player> {
    PlayerHeader(): Sprite<Player>(nullptr) {}
    inline void setPlayer(Player* player) { std::cout << "Set target to " << player << "\n"; target = player; }
    void fullDisp(AbstractIO* io) const override {
        io->draw(*target, coords, false);
    }
};

template<typename T>
struct BoardRow: public ListElement<Sprite<T>> {
    static constexpr bool orientation = UIElement::horizontal;

    void init(const Rect& rect) {
        this->element_size = rect.height; this->offset = 0.2 * rect.height;
    }
};

struct BoardSidePolicy {
    static constexpr bool orientation = UIElement::vertical;
    BoardRow<Creature> attackers, creatures;
    BoardRow<Planeswalker> planeswalkers;
    BoardRow<Permanent> artifacts, lands;

    void init(const Rect& rect, int permanentZSize, int permanentZMargin);

    struct iterator {
        enum positions { attacker, creature, planeswalker, artifact, land };
        BoardSidePolicy* owner;
        uint where;
        iterator() = default;
        iterator(BoardSidePolicy* owner, uint where): owner(owner), where(where) {};
        iterator& operator++() { where++; return *this; }
        iterator& operator--() { where--; return *this; }
        bool operator==(const iterator& other) const { return other.where == where and other.owner == owner; }
        bool operator!=(const iterator& other) const { return !operator==(other); }
        const UIElement& operator*() const { return *operator->(); }
        UIElement* operator->() const {
            switch(where) {
                case attacker: return &owner->attackers;
                case creature: return &owner->creatures;
                case planeswalker: return &owner->planeswalkers;
                case artifact: return &owner->artifacts;
                case land: return &owner->lands;
                default: return nullptr;
            }
        }
    };
    iterator begin() { return iterator(this, 0); }
    iterator begin() const { return iterator(const_cast<BoardSidePolicy*>(this), 0); }
    iterator end() { return iterator(this, 5); }
    iterator end() const { return iterator(const_cast<BoardSidePolicy*>(this), 5); }
};

struct BoardSide : public TemplateGallery<BoardSidePolicy> {};

struct PlayerSide : public TemplateGallery<PolicyWith2Objects<PlayerHeader, BoardSide, UIElement::vertical>> {
    PlayerSide() {
        std::cout << "Create PlayerSide\n";
    }
    void init(const Rect& rect);
    inline PlayerHeader& header() { return policy.object1; }
    inline BoardSide& board() { return policy.object2; }
};

struct Board : public TemplateGallery<ContainerPolicy<std::array<PlayerSide, 2>, PlayerSide>> {
    Board(): TemplateGallery<ContainerPolicy<std::array<PlayerSide, 2>, PlayerSide>>(vertical) {};
    void init(const Rect& rect);
    inline std::array<PlayerSide, 2>& players() { return policy.subWins; }
};

struct StackWindow: public VectorElement<Sprite<Resolvable>> {};

struct LeftBar: public TemplateGallery<PolicyWith2Objects<Rectangle, StackWindow, UIElement::vertical>> {
    void init(const Rect& rect);
    inline Rectangle& poster() { return policy.object1; }
    inline StackWindow& stack() { return policy.object2; }
};

struct OptionZone: public ListElement<Sprite<Card>> {
    OptionZone();
    void init(const Rect& rect);
};

struct RightBar: public TemplateGallery<PolicyWith3Objects<OptionZone, Rectangle, Rectangle, UIElement::vertical>> {
    void init(const Rect& rect);
    inline OptionZone& optionZone() { return policy.object1; }
    inline Rectangle& messageZone() { return policy.object2; }
    inline Rectangle& logbookZone() { return policy.object2; }
};

class Screen: public TemplateGallery<PolicyWith3Objects<LeftBar, Board, RightBar, UIElement::horizontal>> {
public:
    static int linesize;
    inline LeftBar& leftBar() { return policy.object1; }
    inline Board& board() { return policy.object2; }
    inline RightBar& rightBar() { return policy.object3; }

    void initScreen(int width, int height);
};

#endif //OLYMPUS_ARGUS_H
