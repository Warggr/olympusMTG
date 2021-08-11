#ifndef OLYMPUS_HEADQ_RECT_H
#define OLYMPUS_HEADQ_RECT_H

using uint16_t = unsigned short;

struct Rect{
    uint16_t y, z;
    uint16_t width, height;

    int zone() const {return y >> 11; }
    int yy() const {return y & 0x07ff; }
    int right() const {return y + width; }
    int bottom() const {return z+height; }

    Rect() = default;
    Rect(int _y, int _z, int _width, int _height): y((int) _y), z((int) _z), width((int) _width), height((int) _height){};
    void shift(int dy, int dz){y += dy; z += dz; }
};

#endif //OLYMPUS_HEADQ_RECT_H
