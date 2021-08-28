#ifndef OLYMPUS_WINDOW_H
#define OLYMPUS_WINDOW_H

#include "io/6abstractIO.h"
#include "headQ_rect.h"
using uint8_t = unsigned char;
class Target; class AbstractIO;

class UIElement {
public:
    Rect coords;
    bool isWindow;

    static DirectioL direction;
    static constexpr bool vertical = true, horizontal = false;
    UIElement() = default;
    UIElement(int top, int left, int width, int height) { init(top, left, width, height); };
    virtual ~UIElement() = default;
    void init(int t, int l, int w, int h) { coords = Rect(t, l, h, w); } 
    virtual Target* iterate(char requs, bool needstarget) = 0;

    virtual void erase_background(AbstractIO* io) const;
};

class GalleryElement: public UIElement {
protected:
    bool orientation;
    std::vector<UIElement*> subWins;
public:
    GalleryElement(uint8_t subwins, bool orientation): orientation(orientation), subWins(subwins, nullptr) {}
    GalleryElement(uint8_t subwins, bool orientation, int top, int left, int width, int height):
        GalleryElement(subwins, orientation) { init(top, left, width, height); }
    virtual ~GalleryElement() { /*delete[] subWins;*/ }

    UIElement* getElement( int index ) { return subWins[index]; }
};

class ListElement: public GalleryElement {
protected:
    int yOffset, zOffset;
public:
    ListElement(int top, int left, int width, int height, int offset, int maxItems, bool direction);

    Target* iterate(char requs, bool needstarget) override;
    Rect get_coordinates(int *yOff, int *zOff) const;
    void get_coordinates(int *y, int *z, int *yOff, int *zOff) const;
    void get_coordinates(int itemNb, int *y, int *z) const;
};

class ModernElement: public GalleryElement {
    int* endLines;
public:
    ModernElement(uint8_t subwins, bool orientation): GalleryElement(subwins, orientation), endLines(new int[subwins-1]) {}
    ModernElement(uint8_t subwins, bool orientation, int top, int left, int width, int height): ModernElement(subwins, orientation) {
        init(top, left, width, height);
    }
    ~ModernElement() { delete[] endLines; }
    void addElement(UIElement* element, int pos);
    void addLastElement(UIElement *element);
    Target* iterate(char requs, bool needstarget) override;
};

template<typename T>
class Sprite: public UIElement {
    T* target;

    Target* iterate(char requs, bool needstarget) override;
};

class Rectangle: public UIElement {
public:
    Rectangle(int top, int left, int width, int height): UIElement(top, left, width, height) {};
    Target* iterate(char requs, bool needstarget) override {
        (void) requs; (void) needstarget;
        return nullptr;
    }
};

#endif //OLYMPUS_WINDOW_H
