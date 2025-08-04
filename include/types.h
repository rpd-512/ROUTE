#ifndef TYPES_H
#define TYPES_H

#include <cmath>

class Node {
public:
    int id;
    double x, y; // Coordinates
    double energy; // Energy level
    double isAlive;
    
    Node(int id, double x, double y) : id(id), x(x), y(y) {}

    double distanceTo(const Node& other) const {
        double dx = other.x - x;
        double dy = other.y - y;
        return std::sqrt(dx * dx + dy * dy);
    }
};
#endif