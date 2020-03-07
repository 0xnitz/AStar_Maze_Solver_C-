// Credit to https://medium.com/@nicholas.w.swift/easy-a-star-pathfinding-7e6689c7f7b2

#include <iostream>
#include <cmath>
#include <list>
#include <ctime>
#include <cstdlib>
#include <unistd.h>
#include <algorithm>

#include "Node.h"

const int SIZE = 35;

std::list<Position> astar(int maze[SIZE][SIZE], Position start, Position end);
template<class T>
T get(std::list<T> _list, int _i);
std::list<Node> remove_all(std::list<Node> list, Position item);
void generate_maze(int maze[SIZE][SIZE]);
void display_path(int maze[SIZE][SIZE], std::list<Position> path);
int get_index(std::list<Position> path, Position node);

int main() {
    int maze[SIZE][SIZE];
    std::cout << "Welcome\nGenerating Maze..." << "\r\n";
    generate_maze(maze);

    Position start(0, 0);
    Position end(SIZE - 1, SIZE - 1);

    std::cout << "Solving Maze..." << "\r\n";
    std::list<Position> path = astar(maze, start, end);
}

std::list<Position> astar(int maze[SIZE][SIZE], Position start, Position end) {
    // Create the path list
    std::list<Position> path;

    // Create start and end node
    Node start_node = Node(NULL, start);
    start_node.g = start_node.h = start_node.f = 0;
    Node end_node = Node(NULL, end);
    end_node.g = end_node.h = end_node.f = 0;

    // Initialize both open and closed list
    std::list<Node> open_list;
    std::list<Node> closed_list;

    // Add the start node
    open_list.push_back(start_node);
    
    // Loop until you find the end
    while (open_list.size() > 0) {
        // Get the current node
        Node current_node = get<Node>(open_list, 0);
        int i;

        for (i = 0; i < open_list.size(); i++) {
            if (get<Node>(open_list, i).f < current_node.f) {
                get<Node>(open_list, i);
                current_node = get<Node>(open_list, i);
            }
        }
        
        // Pop current off open list, add to closed list
        open_list = remove_all(open_list, *current_node.position);
        closed_list.push_back(current_node);

        // Add current_node to final path
        path.push_back(*current_node.position);
        display_path(maze, path);

        // Found the goal
        if (current_node.position->x == end_node.position->x && current_node.position->y == end_node.position->y) {
            return path;
        }

        // Generate children from neighbors
        std::list<Node> children;
        std::list<Position> neighbors = { Position(0, -1), Position(0, 1), Position(-1, 0), Position(1, 0), Position(-1, -1), Position(-1, 1), Position(1, -1), Position(1, 1) };
        
        for (i = 0; i < neighbors.size(); i++) {
            Position it = get<Position>(neighbors, i);

            // Get node position
            Position node_position(current_node.position->x + it.x, current_node.position->y + it.y);

            // Make sure within range
            if (node_position.x > (SIZE - 1) || node_position.x < 0 || node_position.y > (SIZE -1) || node_position.y < 0) {
                continue;
            }

            // Make sure walkable terrain
            if (maze[node_position.x][node_position.y] != 0) {
                continue;
            }

            // Create new node
            Node new_node(&current_node, node_position);

            // Append valid children to the children list
            children.push_back(new_node);

        }

        // Loop through children
        std::list<Node>::iterator child;
        int j = 0;
        for (child = children.begin(); child != children.end(); child++) {
            // Child is on the closed list
            std::list<Node>::iterator closed_child;
            for (closed_child = closed_list.end(); closed_child != closed_list.end(); closed_child++) {
                if (child == closed_child)
                    continue;
            }

            // Create the f, g, and h values
            child->g = current_node.g + 1;
            child->h = (pow((child->position->x - end_node.position->x), 2)) + (pow((child->position->y - end_node.position->y), 2));
            child->f = child->g + child->h;

            // Child is already in the open list
            std::list<Node>::iterator open_node;
            for (open_node = open_list.end(); open_node != open_list.end(); open_node++) {
                if (child == open_node && child->g > open_node->g)
                    continue;
            }

            // Add the child to the open list
            Node child_node(child->parent, *child->position, child->f, child->g, child->h);
            open_list.push_back(child_node);
        }
    }
}


// This function returns the ith element of a list
template<class T>
T get(std::list<T> _list, int _i) {
    auto it = _list.begin();
    for(int i=0; i<_i; i++){
        ++it;
    }
    return *it;
}

// This function removes all nodes with a position equal to item
std::list<Node> remove_all(std::list<Node> list, Position item) {
    std::list<Node>::iterator it;
    std::list<Node> new_list;
    for (it = list.begin(); it != list.end(); it++) {
        if (!(*it->position == item)) new_list.push_back(*it);
    }

    return new_list;
}

// Generating the maze
void generate_maze(int maze[SIZE][SIZE]) {
    srand(time(NULL));
    int i, j;
    double random;

    for (i = 0; i < SIZE; i++) {
        for (j = 0; j < SIZE; j++) {
            random = ((double) rand() / (RAND_MAX));
            if (random < 0.3) maze[i][j] = 1;
            else maze[i][j] = 0;
        }
    }
}

// Returns the index of a node in a position list
int get_index(std::list<Position> path, Position node) {
    int index;
    std::list<Position>::iterator it = path.begin();

    for (index = 0; it != path.end(); index++) {
        if (Position(it->x, it->y) == node) return index;
        it++;
    }

    return -1;
}

// Displaying the path of a maze
void display_path(int maze[SIZE][SIZE], std::list<Position> path) {
    int i, j;
    int frame;

    std::list<Position>::iterator node = path.begin();
    std::system("clear");

    for (i = 0; i < SIZE; i++) {
        std::cout << "|";

        for (j = 0; j < SIZE; j++) {
            if (maze[i][j])
                std::cout << "#";
            else {
                Position current(i, j);
                int index = get_index(path, current);

                if (std::find(path.begin(), path.end(), current) != path.end())
                    std::cout << "\033[1;31m*\033[0m";
                else
                    std::cout << " ";
            }
        }
        std::cout << "|\r\n";
    }
    usleep(200000);
}
