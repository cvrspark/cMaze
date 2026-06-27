#include "map.hh"
#include <cstdlib>
#include <vector>

using std::vector;

Map::Map(int w, int h) : width(w), height(h) {
    regenerate();
}

void Map::regenerate() {
    layout.assign(height, vector<int>(width, 1));
    generateMaze();
}

void Map::generateMaze() {
    layout.assign(height, vector<int>(width, 1));

    vector<std::pair<int, int>> stack;
    int startX = 1, startY = 1;
    layout[startY][startX] = 0;
    stack.push_back({startX, startY});

    int dx[] = {0, 0, 2, -2};
    int dy[] = {2, -2, 0, 0};

    while (!stack.empty()) {
        auto [cx, cy] = stack.back();
        vector<vector<int>> neighbors;

        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];

            if (nx > 0 && nx < width - 1 && ny > 0 && ny < height - 1) {
                if (layout[ny][nx] == 1) {
                    neighbors.push_back({nx, ny, dx[i], dy[i]});
                }
            }
        }

        if (!neighbors.empty()) {
            int idx = rand() % neighbors.size();
            int nx = neighbors[idx][0];
            int ny = neighbors[idx][1];
            int choiceDx = neighbors[idx][2];
            int choiceDy = neighbors[idx][3];

            layout[cy + choiceDy / 2][cx + choiceDx / 2] = 0;
            layout[ny][nx] = 0;

            stack.push_back({nx, ny});
        } else {
            stack.pop_back();
        }
    }

    bool flagPlaced = false;
    while (!flagPlaced) {
        int fx = width / 2 + rand() % (width / 2 - 1);
        int fy = 1 + rand() % (height - 2);
        if (layout[fy][fx] == 0) {
            layout[fy][fx] = 2;
            flagPlaced = true;
        }
    }
}

int Map::getTile(int x, int y) const {
    if (x < 0 || x >= width || y < 0 || y >= height) return 1;
    return layout[y][x];
}