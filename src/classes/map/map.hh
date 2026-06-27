#pragma once
#include <vector>

class Map {
private:
    int width;
    int height;
    std::vector<std::vector<int>> layout;

    void generateMaze();

public:
    Map(int w, int h);
    void regenerate();
    int getTile(int x, int y) const;
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};