#ifndef OLYMPUS_WINDOW_H
#define OLYMPUS_WINDOW_H

#include "../io/6abstractIO.h"
#include "frontend/headQ_rect.h"
#include "gameplay/permanents/4permanents.h"
#include "gameplay/resolvables/5resolvables.h"
#include "gameplay/2cards.h"
#include "control/3player.h"
#include <iostream>
class Target; class AbstractIO;

/** The UI is a mapping from positions (VIEWS) to game objects (MODEL); from UIElements to Containers.
 * UI / VIEW is part of the agent; for example, a BotAgent needs no UI. Therefore, UI elements and on-screen positions can't be part of the game object,
 * since game objects are agent-agnostic.
 * We will use a pull-based observer for individual objects (PermanentTN -> Sprite), and a push variant for list objects. */

class UIElement {
    Rect _coords;
public:
    bool isWindow;

    static DirectioL direction;
    static AbstractIO* io;
    static constexpr bool vertical = true, horizontal = false;

    UIElement() = default;
    virtual ~UIElement() = default;
    virtual Target* iterate(char requs, bool needstarget) = 0;

    virtual void fullDisp(AbstractIO* io) const = 0;
    virtual void eraseBackground() const;

    void setCoords(const Rect& coords) {
        _coords = coords;
        io->draw_rectangle(AbstractIO::HIGH1, coords, 4);
        std::cout << coords.y << ',' << coords.z << ',' << coords.width << 'x' << coords.height << '\n';
    }
    const Rect& getCoords() const { return _coords; }
};

class LowestUIElement : public UIElement {
protected:
    virtual Target* getTarget(char requs) const = 0;
public:
    Target* iterate(char requs, bool needstarget) override {
        while (true) {
            direction = UIElement::io->get_direction_key();
            switch (direction) {
                case ENTER: {
                    Target *target = getTarget(requs);
                    if(target) return target;
                    else break;
                }
                case NOT_RECOGNIZED:
                    UIElement::io->message("Please press a direction key, SPACE or ENTER");
                    break;
                case BACK:
                    if(!needstarget) return nullptr;
                    break;
                default:
                    return nullptr;
            }
        }
    }
};

template<typename T>
class Sprite: public LowestUIElement {
protected:
    const T* target; //It is const just because there's no good reason for the UI to modify it.
    //However, since the real pointed object is not const, we'll regularly const_cast it when passing it back
public:
    Sprite(const T* content): target(content) {}
    Target* getTarget(char requs) const override {
        if(B_is_a_A(requs, target->targetType())) return const_cast<T*>(target);
        else return nullptr;
    }
    void fullDisp(AbstractIO *io) const override { io->draw(*target, getCoords(), false); }
};

class Rectangle: public LowestUIElement {
public:
    Target* getTarget(char) const override { return nullptr; }
    void fullDisp(AbstractIO*) const override {}; // a Rectangle is empty, fullDisp does nothing
    void init(const Rect& rect) { setCoords(rect); }
};

template<typename Gallery>
struct TemplateGallery: public UIElement {
    bool orientation;
    int element_size, offset;
    Gallery policy;
    using iterator = typename Gallery::iterator;

    TemplateGallery(bool orientation = Gallery::orientation):
            orientation(orientation), element_size(0), offset(0) {}
    TemplateGallery(const Gallery& policy, bool orientation = Gallery::orientation):
        orientation(orientation), element_size(0), offset(0), policy(policy) {}

    Target* iterate(char requs, bool needstarget) override {
        iterator iter;
        if(direction == RIGHT || direction == DOWN) iter = policy.begin();
        else { iter = policy.end(); --iter; }
        while(true) {
            Target* tar = iter->iterate(requs, needstarget);
            if(tar) return tar;
            if((direction == LEFT && orientation == vertical) or (direction == UP && orientation == horizontal) ) {
                if(iter == policy.begin()) return nullptr;
                --iter;
            } else if((direction == RIGHT && orientation == horizontal) or (direction == DOWN && orientation == vertical)) {
                ++iter;
                if(iter == policy.end()) return nullptr;
            } else {
                return nullptr;
            }
        }
    }

    void fullDisp(AbstractIO* io) const override {
        for(auto& i : policy) {
            i.fullDisp(io);
        }
    }
    Rect getCoordinates(int* yOffset, int* zOffset) const {
        Rect rect = getCoords();
        if(orientation == vertical) { *yOffset = 0; *zOffset = offset; rect.height = element_size; }
        else { *yOffset = offset; *zOffset = 0; rect.width = element_size; }
        return rect;
    }
    Rect getCoordinates(int index) const {
        Rect rect = getCoords();
        if(orientation == vertical) { rect.height = element_size; rect.z = index * (element_size + offset); }
        else { rect.width = element_size; rect.y = index * (element_size + offset); }
        return rect;
    }
    void getCoordinates(int pos, int* y, int* z) const {
        if(orientation == horizontal) {
            *y = getCoords().y + pos * (element_size + offset); *z = getCoords().z;
        } else {
            *z = getCoords().z + pos * (element_size + offset); *y = getCoords().y;
        }
    }
};

#endif //OLYMPUS_WINDOW_H
