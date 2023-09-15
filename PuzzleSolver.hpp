#pragma once
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>
#include <algorithm>

class PuzzleSolver {

    struct Node {
        char direction; // 'U', 'D', 'L', 'R'
        std::vector<std::vector<int>> puzzle;
        Node *parent; 
        int cost; // depth + heuristics
        int depth; // number of moves
    };

    private:
        std::vector<std::vector<int>> _puzzle;
        int _lineLength;
        Node *_root;

    public:
        // Constructor and destructor
        PuzzleSolver(std::vector<std::vector<int>> puzzle, int lineLength);
        ~PuzzleSolver();

        // Solving methods
        int manhattanDistance(std::vector<std::vector<int>> puzzle);
        void addNeighbours(Node * currentNode, std::vector<Node *> &openList, std::vector<Node *> &closedList);
        std::vector<std::vector<int>> upPuzzle(std::vector<std::vector<int>> puzzle);
        std::vector<std::vector<int>> downPuzzle(std::vector<std::vector<int>> puzzle);
        std::vector<std::vector<int>> leftPuzzle(std::vector<std::vector<int>> puzzle);
        std::vector<std::vector<int>> rightPuzzle(std::vector<std::vector<int>> puzzle);

        // Getters
        int getLineLength() const;
        std::vector<std::vector<int>> getPuzzle() const;

        // Setters
        void setLineLength(int lineLength);
};