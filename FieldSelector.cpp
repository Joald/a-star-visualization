#include <iostream>
#include "FieldSelector.h"

namespace {

std::tuple<int, int, int, int> findEnds(const FieldMatrix& matrix) {
    int startX = 0, startY = 0, targetX = 0, targetY = 0;
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[i].size(); ++j) {
            if (matrix[i][j] == START) {
                startX = i;
                startY = j;
            }
            if (matrix[i][j] == TARGET) {
                targetX = i;
                targetY = j;
            }
        }
    }
    return {startX, startY, targetX, targetY};

}

} // namespace


Converter initConverter() {
    Converter map;
    for (auto i : {'S', 's'}) {
        map.emplace(i, START);
    }
    for (auto i : {'T', 't'}) {
        map.emplace(i, TARGET);
    }
    for (auto i : {'B', 'b', 'X', 'x'}) {
        map.emplace(i, BLOCKING);
    }
    for (auto i : {'N', 'n', 'O', 'o'}) {
        map.emplace(i, NON_BLOCKING);
    }
    return map;
}

FieldMatrix load_fields() {
    FieldMatrix matrix;
    auto& map = getConverter();
    std::ifstream f("map.txt");
    std::string s;
    while (f >> s) {
        auto& row = matrix.emplace_back();
        for (auto c : s) {
            row.emplace_back(map[c]);
        }
    }
    if (matrix.empty()) {
        throw std::logic_error("Matrix was not loaded!");
    }
    return matrix;
}

Converter& getConverter() {
    static Converter converter = initConverter();
    return converter;
}

void FieldSelector::visitNext() {
    if (finished) {
        return;
    }
    if (std::tie(startX, startY) == std::tie(targetX, targetY)) {
        finished = true;
        markPath();
    }
    switch (algorithm) {
        case UNSPECIFIED:
            break;
        case DIJKSTRA: {
            int weight, x, y;
            std::tie(weight, x, y) = dijkstra.top();
            dijkstra.pop();
            if (matrix[x][y] == Field::VISITED) {
                return;
            }
            matrix[x][y] = Field::VISITED;
            weight++;
            auto visit = [&](int vx, int vy) {
                if (canVisit(vx, vy, weight)) {
                    prevs[vx][vy] = {x, y};
                    dijkstra.emplace(weight, vx, vy);
                    weights[vx][vy] = weight;
                    matrix[vx][vy] = Field::ENQUEUED;
                    if (vx == targetX and vy == targetY) {
                        finished = true;
                        std::clog << "Target found!" << std::endl;
                        markPath();
                    }
                }
            };
            visit(x + 1, y);
            visit(x - 1, y);
            visit(x, y + 1);
            visit(x, y - 1);
            if (dijkstra.empty()) {
                finished = true;
                std::clog << "Queue empty!" << std::endl;
            }
        }
            break;
        case A_STAR:
            break;
        case A_STAR_PREFER_COST:
            break;
        case A_STAR_PREFER_HEURISTIC:
            break;
    }
}

FieldSelector::FieldSelector(Algorithm algorithm, FieldMatrix& matrix)
  : algorithm(algorithm), matrix(matrix), prevs(matrix.size()), weights(matrix.size()), finished(false) {
    for (int i = 0; i < matrix.size(); ++i) {
        prevs[i].resize(matrix[i].size(), {-1, -1});
        weights[i].resize(matrix[i].size(), std::numeric_limits<weight>::max());
    }
    switch (algorithm) {
        case UNSPECIFIED:
            break;
        case DIJKSTRA: {
            std::tie(startX, startY, targetX, targetY) = findEnds(matrix);
            dijkstra.emplace(0, startX, startY);
            weights[startX][startY] = 0;
            break;
        }
        case A_STAR:{
            std::tie(startX, startY, targetX, targetY) = findEnds(matrix);
            dijkstra.emplace(distance(startX, startY), startX, startY);
            weights[startX][startY] = 0;
            break;
        }
        case A_STAR_PREFER_COST:
            break;
        case A_STAR_PREFER_HEURISTIC:
            break;
    }
}

void FieldSelector::markPath() {
    auto x = targetX, y = targetY;
    do {
        matrix[x][y] = PATH;
        std::tie(x, y) = prevs[x][y];
    } while (std::make_pair(x, y) != std::make_pair(-1, -1));
}
