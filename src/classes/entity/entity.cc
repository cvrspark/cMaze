#include "entity.hh"
#include <cstdlib>
#include <string>
#include <vector>

using std::string;

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

    static std::vector<std::pair<int, int>> alphaHistory;
    static std::vector<std::pair<int, int>> betaHistory;
    static std::vector<std::pair<int, int>> deltaHistory;
    
    std::vector<std::pair<int, int>>* myHistory = &alphaHistory;
    if (model == "Bot Beta") {
        myHistory = &betaHistory;
    } else if (model == "Bot Delta") {
        myHistory = &deltaHistory;
    }

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    struct Move { int x, y; bool isGoal; bool isVisited; };
    std::vector<Move> unvisitedMoves;
    std::vector<Move> fallbackMoves;

    for (int i = 0; i < 4; ++i) {
        int tx = currentX + dx[i];
        int ty = currentY + dy[i];
        int tile = map.getTile(tx, ty);

        if (tile != 1) {
            bool visited = false;
            for (const auto& p : *myHistory) {
                if (p.first == tx && p.second == ty) {
                    visited = true;
                    break;
                }
            }

            if (tile == 2) {
                pos.x = tx;
                pos.y = ty;
                return;
            }

            if (!visited) {
                unvisitedMoves.push_back({tx, ty, false, false});
            } else {
                fallbackMoves.push_back({tx, ty, false, true});
            }
        }
    }

    int nextX = currentX;
    int nextY = currentY;

    if (!unvisitedMoves.empty()) {
        int idx = rand() % unvisitedMoves.size();
        nextX = unvisitedMoves[idx].x;
        nextY = unvisitedMoves[idx].y;
    } else if (!fallbackMoves.empty()) {
        int idx = rand() % fallbackMoves.size();
        nextX = fallbackMoves[idx].x;
        nextY = fallbackMoves[idx].y;
    }

    pos.x = nextX;
    pos.y = nextY;

    myHistory->push_back({currentX, currentY});
    if (myHistory->size() > 14) {
        myHistory->erase(myHistory->begin());
    }
}