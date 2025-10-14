// Pathfinder.h
#pragma once
#include <vector>
#include <glm/glm.hpp>

class TileMap; // forward declaration

class Pathfinder {
public:
    Pathfinder(TileMap* map);
    bool findPath(const glm::ivec2& start, const glm::ivec2& goal, std::vector<glm::ivec2>& path);

private:
    TileMap* tilemap;
    int width, height;

    bool isValid(int x, int y);
};
