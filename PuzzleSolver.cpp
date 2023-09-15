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
    while (!openList.empty()){
        Node *currentNode = openList[0];
        int j = 0;
        for (int i = 0; i < openList.size(); i++){
            if (openList[i]->cost < currentNode->cost){
                currentNode = openList[i];
                j = i;
            }
        }
        openList.erase(openList.begin() + j);
        closedList.push_back(currentNode);
        // for (int i = 0; i < currentNode->puzzle.size(); i++){
        //     for (int j = 0; j < currentNode->puzzle[i].size(); j++){
        //         if (j == currentNode->puzzle[i].size() - 1)
        //             std::cout << currentNode->puzzle[i][j];
        //         else
        //             std::cout << currentNode->puzzle[i][j] << " ";
        //     }
        //     std::cout << std::endl;
        // }
        // std::cout << std::endl;
        if (manhattanDistance(currentNode->puzzle) == 0){
            std::cout << "Solved!" << std::endl;
            for (int i = 0; i < currentNode->puzzle.size(); i++){
                for (int j = 0; j < currentNode->puzzle[i].size(); j++){
                    if (j == currentNode->puzzle[i].size() - 1)
                        std::cout << currentNode->puzzle[i][j];
                    else
                        std::cout << currentNode->puzzle[i][j] << " ";
                }
                std::cout << std::endl;
            }
            break;
        }
        addNeighbours(currentNode, openList, closedList);
    }
}

PuzzleSolver::~PuzzleSolver() { }


// Solving methods

