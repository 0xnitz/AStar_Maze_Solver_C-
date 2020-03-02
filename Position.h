//This class represents a point on a grid, or a position in our maze
class Position {
public:
    int x, y;

    Position() {
        x = y = 0;
    }

    Position(int x, int y) {
        this->x = x;
        this->y = y;
    }

    // Copy constructor
    Position(const Position &other) {
        x = other.x;
        y = other.y;
    }

    // The == operator that checks if two position are identical 
    bool operator==(const Position other) const {
        return x == other.x && y == other.y;
    }
};
