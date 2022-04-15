#ifndef OLYMPUS_HEADQ_RECT_H
#define OLYMPUS_HEADQ_RECT_H

using uint16_t = unsigned short;

struct Rect {
    uint16_t zone : 5;
    uint16_t y : 11, z;
    uint16_t width, height;

    int left() const { return y; }
    int top() const { return z; }
    int right() const {return y + width; }
    int bottom() const {return z + height; }

    Rect() = default;
    Rect(int y, int z, int width, int height): Rect(y, z, 0, width, height) {};
    Rect(int y, int z, int zone, int width, int height): zone(zone), y(y), z(z), width(width), height(height){};
    void shift(int dy, int dz){y += dy; z += dz; }
};

enum DirectioL { UP, DOWN, LEFT, RIGHT, BACK, ENTER, MOUSE, NOT_RECOGNIZED };

#endif //OLYMPUS_HEADQ_RECT_H
