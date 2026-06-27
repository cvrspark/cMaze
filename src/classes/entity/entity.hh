#pragma once
#include <string>
#include "../map/map.hh"

struct Position {
    double x;
    double y;
};

class Entity {
private:
    std::string model;
    std::string avatar;
    bool isBot;

protected:
    Position pos;
    double speed;
    double maxSpeed;

public:
    Entity(Position pos, double maxSpeed, std::string model, std::string avatar, bool isBot = false);
    virtual ~Entity() = default;

    void updatePhysics();
    void moveControlled(char input, const Map& map);
    void updateBotAI(const Map& map);

    Position getPos() const { return pos; }
    std::string getAvatar() const { return avatar; }
    std::string getModel() const { return model; }
};