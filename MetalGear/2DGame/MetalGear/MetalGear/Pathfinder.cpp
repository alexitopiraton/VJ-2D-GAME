// Pathfinder.cpp
#include "Pathfinder.h"
#include "TileMap.h"
#include <queue>
#include <algorithm>
#include <iostream>

Pathfinder::Pathfinder(TileMap* map) {
    tilemap = map;
    width = map->getWidth();
    height = map->getHeight();
}

bool Pathfinder::isValid(int x, int y) {
    return x >= 0 && y >= 0 && x < width && y < height && tilemap->isWalkable(x, y);
}

bool Pathfinder::findPath(const glm::ivec2& start, const glm::ivec2& goal, std::vector<glm::ivec2>& path) {
    path.clear();

    std::queue<glm::ivec2> q;
    std::vector<std::vector<bool>> visited(height, std::vector<bool>(width, false));
    std::vector<std::vector<glm::ivec2>> parent(height, std::vector<glm::ivec2>(width, { -1,-1 }));

    q.push(start);
    visited[start.y][start.x] = true;

    int dx[4] = { 1, -1, 0, 0 };
    int dy[4] = { 0, 0, 1, -1 };

    while (!q.empty()) {
        glm::ivec2 current = q.front(); q.pop();

        if (current == goal) {
            // reconstruir camino
            glm::ivec2 n = goal;
            while (n != start) {
                path.push_back(n);
                n = parent[n.y][n.x];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return true;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = current.x + dx[i];
            int ny = current.y + dy[i];

            if (!isValid(nx, ny) || visited[ny][nx]) continue;

            visited[ny][nx] = true;
            parent[ny][nx] = current;
            q.push({ nx, ny });
        }
    }

    return false; // no hay camino
}
