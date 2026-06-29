#include "entity.hh"
#include <cstdlib>
#include <string>
#include <vector>

using std::string;
using std::vector;
using std::pair;

Entity::Entity(Position pos, double maxSpeed, string model, string avatar, bool isBot)
    : pos(pos), maxSpeed(maxSpeed), speed(maxSpeed), model(model), avatar(avatar), isBot(isBot) {}

void Entity::updatePhysics() {}

void Entity::moveControlled(char input, const Map& map) {
    int nextX = static_cast<int>(pos.x);
    int nextY = static_cast<int>(pos.y);

    if (input == 'w' || input == 'W') nextY -= 1;
    if (input == 's' || input == 'S') nextY += 1;
    if (input == 'a' || input == 'A') nextX -= 1;
    if (input == 'd' || input == 'D') nextX += 1;

    if (map.getTile(nextX, nextY) != 1) {
        pos.x = nextX;
        pos.y = nextY;
    }
}

void Entity::updateBotAI(const Map& map) {
    int currentX = static_cast<int>(pos.x);
    int currentY = static_cast<int>(pos.y);

    static vector<pair<int, int>> alphaHistory;
    static vector<pair<int, int>> betaHistory;
    static vector<pair<int, int>> deltaHistory;
    
    static int heatMap[65][50] = {0}; 
    static int distanceField[65][50] = {0};
    
    static int lastWallCount = -1;
    static int lastGoalX = -1;
    static int lastGoalY = -1;

    int currentWallCount = 0;
    int currentGoalX = -1, currentGoalY = -1;
    
    for (int x = 0; x < 65; ++x) {
        for (int y = 0; y < 50; ++y) {
            int t = map.getTile(x, y);
            if (t == 1) currentWallCount++;
            if (t == 2) { currentGoalX = x; currentGoalY = y; }
        }
    }

    if (currentWallCount != lastWallCount || currentGoalX != lastGoalX || currentGoalY != lastGoalY) {
        lastWallCount = currentWallCount;
        lastGoalX = currentGoalX;
        lastGoalY = currentGoalY;
        
        alphaHistory.clear();
        betaHistory.clear();
        deltaHistory.clear();

        for(int x = 0; x < 65; ++x) {
            for(int y = 0; y < 50; ++y) {
                heatMap[x][y] = 0;
                distanceField[x][y] = 99999;
            }
        }
        if (currentGoalX != -1) {
            struct Point { int x, y, dist; };
            vector<Point> queue;
            queue.push_back({currentGoalX, currentGoalY, 0});
            distanceField[currentGoalX][currentGoalY] = 0;
            
            size_t head = 0;
            int qx[] = {1, -1, 0, 0};
            int qy[] = {0, 0, 1, -1};
            
            while (head < queue.size()) {
                Point p = queue[head++];
                for (int i = 0; i < 4; ++i) {
                    int nx = p.x + qx[i];
                    int ny = p.y + qy[i];
                    if (nx >= 0 && nx < 65 && ny >= 0 && ny < 50) {
                        if (map.getTile(nx, ny) != 1 && distanceField[nx][ny] == 99999) {
                            distanceField[nx][ny] = p.dist + 1;
                            queue.push_back({nx, ny, p.dist + 1});
                        }
                    }
                }
            }
        }
    }

    if (model != "Bot Omega") {
        vector<pair<int, int>>* myHistory = &alphaHistory;
        if (model == "Bot Beta")  myHistory = &betaHistory;
        if (model == "Bot Delta") myHistory = &deltaHistory;

        int dx[] = {1, -1, 0, 0};
        int dy[] = {0, 0, 1, -1};
        struct Move { int x, y; };
        vector<Move> unvisitedMoves;
        vector<Move> fallbackMoves;

        for (int i = 0; i < 4; ++i) {
            int tx = currentX + dx[i];
            int ty = currentY + dy[i];
            if (tx < 0 || tx >= 65 || ty < 0 || ty >= 50) continue;
            
            int tile = map.getTile(tx, ty);
            if (tile != 1) {
                bool visited = false;
                for (const auto& p : *myHistory) {
                    if (p.first == tx && p.second == ty) { visited = true; break; }
                }
                if (tile == 2) { pos.x = tx; pos.y = ty; return; }
                if (!visited) unvisitedMoves.push_back({tx, ty});
                else fallbackMoves.push_back({tx, ty});
            }
        }
        int nextX = currentX, nextY = currentY;
        if (!unvisitedMoves.empty()) {
            int idx = rand() % unvisitedMoves.size();
            nextX = unvisitedMoves[idx].x; nextY = unvisitedMoves[idx].y;
        } else if (!fallbackMoves.empty()) {
            int idx = rand() % fallbackMoves.size();
            nextX = fallbackMoves[idx].x; nextY = fallbackMoves[idx].y;
        }
        pos.x = nextX; pos.y = nextY;
        myHistory->push_back({currentX, currentY});
        if (myHistory->size() > 14) myHistory->erase(myHistory->begin());
        return;
    }

    heatMap[currentX][currentY] += 1;

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    struct MoveCost { int x, y, totalCost; };
    vector<MoveCost> validMoves;

    for (int i = 0; i < 4; ++i) {
        int tx = currentX + dx[i];
        int ty = currentY + dy[i];
        
        if (tx < 0 || tx >= 65 || ty < 0 || ty >= 50) continue;

        int tile = map.getTile(tx, ty);
        if (tile == 1) continue;
        if (tile == 2) {
            pos.x = tx;
            pos.y = ty;
            return;
        }

        int cost = (heatMap[tx][ty] * 10000) + distanceField[tx][ty];
        
        validMoves.push_back({tx, ty, cost});
    }

    if (!validMoves.empty()) {
        int minCost = 2147483647; 
        for (const auto& move : validMoves) {
            if (move.totalCost < minCost) minCost = move.totalCost;
        }

        vector<MoveCost> bestMoves;
        for (const auto& move : validMoves) {
            if (move.totalCost == minCost) bestMoves.push_back(move);
        }

        int idx = rand() % bestMoves.size();
        int nextX = bestMoves[idx].x;
        int nextY = bestMoves[idx].y;

        heatMap[nextX][nextY] += 5; 
        
        pos.x = nextX;
        pos.y = nextY;
    }
}