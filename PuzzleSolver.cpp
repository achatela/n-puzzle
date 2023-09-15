#include "PuzzleSolver.hpp"

// Constructor and destructor
PuzzleSolver::PuzzleSolver(std::vector<std::vector<int>> puzzle, int lineLength) : _puzzle(puzzle), _lineLength(lineLength) {
    std::cout << "Initial manhattan distance: " << manhattanDistance(puzzle) << std::endl;
    _root = new Node;
    _root->puzzle = puzzle;
    _root->parent = nullptr;
    _root->cost = 0;
    _root->depth = 0;
    _root->direction = 'N'; // none

    std::vector<Node *> openList;
    std::vector<Node *> closedList;

    openList.push_back(_root);
    while (openList.empty() == false){
        Node *currentNode = openList[0];
        Node *upNode = new Node;
        Node *downNode = new Node;
        Node *leftNode = new Node;
        Node *rightNode = new Node;
        assignPuzzle(currentNode->puzzle, *upNode, *downNode, *leftNode, *rightNode);

    }   
}

PuzzleSolver::~PuzzleSolver() { }


// Solving methods

void PuzzleSolver::assignPuzzle(std::vector<std::vector<int>> puzzle, Node & upNode, Node & downNode, Node & leftNode, Node & RightNode) {
    return ;
}

int PuzzleSolver::manhattanDistance(std::vector<std::vector<int>> puzzle) {
    int manhattanDistance = 0;

    for (auto i = 0; i < puzzle.size(); i++) {
        for (auto j = 0; j < puzzle[i].size(); j++) {
            int value = puzzle[i][j];
            if (value == 0)
                continue;
            int expectedRow = (value - 1) / _lineLength;
            int expectedColumn = (value - 1) % _lineLength;
            int distance = abs(i - expectedRow) + abs(j - expectedColumn);
            manhattanDistance += distance;
        }
    }
    return manhattanDistance;
}


// Getters

int PuzzleSolver::getLineLength() const
{
    return _lineLength;
}

std::vector<std::vector<int>> PuzzleSolver::getPuzzle() const
{
    return _puzzle;
}

// Setters

void PuzzleSolver::setLineLength(int lineLength)
{
    this->_lineLength = lineLength;
}