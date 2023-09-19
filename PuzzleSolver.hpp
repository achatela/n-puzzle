#pragma once
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <map>

class PuzzleSolver
{

    struct Node
    {
        char direction; // 'U', 'D', 'L', 'R'
        std::vector<std::vector<int>> puzzle;
        Node *parent;
        int cost;  // depth + heuristics
        int depth; // number of moves
    };

private:
    std::vector<std::vector<int>> _puzzle;
    std::vector<std::vector<int>> _snailSolution;
    std::map<int, std::pair<int, int>> _snailPositions;
    int _lineLength;
    Node *_root;

public:
    // Constructor and destructor
    PuzzleSolver(std::vector<std::vector<int>> puzzle, int lineLength, std::string heuristicInput);
    ~PuzzleSolver();

    // Solving methods
    int manhattanDistance(std::vector<std::vector<int>> puzzle);
    int linearConflict(std::vector<std::vector<int>> puzzle);
    int uniformCostFakeHeuristic(std::vector<std::vector<int>> puzzle);
    void addNeighbours(Node *currentNode, std::vector<Node *> &openList, std::vector<Node *> &closedList, int (PuzzleSolver::*heuristic)(std::vector<std::vector<int>>));
    std::vector<std::vector<int>> upPuzzle(std::vector<std::vector<int>> puzzle);
    std::vector<std::vector<int>> downPuzzle(std::vector<std::vector<int>> puzzle);
    std::vector<std::vector<int>> leftPuzzle(std::vector<std::vector<int>> puzzle);
    std::vector<std::vector<int>> rightPuzzle(std::vector<std::vector<int>> puzzle);
    std::vector<std::vector<int>> getSnailSolution();

    // Getters
    int getLineLength() const;
    std::vector<std::vector<int>> getPuzzle() const;

    // Setters
    void setSnailPuzzle();
    void setLineLength(int lineLength);
};