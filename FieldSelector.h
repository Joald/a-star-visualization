#pragma once

#include <vector>
#include <map>
#include <fstream>
#include <queue>
#include <cmath>
#include <functional>


enum Algorithm {
    UNSPECIFIED, DIJKSTRA, A_STAR, A_STAR_SQUARE_DISTANCE, A_STAR_MANHATTAN
};

enum Field {
    START, TARGET, BLOCKING, NON_BLOCKING, VISITED, PATH, ENQUEUED
};
using FieldMatrix = std::vector<std::vector<Field>>;
using Converter = std::map<char, Field>;

Converter initConverter();

Converter& getConverter();

FieldMatrix load_fields();


template<class T>
struct comparator {
    using type = std::function<bool(const T&, const T&)>;
};

template<class T>
using comparator_t = typename comparator<T>::type;

class FieldSelector {
    using weight = double;

    Algorithm algorithm;
    FieldMatrix& matrix;
    std::vector<std::vector<std::pair<int, int>>> prevs;
    std::vector<std::vector<weight>> weights;

    using queue_t = std::pair<int, int>;
    comparator_t<queue_t> queue_cmp = [&](const queue_t& lhs, const queue_t& rhs) {
        auto[lx, ly] = lhs;
        auto[rx, ry] = rhs;
        return std::make_tuple(getWeight(lx, ly), lx, ly) > std::make_tuple(getWeight(rx, ry), rx, ry);
    };
    std::priority_queue<queue_t, std::vector<queue_t>, decltype(queue_cmp)> q{queue_cmp};

    bool finished;

    int startX, startY, targetX, targetY;

    weight distance(int x, int y) const {
        weight dx = targetX - x;
        weight dy = targetY - y;
        return std::sqrt(dx * dx + dy * dy);
    }

    bool canVisit(int x, int y, weight w) const {
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
        switch (algorithm) {
            case DIJKSTRA:
                return weights[x][y];
            case A_STAR:
                return weights[x][y] + distance(x, y);
            case A_STAR_SQUARE_DISTANCE:
                return weights[x][y] + distance(x, y) * distance(x, y);
            case A_STAR_MANHATTAN:
                return weights[x][y] + std::abs(x - targetX) + std::abs(y - targetY);
            case UNSPECIFIED:
                throw std::invalid_argument("Algorithm unsupported.");
        }
        return -1;
    }
};
