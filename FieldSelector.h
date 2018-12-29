#pragma once

#include <vector>
#include <map>
#include <fstream>
#include <queue>
#include <cmath>
#include <functional>


enum Algorithm {
    UNSPECIFIED, DIJKSTRA, A_STAR, A_STAR_PREFER_COST, A_STAR_PREFER_HEURISTIC
};

enum Field {
    START, TARGET, BLOCKING, NON_BLOCKING, VISITED, PATH, ENQUEUED
};
using FieldMatrix = std::vector<std::vector<Field>>;
using Converter = std::map<char, Field>;

Converter initConverter();

Converter& getConverter();

FieldMatrix load_fields();


class FieldSelector {
    using weight = float;

    Algorithm algorithm;
    FieldMatrix& matrix;
    std::vector<std::vector<std::pair<int, int>>> prevs;
    std::vector<std::vector<float>> weights;


    // weight, x, y
    using dijkstra_t = std::tuple<weight, int, int>;
    std::priority_queue<dijkstra_t, std::vector<dijkstra_t>, std::greater<>> dijkstra;
    
    bool finished;

    int startX, startY, targetX, targetY;

    weight distance(int x, int y) {
        float dx = targetX - x;
        float dy = targetY - y;
        return std::sqrt(dx * dx + dy * dy);
    }

    bool canVisit(int x, int y, weight w) {
        return !(x < 0 or matrix.size() <= x or
                 y < 0 or matrix[x].size() <= y or
                 matrix[x][y] == Field::BLOCKING or
                 matrix[x][y] == Field::VISITED or
                 w >= weights[x][y]);
    }

    void markPath();

public:
    FieldSelector(Algorithm algorithm, FieldMatrix& matrix);

    void visitNext();

    bool isFinished() const {
        return finished;
    }

    weight getWeight(int x, int y) const {
        return weights[x][y];
    }
};