void PuzzleSolver::addNeighbours(Node * currentNode, std::vector<Node *> &openList, std::vector<Node *> &closedList) {
    std::vector<std::vector<int>> puzzle = currentNode->puzzle;
    if (currentNode->direction == 'N'){
        int upCost = -1;
        int downCost = -1;
        int leftCost = -1;
        int rightCost = -1;
        std::vector<std::vector<int>> upPuzzleResult = upPuzzle(puzzle);
        if (upPuzzleResult != puzzle)
            upCost = currentNode->depth + 1 + manhattanDistance(upPuzzleResult);
        std::vector<std::vector<int>> downPuzzleResult = downPuzzle(puzzle);
        if (downPuzzleResult != puzzle)
            downCost = currentNode->depth + 1 + manhattanDistance(downPuzzleResult);
        std::vector<std::vector<int>> leftPuzzleResult = leftPuzzle(puzzle);
        if (leftPuzzleResult != puzzle)
            leftCost = currentNode->depth + 1 + manhattanDistance(leftPuzzleResult);
        std::vector<std::vector<int>> rightPuzzleResult = rightPuzzle(puzzle);
        if (rightPuzzleResult != puzzle)
            rightCost = currentNode->depth + 1 + manhattanDistance(rightPuzzleResult);
        if (upCost != -1){
            Node *upNode = new Node;
            upNode->puzzle = upPuzzleResult;
            upNode->parent = currentNode;
            upNode->cost = upCost;
            upNode->depth = currentNode->depth + 1;
            upNode->direction = 'U';
            openList.push_back(upNode);
        }
        if (downCost != -1){
            Node *downNode = new Node;
            downNode->puzzle = downPuzzleResult;
            downNode->parent = currentNode;
            downNode->cost = downCost;
            downNode->depth = currentNode->depth + 1;
            downNode->direction = 'D';
            openList.push_back(downNode);
        }
        if (leftCost != -1){
            Node *leftNode = new Node;
            leftNode->puzzle = leftPuzzleResult;
            leftNode->parent = currentNode;
            leftNode->cost = leftCost;
            leftNode->depth = currentNode->depth + 1;
            leftNode->direction = 'L';
            openList.push_back(leftNode);
        }
        if (rightCost != -1) {
            Node *rightNode = new Node;
            rightNode->puzzle = rightPuzzleResult;
            rightNode->parent = currentNode;
            rightNode->cost = rightCost;
            rightNode->depth = currentNode->depth + 1;
            rightNode->direction = 'R';
            openList.push_back(rightNode);
        }
    }
    else if (currentNode->direction == 'U'){
        int upCost = -1;
        int leftCost = -1;
        int rightCost = -1;
        std::vector<std::vector<int>> upPuzzleResult = upPuzzle(puzzle);
        if (upPuzzleResult != puzzle)
            upCost = currentNode->depth + 1 + manhattanDistance(upPuzzleResult);
        std::vector<std::vector<int>> leftPuzzleResult = leftPuzzle(puzzle);
        if (leftPuzzleResult != puzzle)
            leftCost = currentNode->depth + 1 + manhattanDistance(leftPuzzleResult);
        std::vector<std::vector<int>> rightPuzzleResult = rightPuzzle(puzzle);
        if (rightPuzzleResult != puzzle)
            rightCost = currentNode->depth + 1 + manhattanDistance(rightPuzzleResult);
        if (upCost != -1){
            Node *upNode = new Node;
            upNode->puzzle = upPuzzleResult;
            upNode->parent = currentNode;
            upNode->cost = upCost;
            upNode->depth = currentNode->depth + 1;
            upNode->direction = 'U';
            bool inOpenList = false;
            for (int i = 0; i < openList.size(); i++){
                if (openList[i]->puzzle == upNode->puzzle){
                    inOpenList = true;
                    if (openList[i]->cost > upNode->cost){
                        openList[i]->cost = upNode->cost;
                        openList[i]->parent = upNode->parent;
                        openList[i]->depth = upNode->depth;
                        openList[i]->direction = upNode->direction;
                    }
                }
            }
            bool inClosedList = false;
            for (int i = 0; i < closedList.size(); i++){
                if (closedList[i]->puzzle == upNode->puzzle){
                    inClosedList = true;
                    if (closedList[i]->cost > upNode->cost){
                        closedList[i]->cost = upNode->cost;
                        closedList[i]->parent = upNode->parent;
                        closedList[i]->depth = upNode->depth;
                        closedList[i]->direction = upNode->direction;
                    }
                }
            }
            if (inOpenList == false && inClosedList == false)
                openList.push_back(upNode);
            else
                delete upNode;
        }
        if (leftCost != -1){
            Node *leftNode = new Node;
            leftNode->puzzle = leftPuzzleResult;
            leftNode->parent = currentNode;
            leftNode->cost = leftCost;
            leftNode->depth = currentNode->depth + 1;
            leftNode->direction = 'L';
            bool inOpenList = false;
            for (int i = 0; i < openList.size(); i++){
                if (openList[i]->puzzle == leftNode->puzzle){
                    inOpenList = true;
                    if (openList[i]->cost > leftNode->cost){
                        openList[i]->cost = leftNode->cost;
                        openList[i]->parent = leftNode->parent;
                        openList[i]->depth = leftNode->depth;
                        openList[i]->direction = leftNode->direction;
                    }
                }
            }
            bool inClosedList = false;
            for (int i = 0; i < closedList.size(); i++){
                if (closedList[i]->puzzle == leftNode->puzzle){
                    inClosedList = true;
                    if (closedList[i]->cost > leftNode->cost){
                        closedList[i]->cost = leftNode->cost;
                        closedList[i]->parent = leftNode->parent;
                        closedList[i]->depth = leftNode->depth;
                        closedList[i]->direction = leftNode->direction;
                    }
                }
            }
            if (inOpenList == false && inClosedList == false)
                openList.push_back(leftNode);
            else
                delete leftNode;
        }
        if (rightCost != -1) {
            Node *rightNode = new Node;
            rightNode->puzzle = rightPuzzleResult;
            rightNode->parent = currentNode;
            rightNode->cost = rightCost;
            rightNode->depth = currentNode->depth + 1;
            rightNode->direction = 'R';
            bool inOpenList = false;
            for (int i = 0; i < openList.size(); i++){
                if (openList[i]->puzzle == rightNode->puzzle){
                    inOpenList = true;
                    if (openList[i]->cost > rightNode->cost){
                        openList[i]->cost = rightNode->cost;
                        openList[i]->parent = rightNode->parent;
                        openList[i]->depth = rightNode->depth;
                        openList[i]->direction = rightNode->direction;
                    }
                }
            }
            bool inClosedList = false;
            for (int i = 0; i < closedList.size(); i++){
                if (closedList[i]->puzzle == rightNode->puzzle){
                    inClosedList = true;
                    if (closedList[i]->cost > rightNode->cost){
                        closedList[i]->cost = rightNode->cost;
                        closedList[i]->parent = rightNode->parent;
                        closedList[i]->depth = rightNode->depth;
                        closedList[i]->direction = rightNode->direction;
                    }
                }
            }
            if (inOpenList == false && inClosedList == false)
                openList.push_back(rightNode);
            else
                delete rightNode;
        }
    }
    else if (currentNode->direction == 'D'){
        int downCost = -1;
        int leftCost = -1;
        int rightCost = -1;
        std::vector<std::vector<int>> downPuzzleResult = downPuzzle(puzzle);
        if (downPuzzleResult != puzzle)
            downCost = currentNode->depth + 1 + manhattanDistance(downPuzzleResult);
        std::vector<std::vector<int>> leftPuzzleResult = leftPuzzle(puzzle);
        if (leftPuzzleResult != puzzle)
            leftCost = currentNode->depth + 1 + manhattanDistance(leftPuzzleResult);
        std::vector<std::vector<int>> rightPuzzleResult = rightPuzzle(puzzle);
        if (rightPuzzleResult != puzzle)
            rightCost = currentNode->depth + 1 + manhattanDistance(rightPuzzleResult);
        if (downCost != -1){
            Node *downNode = new Node;
            downNode->puzzle = downPuzzleResult;
            downNode->parent = currentNode;
            downNode->cost = downCost;
            downNode->depth = currentNode->depth + 1;
            downNode->direction = 'D';
            bool inOpenList = false;
            for (int i = 0; i < openList.size(); i++){
                if (openList[i]->puzzle == downNode->puzzle){
                    inOpenList = true;
                    if (openList[i]->cost > downNode->cost){
                        openList[i]->cost = downNode->cost;
                        openList[i]->parent = downNode->parent;
                        openList[i]->depth = downNode->depth;
                        openList[i]->direction = downNode->direction;
                    }
                }
            }
            bool inClosedList = false;
            for (int i = 0; i < closedList.size(); i++){
                if (closedList[i]->puzzle == downNode->puzzle){
                    inClosedList = true;
                    if (closedList[i]->cost > downNode->cost){
                        closedList[i]->cost = downNode->cost;
                        closedList[i]->parent = downNode->parent;
                        closedList[i]->depth = downNode->depth;
                        closedList[i]->direction = downNode->direction;
                    }
                }
            }
            if (inOpenList == false && inClosedList == false)
                openList.push_back(downNode);
            else
                delete downNode;
        }
        if (leftCost != -1){
            Node *leftNode = new Node;
            leftNode->puzzle = leftPuzzleResult;
            leftNode->parent = currentNode;
            leftNode->cost = leftCost;
            leftNode->depth = currentNode->depth + 1;
            leftNode->direction = 'L';
            bool inOpenList = false;
            for (int i = 0; i < openList.size(); i++){
                if (openList[i]->puzzle == leftNode->puzzle){
                    inOpenList = true;
                    if (openList[i]->cost > leftNode->cost){
                        openList[i]->cost = leftNode->cost;
                        openList[i]->parent = leftNode->parent;
                        openList[i]->depth = leftNode->depth;
                        openList[i]->direction = leftNode->direction;
                    }
                }
            }
            bool inClosedList = false;
            for (int i = 0; i < closedList.size(); i++){
                if (closedList[i]->puzzle == leftNode->puzzle){
                    inClosedList = true;
                    if (closedList[i]->cost > leftNode->cost){
                        closedList[i]->cost = leftNode->cost;
                        closedList[i]->parent = leftNode->parent;
                        closedList[i]->depth = leftNode->depth;
                        closedList[i]->direction = leftNode->direction;
                    }
                }
            }
            if (inOpenList == false && inClosedList == false)
                openList.push_back(leftNode);
            else
                delete leftNode;
        }
        if (rightCost != -1) {
            Node *rightNode = new Node;
            rightNode->puzzle = rightPuzzleResult;
            rightNode->parent = currentNode;
            rightNode->cost = rightCost;
            rightNode->depth = currentNode->depth + 1;
            rightNode->direction = 'R';
            bool inOpenList = false;
            for (int i = 0; i < openList.size(); i++){
                if (openList[i]->puzzle == rightNode->puzzle){
                    inOpenList = true;
                    if (openList[i]->cost > rightNode->cost){
                        openList[i]->cost = rightNode->cost;
                        openList[i]->parent = rightNode->parent;
                        openList[i]->depth = rightNode->depth;
                        openList[i]->direction = rightNode->direction;
                    }
                }
            }
            bool inClosedList = false;
            for (int i = 0; i < closedList.size(); i++){
                if (closedList[i]->puzzle == rightNode->puzzle){
                    inClosedList = true;
                    if (closedList[i]->cost > rightNode->cost){
                        closedList[i]->cost = rightNode->cost;
                        closedList[i]->parent = rightNode->parent;
                        closedList[i]->depth = rightNode->depth;
                        closedList[i]->direction = rightNode->direction;
                    }
                }
            }
            if (inOpenList == false && inClosedList == false)
                openList.push_back(rightNode);
            else
                delete rightNode;
        }
    }
    else if (currentNode->direction == 'L'){
        int upCost = -1;
        int downCost = -1;
        int leftCost = -1;
        std::vector<std::vector<int>> upPuzzleResult = upPuzzle(puzzle);
        if (upPuzzleResult != puzzle)
            upCost = currentNode->depth + 1 + manhattanDistance(upPuzzleResult);
        std::vector<std::vector<int>> downPuzzleResult = downPuzzle(puzzle);
        if (downPuzzleResult != puzzle)
            downCost = currentNode->depth + 1 + manhattanDistance(downPuzzleResult);
        std::vector<std::vector<int>> leftPuzzleResult = leftPuzzle(puzzle);
        if (leftPuzzleResult != puzzle)
            leftCost = currentNode->depth + 1 + manhattanDistance(leftPuzzleResult);
        if (upCost != -1){
            Node *upNode = new Node;
            upNode->puzzle = upPuzzleResult;
            upNode->parent = currentNode;
            upNode->cost = upCost;
            upNode->depth = currentNode->depth + 1;
            upNode->direction = 'U';
            bool inOpenList = false;
            for (int i = 0; i < openList.size(); i++){
                if (openList[i]->puzzle == upNode->puzzle){
                    inOpenList = true;
                    if (openList[i]->cost > upNode->cost){
                        openList[i]->cost = upNode->cost;
                        openList[i]->parent = upNode->parent;
                        openList[i]->depth = upNode->depth;
                        openList[i]->direction = upNode->direction;
                    }
                }
            }
            bool inClosedList = false;
            for (int i = 0; i < closedList.size(); i++){
                if (closedList[i]->puzzle == upNode->puzzle){
                    inClosedList = true;
                    if (closedList[i]->cost > upNode->cost){
                        closedList[i]->cost = upNode->cost;
                        closedList[i]->parent = upNode->parent;
                        closedList[i]->depth = upNode->depth;
                        closedList[i]->direction = upNode->direction;
                    }
                }
            }
            if (inOpenList == false && inClosedList == false)
                openList.push_back(upNode);
            else
                delete upNode;
        }
        if (downCost != -1){
            Node *downNode = new Node;
            downNode->puzzle = downPuzzleResult;
            downNode->parent = currentNode;
            downNode->cost = downCost;
            downNode->depth = currentNode->depth + 1;
            downNode->direction = 'D';
            bool inOpenList = false;
            for (int i = 0; i < openList.size(); i++){
                if (openList[i]->puzzle == downNode->puzzle){
                    inOpenList = true;
                    if (openList[i]->cost > downNode->cost){
                        openList[i]->cost = downNode->cost;
                        openList[i]->parent = downNode->parent;
                        openList[i]->depth = downNode->depth;
                        openList[i]->direction = downNode->direction;
                    }
                }
            }
            bool inClosedList = false;
            for (int i = 0; i < closedList.size(); i++){
                if (closedList[i]->puzzle == downNode->puzzle){
                    inClosedList = true;
                    if (closedList[i]->cost > downNode->cost){
                        closedList[i]->cost = downNode->cost;
                        closedList[i]->parent = downNode->parent;
                        closedList[i]->depth = downNode->depth;
                        closedList[i]->direction = downNode->direction;
                    }
                }
            }
            if (inOpenList == false && inClosedList == false)
                openList.push_back(downNode);
            else
                delete downNode;
        }
        if (leftCost != -1){
            Node *leftNode = new Node;
            leftNode->puzzle = leftPuzzleResult;
            leftNode->parent = currentNode;
            leftNode->cost = leftCost;
            leftNode->depth = currentNode->depth + 1;
            leftNode->direction = 'L';
            bool inOpenList = false;
            for (int i = 0; i < openList.size(); i++){
                if (openList[i]->puzzle == leftNode->puzzle){
                    inOpenList = true;
                    if (openList[i]->cost > leftNode->cost){
                        openList[i]->cost = leftNode->cost;
                        openList[i]->parent = leftNode->parent;
                        openList[i]->depth = leftNode->depth;
                        openList[i]->direction = leftNode->direction;
                    }
                }
            }
            bool inClosedList = false;
            for (int i = 0; i < closedList.size(); i++){
                if (closedList[i]->puzzle == leftNode->puzzle){
                    inClosedList = true;
                    if (closedList[i]->cost > leftNode->cost){
                        closedList[i]->cost = leftNode->cost;
                        closedList[i]->parent = leftNode->parent;
                        closedList[i]->depth = leftNode->depth;
                        closedList[i]->direction = leftNode->direction;
                    }
                }
            }
            if (inOpenList == false && inClosedList == false)
                openList.push_back(leftNode);
            else
                delete leftNode;
        }
    }
    else if (currentNode->direction == 'R'){
        int upCost = -1;
        int downCost = -1;
        int rightCost = -1;
        std::vector<std::vector<int>> upPuzzleResult = upPuzzle(puzzle);
        if (upPuzzleResult != puzzle)
            upCost = currentNode->depth + 1 + manhattanDistance(upPuzzleResult);
        std::vector<std::vector<int>> downPuzzleResult = downPuzzle(puzzle);
        if (downPuzzleResult != puzzle)
            downCost = currentNode->depth + 1 + manhattanDistance(downPuzzleResult);
        std::vector<std::vector<int>> rightPuzzleResult = rightPuzzle(puzzle);
        if (rightPuzzleResult != puzzle)
            rightCost = currentNode->depth + 1 + manhattanDistance(rightPuzzleResult);
        if (upCost != -1){
            Node *upNode = new Node;
            upNode->puzzle = upPuzzleResult;
            upNode->parent = currentNode;
            upNode->cost = upCost;
            upNode->depth = currentNode->depth + 1;
            upNode->direction = 'U';
            bool inOpenList = false;
            for (int i = 0; i < openList.size(); i++){
                if (openList[i]->puzzle == upNode->puzzle){
                    inOpenList = true;
                    if (openList[i]->cost > upNode->cost){
                        openList[i]->cost = upNode->cost;
                        openList[i]->parent = upNode->parent;
                        openList[i]->depth = upNode->depth;
                        openList[i]->direction = upNode->direction;
                    }
                }
            }
            bool inClosedList = false;
            for (int i = 0; i < closedList.size(); i++){
                if (closedList[i]->puzzle == upNode->puzzle){
                    inClosedList = true;
                    if (closedList[i]->cost > upNode->cost){
                        closedList[i]->cost = upNode->cost;
                        closedList[i]->parent = upNode->parent;
                        closedList[i]->depth = upNode->depth;
                        closedList[i]->direction = upNode->direction;
                    }
                }
            }
            if (inOpenList == false && inClosedList == false)
                openList.push_back(upNode);
            else
                delete upNode;
        }
        if (downCost != -1){
            Node *downNode = new Node;
            downNode->puzzle = downPuzzleResult;
            downNode->parent = currentNode;
            downNode->cost = downCost;
            downNode->depth = currentNode->depth + 1;
            downNode->direction = 'D';
            bool inOpenList = false;
            for (int i = 0; i < openList.size(); i++){
                if (openList[i]->puzzle == downNode->puzzle){
                    inOpenList = true;
                    if (openList[i]->cost > downNode->cost){
                        openList[i]->cost = downNode->cost;
                        openList[i]->parent = downNode->parent;
                        openList[i]->depth = downNode->depth;
                        openList[i]->direction = downNode->direction;
                    }
                }
            }
            bool inClosedList = false;
            for (int i = 0; i < closedList.size(); i++){
                if (closedList[i]->puzzle == downNode->puzzle){
                    inClosedList = true;
                    if (closedList[i]->cost > downNode->cost){
                        closedList[i]->cost = downNode->cost;
                        closedList[i]->parent = downNode->parent;
                        closedList[i]->depth = downNode->depth;
                        closedList[i]->direction = downNode->direction;
                    }
                }
            }
            if (inOpenList == false && inClosedList == false)
                openList.push_back(downNode);
            else
                delete downNode;
        }
        if (rightCost != -1) {
            Node *rightNode = new Node;
            rightNode->puzzle = rightPuzzleResult;
            rightNode->parent = currentNode;
            rightNode->cost = rightCost;
            rightNode->depth = currentNode->depth + 1;
            rightNode->direction = 'R';
            bool inOpenList = false;
            for (int i = 0; i < openList.size(); i++){
                if (openList[i]->puzzle == rightNode->puzzle){
                    inOpenList = true;
                    if (openList[i]->cost > rightNode->cost){
                        openList[i]->cost = rightNode->cost;
                        openList[i]->parent = rightNode->parent;
                        openList[i]->depth = rightNode->depth;
                        openList[i]->direction = rightNode->direction;
                    }
                }
            }
            bool inClosedList = false;
            for (int i = 0; i < closedList.size(); i++){
                if (closedList[i]->puzzle == rightNode->puzzle){
                    inClosedList = true;
                    if (closedList[i]->cost > rightNode->cost){
                        closedList[i]->cost = rightNode->cost;
                        closedList[i]->parent = rightNode->parent;
                        closedList[i]->depth = rightNode->depth;
                        closedList[i]->direction = rightNode->direction;
                    }
                }
            }
            if (inOpenList == false && inClosedList == false)
                openList.push_back(rightNode);
            else
                delete rightNode;
        }
    }
    else {
        throw std::invalid_argument("Invalid direction");
    }
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

std::vector<std::vector<int>> PuzzleSolver::upPuzzle(std::vector<std::vector<int>> puzzle){
    std::vector<std::vector<int>> upPuzzle = puzzle;
    for (int i = 0; i < upPuzzle.size(); i++){
        for (int j = 0; j < upPuzzle[i].size(); j++){
            if (upPuzzle[i][j] == 0){
                if (i == 0){
                    return puzzle;
                }
                else {
                    upPuzzle[i][j] = upPuzzle[i - 1][j];
                    upPuzzle[i - 1][j] = 0;
                    return upPuzzle;
                }
            }
        }
    }
    return upPuzzle;
}

std::vector<std::vector<int>> PuzzleSolver::downPuzzle(std::vector<std::vector<int>> puzzle) {
    std::vector<std::vector<int>> downPuzzle = puzzle;

    for (int i = 0; i < downPuzzle.size(); i++) {
        for (int j = 0; j < downPuzzle[i].size(); j++) {
            if (downPuzzle[i][j] == 0) {
                if (i == downPuzzle.size() - 1) {
                    return puzzle;
                }
                else {
                    downPuzzle[i][j] = downPuzzle[i + 1][j];
                    downPuzzle[i + 1][j] = 0;
                    return downPuzzle;
                }
            }
        }
    }
    return downPuzzle;
}

std::vector<std::vector<int>> PuzzleSolver::leftPuzzle(std::vector<std::vector<int>> puzzle) {
    std::vector<std::vector<int>> leftPuzzle = puzzle;
    for (int i = 0; i < leftPuzzle.size(); i++) {
        for (int j = 0; j < leftPuzzle[i].size(); j++) {
            if (leftPuzzle[i][j] == 0) {
                if (j == 0) {
                    return puzzle;
                }
                else {
                    leftPuzzle[i][j] = leftPuzzle[i][j - 1];
                    leftPuzzle[i][j - 1] = 0;
                    return leftPuzzle;
                }
            }
        }
    }
    return leftPuzzle;
}

std::vector<std::vector<int>> PuzzleSolver::rightPuzzle(std::vector<std::vector<int>> puzzle) {
    std::vector<std::vector<int>> rightPuzzle = puzzle;
    for (int i = 0; i < rightPuzzle.size(); i++) {
        for (int j = 0; j < rightPuzzle[i].size(); j++) {
            if (rightPuzzle[i][j] == 0) {
                if (j == rightPuzzle[i].size() - 1) {
                    return puzzle;
                }
                else {
                    rightPuzzle[i][j] = rightPuzzle[i][j + 1];
                    rightPuzzle[i][j + 1] = 0;
                    return rightPuzzle;
                }
            }
        }
    }
    return rightPuzzle;
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