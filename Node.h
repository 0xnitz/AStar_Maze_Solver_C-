#include "Position.h"

// This class represent a node in our maze
class Node {
public:
    Node *parent;
    Position *position;
    float f,g,h;

    Node(Position position) {
        parent = NULL;
        this->position = new Position(position);
        f = g = h = 0;
    }

    Node(Node *parent, Position position) {
        this->parent = parent;
        this->position = new Position(position);
        f = g = h = 0;
    }

    Node(Node *parent, Position position, float f, float g, float h) {
        this->parent = parent;
        this->position = new Position(position);
        this->f = f;
        this->g = g;
        this->h = h;
    }

    // The == operator checks if two nodes are identical by comparing their positions
    bool operator==(const Node other) const {
        return position->x == other.position->x && position->y == other.position->y;
    }
};
