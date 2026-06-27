#include "screen.hh"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using std::string;
using std::vector;
using std::cout;

Screen::Screen(Dimension dim) : dim(dim) {
    clear();
}

void Screen::clear() {
    grid.assign(dim.y, vector<string>(dim.x, "  ")); 
}

void Screen::setPixel(int x, int y, string visual) {
    if (x >= 0 && x < dim.x && y >= 0 && y < dim.y) {
        grid[y][x] = visual;
    }
}

void Screen::drawMap(const Map& map) {
    for (int y = 0; y < dim.y; ++y) {
        for (int x = 0; x < dim.x; ++x) {
            int tile = map.getTile(x, y);
            if (tile == 1) {
                grid[y][x] = "██"; 
            } else if (tile == 2) {
                grid[y][x] = "🚩"; 
            }
        }
    }
}

void Screen::render() {
    #ifdef _WIN32
    cout << "\x1B[H";
    #else
    cout << "\033[H";
    #endif

    std::stringstream buffer;

    for (int y = 0; y < dim.y; ++y) {
        for (int x = 0; x < dim.x; ++x) {
            buffer << grid[y][x];
        }
        buffer << "\n";
    }

    cout << buffer.str();
}