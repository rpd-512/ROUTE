#ifndef TYPES_H
#define TYPES_H

#include <cmath>
#include <iostream>

using namespace std;

struct Position {
    double x, y;
};

class Node {
public:
    int id;
    Position position;
    double radius;
    double energy;
    bool isAlive;
    int packetCount;
    double txCost;
    double rxCost;
    bool isSink = false; // Default to false, can be set later

    Node(int id,
         Position position,
         double radius,
         double energy = 100.0,
         int packetCount = 512,
         double txCost = 1.0,
         double rxCost = 1.0,
         bool isSink = false)
        : id(id),
          position(position),
          radius(radius),
          energy(energy < 0 ? 0 : energy),
          isAlive(true),
          packetCount(packetCount),
          txCost(txCost),
          rxCost(rxCost),
          isSink(isSink) {}
    
    double distanceTo(const Node& other) const {
        double dx = other.position.x - position.x;
        double dy = other.position.y - position.y;
        return std::sqrt(dx * dx + dy * dy);
    }

    bool isInRange(const Node& other) const {
        return distanceTo(other) <= radius && other.isAlive && isAlive;
    }

    void depleteEnergy(double amount) {
        energy -= amount;
        if (energy <= 0) {
            energy = 0;
            isAlive = false;
        }
    }
    void reset() {
        energy = 100.0; // Reset energy to default value
        isAlive = true; // Reset alive status
    }
    void printStatus() const {
        std::cout << "Node ID: " << id
                  << ", Position: (" << position.x << ", " << position.y << ")"
                  << ", Energy: " << energy
                  << ", Is Alive: " << (isAlive ? "Yes" : "No")
                  << ", Packets: " << packetCount
                  << ", TX Cost: " << txCost
                  << ", RX Cost: " << rxCost
                  << ", Is Sink: " << (isSink ? "Yes" : "No")
                  << ", Radius: " << radius
                  << std::endl;
    }
};

#endif