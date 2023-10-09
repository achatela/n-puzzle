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
#include <list>

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

        int field;

        CompareNode(int field = 0) : field(field) {}

        bool operator()(Node *const &n1, Node *const &n2)
        {
            switch (field)
            {
            case 0:
                return n1->cost > n2->cost;
            case 1:
                return (n1->cost - n1->depth - 1) < (n2->cost - n2->depth - 1);
            }
            return (0);
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
        PuzzleSolver(std::vector<std::vector<int>> puzzle, int lineLength, std::string heuristicInput, std::string algorithmInput);
        ~PuzzleSolver();

        // Solving methods
        std::vector<std::vector<int>> getSnailSolution();
        void addNeighbours(Node *currentNode, std::priority_queue<Node *, std::vector<Node *>, CompareNode> &openList, std::map<std::vector<std::vector<int>>, Node *> &closedList, int (PuzzleSolver::*heuristic)(std::vector<std::vector<int>> puzzle));
        void pushNode(char direction, Node *parentNode, int (PuzzleSolver::*heuristic)(std::vector<std::vector<int>> puzzle), std::priority_queue<Node *, std::vector<Node *>, CompareNode> &openList, std::map<std::vector<std::vector<int>>, Node *> &closedList);
        std::vector<std::vector<int>> makePuzzle(char direction, std::vector<std::vector<int>> puzzle);
        int manhattanDistance(std::vector<std::vector<int>> puzzle);
        int euclidianDistance(std::vector<std::vector<int>> puzzle);
        int resolveConflicts(std::list<std::pair<int, int>> conflicts, std::map<int, int> conflictsNumber);
        int linearConflict(std::vector<std::vector<int>> puzzle);
        int uniformCostFakeHeuristic(std::vector<std::vector<int>> puzzle);

        // Getters
        int getLineLength() const;
        std::vector<std::vector<int>> getPuzzle() const;

        // Setters
        void setSnailPuzzle();
        void setLineLength(int lineLength);
};