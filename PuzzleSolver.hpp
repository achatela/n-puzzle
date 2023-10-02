#pragma once
#include <vector>
#include <iostream>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <map>
#include <queue>
#include <math.h>
#include <set>

class PuzzleSolver
{

    struct Node
    {
        char direction; // 'U', 'D', 'L', 'R'
        std::vector<std::vector<int>> puzzle;
        Node *parent;

        typedef int value_type;
        value_type cost; // depth + heuristics
        int depth;       // number of moves
    };

    struct CompareNode
    {
        bool operator()(Node *const &n1, Node *const &n2)
        {
            return n1->cost > n2->cost;
        }
    };

private:
    std::vector<std::vector<int>> _puzzle;
    std::vector<std::vector<int>> _snailSolution;
    std::map<int, std::pair<int, int>> _snailPositions;
    std::set<std::vector<std::vector<int>>> _openListSet;
    int _lineLength;
    Node *_root;

public:
    // Constructor and destructor
    PuzzleSolver(std::vector<std::vector<int>> puzzle, int lineLength, std::string heuristicInput);
    ~PuzzleSolver();

    // Solving methods
    std::vector<std::vector<int>> getSnailSolution();
    void addNeighbours(Node *currentNode, std::priority_queue<Node *, std::vector<Node *>, CompareNode> &openList, std::map<std::vector<std::vector<int>>, Node *> &closedList, int (PuzzleSolver::*heuristic)(std::vector<std::vector<int>> puzzle));
    void pushNode(char direction, Node *parentNode, int (PuzzleSolver::*heuristic)(std::vector<std::vector<int>> puzzle), std::priority_queue<Node *, std::vector<Node *>, CompareNode> &openList, std::map<std::vector<std::vector<int>>, Node *> &closedList);
    std::vector<std::vector<int>> makePuzzle(char direction, std::vector<std::vector<int>> puzzle);
    int manhattanDistance(std::vector<std::vector<int>> puzzle);
    int euclidianDistance(std::vector<std::vector<int>> puzzle);
    int linearConflict(std::vector<std::vector<int>> puzzle);
    int uniformCostFakeHeuristic(std::vector<std::vector<int>> puzzle);

    // Getters
    int getLineLength() const;
    std::vector<std::vector<int>> getPuzzle() const;

    // Setters
    void setSnailPuzzle();
    void setLineLength(int lineLength);
};