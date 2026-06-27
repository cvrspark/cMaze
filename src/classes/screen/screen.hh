#pragma once
#include <vector>
#include <string>
#include "../map/map.hh"

struct Dimension {
    int x;
    int y;
};

class Screen {
private:
    Dimension dim;
    std::vector<std::vector<std::string>> grid;

public:
    Screen(Dimension dim);
    virtual ~Screen() = default;

    void clear();
    void setPixel(int x, int y, std::string visual);
    void drawMap(const Map& map);
    void render();
    
    Dimension getDim() const { return dim; }
};