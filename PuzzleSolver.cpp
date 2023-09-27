#include "PuzzleSolver.hpp"

// Constructor and destructor
void PuzzleSolver::setSnailPuzzle()
{
    std::vector<std::vector<int> > snailSolution;
    for (int i = 0; i != _lineLength; i++) {
        snailSolution.push_back(std::vector<int>());
        for (int j = 0; j != _lineLength; j++) {
            snailSolution[i].push_back(0);
        }
    }

    int total = _lineLength * _lineLength;
    int size = _lineLength;
    int number = 1;
    int column = 0;
    int row = 0;

    for (int i = 0; number != total; i++) {
        if (i % 4 == 0) {
            size--;
            while (row < size)
                snailSolution[column][row++] = number++;
        }
        else if (i % 4 == 1)
            while (column < size)
                snailSolution[column++][row] = number++;
        else if (i % 4 == 2)
            while (row >= _lineLength - size)
                snailSolution[column][row--] = number++;
        else
            while (column >= _lineLength - size)
                snailSolution[column--][row] = number++;
    }

    // for (int i = 0; i != _lineLength; i++) {
    //     for (int j = 0; j != _lineLength; j++) {
    //         std::cout << snailSolution[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }

    for (int i = 0; i < _snailSolution.size(); i++)
    {
        for (int j = 0; j < _snailSolution[i].size(); j++)
        {
            _snailPositions[_snailSolution[i][j]] = std::make_pair(i, j);
        }
    }
}

PuzzleSolver::PuzzleSolver(std::vector<std::vector<int> > puzzle, int lineLength, std::string heuristicInput) : _puzzle(puzzle), _lineLength(lineLength)
{
    _root = new Node;
    _root->puzzle = puzzle;
    _root->parent = nullptr;
    _root->cost = 0;
    _root->depth = 0;
    _root->direction = 'N'; // none

    setSnailPuzzle();
    int timeComplexity = 0;

    std::map<std::vector<std::vector<int> >, Node **> addedNodes;
    std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList;


    std::map<std::vector<std::vector<int> >, Node **> closedList;

    int (PuzzleSolver::*heuristic)(std::vector<std::vector<int> > puzzle) = nullptr;
    if (heuristicInput == "manhattan")
        heuristic = &PuzzleSolver::manhattanDistance;
    else if (heuristicInput == "linear")
        heuristic = &PuzzleSolver::linearConflict;
    else if (heuristicInput == "bonus_uniform")
        heuristic = &PuzzleSolver::uniformCostFakeHeuristic;
    else if (heuristicInput == "euclidian")
        heuristic = &PuzzleSolver::euclidianDistance;
    else
        throw std::invalid_argument("Invalid heuristic");

    openList.push(_root);
    addedNodes.insert(std::pair<std::vector<std::vector<int> >, Node **>(_root->puzzle, &_root));

    while (!openList.empty())
    {
        Node *currentNode = openList.top();
        int j = 0;
        openList.pop();
        // closedList.push_back(currentNode);
        closedList[currentNode->puzzle] = &currentNode;
        addedNodes.erase(currentNode->puzzle);
        timeComplexity++;
        if (manhattanDistance(currentNode->puzzle) == 0)
        {
            std::cout << "Solved!" << std::endl;
            for (int i = 0; i < currentNode->puzzle.size(); i++)
            {
                for (int j = 0; j < currentNode->puzzle[i].size(); j++)
                {
                    if (j == currentNode->puzzle[i].size() - 1)
                        std::cout << currentNode->puzzle[i][j];
                    else
                        std::cout << currentNode->puzzle[i][j] << " ";
                }
                std::cout << std::endl;
            }
            std::string solution = "";
            while (currentNode->parent != nullptr)
            {
                solution += currentNode->direction;
                currentNode = currentNode->parent;
            }
            std::reverse(solution.begin(), solution.end());
            std::cout << "Time complexity: " << timeComplexity << std::endl;
            std::cout << "Space complexity: " << openList.size() + closedList.size() << std::endl;
            std::cout << "Number of moves required " << solution.length() << std::endl;
            std::cout << "Solution: " << solution << std::endl;
            break;
        }
        addNeighbours(currentNode, openList, closedList, addedNodes, heuristic);
    }
}

PuzzleSolver::~PuzzleSolver() {}

// Solving methods

int PuzzleSolver::uniformCostFakeHeuristic(std::vector<std::vector<int> > puzzle)
{
    return 0;
}

void PuzzleSolver::addNeighbours(Node *currentNode, std::priority_queue<Node *, std::vector<Node *>, CompareNode> &openList, std::map<std::vector<std::vector<int> >, Node **> &closedList, std::map<std::vector<std::vector<int> >, Node **> &addedNodes, int (PuzzleSolver::*heuristic)(std::vector<std::vector<int> > puzzle))
{
    std::vector<std::vector<int> > puzzle = currentNode->puzzle;
    if (currentNode->direction == 'N')
    {
        int upCost = -1;
        int downCost = -1;
        int leftCost = -1;
        int rightCost = -1;
        std::vector<std::vector<int> > upPuzzleResult = upPuzzle(puzzle);
        if (upPuzzleResult != puzzle)
            upCost = currentNode->depth + 1 + (this->*heuristic)(upPuzzleResult);
        std::vector<std::vector<int> > downPuzzleResult = downPuzzle(puzzle);
        if (downPuzzleResult != puzzle)
            downCost = currentNode->depth + 1 + (this->*heuristic)(downPuzzleResult);
        std::vector<std::vector<int> > leftPuzzleResult = leftPuzzle(puzzle);
        if (leftPuzzleResult != puzzle)
            leftCost = currentNode->depth + 1 + (this->*heuristic)(leftPuzzleResult);
        std::vector<std::vector<int> > rightPuzzleResult = rightPuzzle(puzzle);
        if (rightPuzzleResult != puzzle)
            rightCost = currentNode->depth + 1 + (this->*heuristic)(rightPuzzleResult);
        if (upCost != -1)
        {
            Node *upNode = new Node;
            upNode->puzzle = upPuzzleResult;
            upNode->parent = currentNode;
            upNode->cost = upCost;
            upNode->depth = currentNode->depth + 1;
            upNode->direction = 'U';
            openList.push(upNode);
        }
        if (downCost != -1)
        {
            Node *downNode = new Node;
            downNode->puzzle = downPuzzleResult;
            downNode->parent = currentNode;
            downNode->cost = downCost;
            downNode->depth = currentNode->depth + 1;
            downNode->direction = 'D';
            openList.push(downNode);
        }
        if (leftCost != -1)
        {
            Node *leftNode = new Node;
            leftNode->puzzle = leftPuzzleResult;
            leftNode->parent = currentNode;
            leftNode->cost = leftCost;
            leftNode->depth = currentNode->depth + 1;
            leftNode->direction = 'L';
            openList.push(leftNode);
        }
        if (rightCost != -1)
        {
            Node *rightNode = new Node;
            rightNode->puzzle = rightPuzzleResult;
            rightNode->parent = currentNode;
            rightNode->cost = rightCost;
            rightNode->depth = currentNode->depth + 1;
            rightNode->direction = 'R';
            openList.push(rightNode);
        }
    }
    else if (currentNode->direction == 'U')
    {
        int upCost = -1;
        int leftCost = -1;
        int rightCost = -1;
        std::vector<std::vector<int> > upPuzzleResult = upPuzzle(puzzle);
        if (upPuzzleResult != puzzle)
            upCost = currentNode->depth + 1 + (this->*heuristic)(upPuzzleResult);
        std::vector<std::vector<int> > leftPuzzleResult = leftPuzzle(puzzle);
        if (leftPuzzleResult != puzzle)
            leftCost = currentNode->depth + 1 + (this->*heuristic)(leftPuzzleResult);
        std::vector<std::vector<int> > rightPuzzleResult = rightPuzzle(puzzle);
        if (rightPuzzleResult != puzzle)
            rightCost = currentNode->depth + 1 + (this->*heuristic)(rightPuzzleResult);
        if (upCost != -1)
        {
            Node *upNode = new Node;
            upNode->puzzle = upPuzzleResult;
            upNode->parent = currentNode;
            upNode->cost = upCost;
            upNode->depth = currentNode->depth + 1;
            upNode->direction = 'U';
            bool inOpenList = false;
            if (addedNodes.find(upNode->puzzle) != addedNodes.end())
            {
                Node *node = *addedNodes[upNode->puzzle];
                if (node->direction == upNode->direction)
                    inOpenList = true;
                if (inOpenList == true && node->cost > upNode->cost)
                {
                    node->cost = upNode->cost;
                    node->parent = upNode->parent;
                    node->depth = upNode->depth;
                    node->direction = upNode->direction;
                }
            }
            bool inClosedList = false;
            if (closedList.find(upNode->puzzle) != closedList.end()) {
                Node *node = *closedList[upNode->puzzle];
                inClosedList = true;
                if (inClosedList == true && node->cost > upNode->cost)
                {
                    node->cost = upNode->cost;
                    node->parent = upNode->parent;
                    node->depth = upNode->depth;
                    node->direction = upNode->direction;
                }
            }
            // for (int i = 0; i < closedList.size(); i++)
            // {
            //     if (closedList[i]->puzzle == upNode->puzzle)
            //     {
            //         inClosedList = true;
            //         if (closedList[i]->cost > upNode->cost)
            //         {
            //             closedList[i]->cost = upNode->cost;
            //             closedList[i]->parent = upNode->parent;
            //             closedList[i]->depth = upNode->depth;
            //             closedList[i]->direction = upNode->direction;
            //         }
            //     }
            // }
            if (inOpenList == false && inClosedList == false)
                openList.push(upNode);
            else
                delete upNode;
        }
        if (leftCost != -1)
        {
            Node *leftNode = new Node;
            leftNode->puzzle = leftPuzzleResult;
            leftNode->parent = currentNode;
            leftNode->cost = leftCost;
            leftNode->depth = currentNode->depth + 1;
            leftNode->direction = 'L';
            bool inOpenList = false;
            if (addedNodes.find(leftNode->puzzle) != addedNodes.end())
            {
                Node *node = *addedNodes[leftNode->puzzle];
                if (node->direction == leftNode->direction)
                    inOpenList = true;
                if (inOpenList == true && node->cost > leftNode->cost)
                {
                    node->cost = leftNode->cost;
                    node->parent = leftNode->parent;
                    node->depth = leftNode->depth;
                    node->direction = leftNode->direction;
                }
            }
            bool inClosedList = false;
            if (closedList.find(leftNode->puzzle) != closedList.end()) {
                Node *node = *closedList[leftNode->puzzle];
                inClosedList = true;
                if (inClosedList == true && node->cost > leftNode->cost)
                {
                    node->cost = leftNode->cost;
                    node->parent = leftNode->parent;
                    node->depth = leftNode->depth;
                    node->direction = leftNode->direction;
                }
            }
            // for (int i = 0; i < closedList.size(); i++)
            // {
            //     if (closedList[i]->puzzle == leftNode->puzzle)
            //     {
            //         inClosedList = true;
            //         if (closedList[i]->cost > leftNode->cost)
            //         {
            //             closedList[i]->cost = leftNode->cost;
            //             closedList[i]->parent = leftNode->parent;
            //             closedList[i]->depth = leftNode->depth;
            //             closedList[i]->direction = leftNode->direction;
            //         }
            //     }
            // }
            if (inOpenList == false && inClosedList == false)
                openList.push(leftNode);
            else
                delete leftNode;
        }
        if (rightCost != -1)
        {
            Node *rightNode = new Node;
            rightNode->puzzle = rightPuzzleResult;
            rightNode->parent = currentNode;
            rightNode->cost = rightCost;
            rightNode->depth = currentNode->depth + 1;
            rightNode->direction = 'R';
            bool inOpenList = false;
            if (addedNodes.find(rightNode->puzzle) != addedNodes.end())
            {
                Node *node = *addedNodes[rightNode->puzzle];
                if (node->direction == rightNode->direction)
                    inOpenList = true;
                if (inOpenList == true && node->cost > rightNode->cost)
                {
                    node->cost = rightNode->cost;
                    node->parent = rightNode->parent;
                    node->depth = rightNode->depth;
                    node->direction = rightNode->direction;
                }
            }
            bool inClosedList = false;
            if (closedList.find(rightNode->puzzle) != closedList.end()) {
                Node *node = *closedList[rightNode->puzzle];
                inClosedList = true;
                if (inClosedList == true && node->cost > rightNode->cost)
                {
                    node->cost = rightNode->cost;
                    node->parent = rightNode->parent;
                    node->depth = rightNode->depth;
                    node->direction = rightNode->direction;
                }
            }
            // for (int i = 0; i < closedList.size(); i++)
            // {
            //     if (closedList[i]->puzzle == rightNode->puzzle)
            //     {
            //         inClosedList = true;
            //         if (closedList[i]->cost > rightNode->cost)
            //         {
            //             closedList[i]->cost = rightNode->cost;
            //             closedList[i]->parent = rightNode->parent;
            //             closedList[i]->depth = rightNode->depth;
            //             closedList[i]->direction = rightNode->direction;
            //         }
            //     }
            // }
            if (inOpenList == false && inClosedList == false)
                openList.push(rightNode);
            else
                delete rightNode;
        }
    }
    else if (currentNode->direction == 'D')
    {
        int downCost = -1;
        int leftCost = -1;
        int rightCost = -1;
        std::vector<std::vector<int> > downPuzzleResult = downPuzzle(puzzle);
        if (downPuzzleResult != puzzle)
            downCost = currentNode->depth + 1 + (this->*heuristic)(downPuzzleResult);
        std::vector<std::vector<int> > leftPuzzleResult = leftPuzzle(puzzle);
        if (leftPuzzleResult != puzzle)
            leftCost = currentNode->depth + 1 + (this->*heuristic)(leftPuzzleResult);
        std::vector<std::vector<int> > rightPuzzleResult = rightPuzzle(puzzle);
        if (rightPuzzleResult != puzzle)
            rightCost = currentNode->depth + 1 + (this->*heuristic)(rightPuzzleResult);
        if (downCost != -1)
        {
            Node *downNode = new Node;
            downNode->puzzle = downPuzzleResult;
            downNode->parent = currentNode;
            downNode->cost = downCost;
            downNode->depth = currentNode->depth + 1;
            downNode->direction = 'D';
            bool inOpenList = false;
            if (addedNodes.find(downNode->puzzle) != addedNodes.end())
            {
                Node *node = *addedNodes[downNode->puzzle];
                if (node->direction == downNode->direction)
                    inOpenList = true;
                if (inOpenList == true && node->cost > downNode->cost)
                {
                    node->cost = downNode->cost;
                    node->parent = downNode->parent;
                    node->depth = downNode->depth;
                    node->direction = downNode->direction;
                }
            }
            bool inClosedList = false;
            if (closedList.find(downNode->puzzle) != closedList.end()) {
                Node *node = *closedList[downNode->puzzle];
                inClosedList = true;
                if (inClosedList == true && node->cost > downNode->cost)
                {
                    node->cost = downNode->cost;
                    node->parent = downNode->parent;
                    node->depth = downNode->depth;
                    node->direction = downNode->direction;
                }
            }
            // for (int i = 0; i < closedList.size(); i++)
            // {
            //     if (closedList[i]->puzzle == downNode->puzzle)
            //     {
            //         inClosedList = true;
            //         if (closedList[i]->cost > downNode->cost)
            //         {
            //             closedList[i]->cost = downNode->cost;
            //             closedList[i]->parent = downNode->parent;
            //             closedList[i]->depth = downNode->depth;
            //             closedList[i]->direction = downNode->direction;
            //         }
            //     }
            // }
            if (inOpenList == false && inClosedList == false)
                openList.push(downNode);
            else
                delete downNode;
        }
        if (leftCost != -1)
        {
            Node *leftNode = new Node;
            leftNode->puzzle = leftPuzzleResult;
            leftNode->parent = currentNode;
            leftNode->cost = leftCost;
            leftNode->depth = currentNode->depth + 1;
            leftNode->direction = 'L';
            bool inOpenList = false;
            if (addedNodes.find(leftNode->puzzle) != addedNodes.end())
            {
                Node *node = *addedNodes[leftNode->puzzle];
                if (node->direction == leftNode->direction)
                    inOpenList = true;
                if (inOpenList == true && node->cost > leftNode->cost)
                {
                    node->cost = leftNode->cost;
                    node->parent = leftNode->parent;
                    node->depth = leftNode->depth;
                    node->direction = leftNode->direction;
                }
            }
            bool inClosedList = false;
            if (closedList.find(leftNode->puzzle) != closedList.end()) {
                Node *node = *closedList[leftNode->puzzle];
                inClosedList = true;
                if (inClosedList == true && node->cost > leftNode->cost)
                {
                    node->cost = leftNode->cost;
                    node->parent = leftNode->parent;
                    node->depth = leftNode->depth;
                    node->direction = leftNode->direction;
                }
            }
            // for (int i = 0; i < closedList.size(); i++)
            // {
            //     if (closedList[i]->puzzle == leftNode->puzzle)
            //     {
            //         inClosedList = true;
            //         if (closedList[i]->cost > leftNode->cost)
            //         {
            //             closedList[i]->cost = leftNode->cost;
            //             closedList[i]->parent = leftNode->parent;
            //             closedList[i]->depth = leftNode->depth;
            //             closedList[i]->direction = leftNode->direction;
            //         }
            //     }
            // }
            if (inOpenList == false && inClosedList == false)
                openList.push(leftNode);
            else
                delete leftNode;
        }
        if (rightCost != -1)
        {
            Node *rightNode = new Node;
            rightNode->puzzle = rightPuzzleResult;
            rightNode->parent = currentNode;
            rightNode->cost = rightCost;
            rightNode->depth = currentNode->depth + 1;
            rightNode->direction = 'R';
            bool inOpenList = false;
            if (addedNodes.find(rightNode->puzzle) != addedNodes.end())
            {
                Node *node = *addedNodes[rightNode->puzzle];
                if (node->direction == rightNode->direction)
                    inOpenList = true;
                if (inOpenList == true && node->cost > rightNode->cost)
                {
                    node->cost = rightNode->cost;
                    node->parent = rightNode->parent;
                    node->depth = rightNode->depth;
                    node->direction = rightNode->direction;
                }
            }
            bool inClosedList = false;
            if (closedList.find(rightNode->puzzle) != closedList.end()) {
                Node *node = *closedList[rightNode->puzzle];
                inClosedList = true;
                if (inClosedList == true && node->cost > rightNode->cost)
                {
                    node->cost = rightNode->cost;
                    node->parent = rightNode->parent;
                    node->depth = rightNode->depth;
                    node->direction = rightNode->direction;
                }
            }
            // for (int i = 0; i < closedList.size(); i++)
            // {
            //     if (closedList[i]->puzzle == rightNode->puzzle)
            //     {
            //         inClosedList = true;
            //         if (closedList[i]->cost > rightNode->cost)
            //         {
            //             closedList[i]->cost = rightNode->cost;
            //             closedList[i]->parent = rightNode->parent;
            //             closedList[i]->depth = rightNode->depth;
            //             closedList[i]->direction = rightNode->direction;
            //         }
            //     }
            // }
            if (inOpenList == false && inClosedList == false)
                openList.push(rightNode);
            else
                delete rightNode;
        }
    }
    else if (currentNode->direction == 'L')
    {
        int upCost = -1;
        int downCost = -1;
        int leftCost = -1;
        std::vector<std::vector<int> > upPuzzleResult = upPuzzle(puzzle);
        if (upPuzzleResult != puzzle)
            upCost = currentNode->depth + 1 + (this->*heuristic)(upPuzzleResult);
        std::vector<std::vector<int> > downPuzzleResult = downPuzzle(puzzle);
        if (downPuzzleResult != puzzle)
            downCost = currentNode->depth + 1 + (this->*heuristic)(downPuzzleResult);
        std::vector<std::vector<int> > leftPuzzleResult = leftPuzzle(puzzle);
        if (leftPuzzleResult != puzzle)
            leftCost = currentNode->depth + 1 + (this->*heuristic)(leftPuzzleResult);
        if (upCost != -1)
        {
            Node *upNode = new Node;
            upNode->puzzle = upPuzzleResult;
            upNode->parent = currentNode;
            upNode->cost = upCost;
            upNode->depth = currentNode->depth + 1;
            upNode->direction = 'U';
            bool inOpenList = false;
            if (addedNodes.find(upNode->puzzle) != addedNodes.end())
            {
                Node *node = *addedNodes[upNode->puzzle];
                if (node->direction == upNode->direction)
                    inOpenList = true;
                if (inOpenList == true && node->cost > upNode->cost)
                {
                    node->cost = upNode->cost;
                    node->parent = upNode->parent;
                    node->depth = upNode->depth;
                    node->direction = upNode->direction;
                }
            }
            bool inClosedList = false;
            if (closedList.find(upNode->puzzle) != closedList.end()) {
                Node *node = *closedList[upNode->puzzle];
                inClosedList = true;
                if (inClosedList == true && node->cost > upNode->cost)
                {
                    node->cost = upNode->cost;
                    node->parent = upNode->parent;
                    node->depth = upNode->depth;
                    node->direction = upNode->direction;
                }
            }
            // for (int i = 0; i < closedList.size(); i++)
            // {
            //     if (closedList[i]->puzzle == upNode->puzzle)
            //     {
            //         inClosedList = true;
            //         if (closedList[i]->cost > upNode->cost)
            //         {
            //             closedList[i]->cost = upNode->cost;
            //             closedList[i]->parent = upNode->parent;
            //             closedList[i]->depth = upNode->depth;
            //             closedList[i]->direction = upNode->direction;
            //         }
            //     }
            // }
            if (inOpenList == false && inClosedList == false)
                openList.push(upNode);
            else
                delete upNode;
        }
        if (downCost != -1)
        {
            Node *downNode = new Node;
            downNode->puzzle = downPuzzleResult;
            downNode->parent = currentNode;
            downNode->cost = downCost;
            downNode->depth = currentNode->depth + 1;
            downNode->direction = 'D';
            bool inOpenList = false;
            if (addedNodes.find(downNode->puzzle) != addedNodes.end())
            {
                Node *node = *addedNodes[downNode->puzzle];
                if (node->direction == downNode->direction)
                    inOpenList = true;
                if (inOpenList == true && node->cost > downNode->cost)
                {
                    node->cost = downNode->cost;
                    node->parent = downNode->parent;
                    node->depth = downNode->depth;
                    node->direction = downNode->direction;
                }
            }
            bool inClosedList = false;
            if (closedList.find(downNode->puzzle) != closedList.end()) {
                Node *node = *closedList[downNode->puzzle];
                inClosedList = true;
                if (inClosedList == true && node->cost > downNode->cost)
                {
                    node->cost = downNode->cost;
                    node->parent = downNode->parent;
                    node->depth = downNode->depth;
                    node->direction = downNode->direction;
                }
            }
            // for (int i = 0; i < closedList.size(); i++)
            // {
            //     if (closedList[i]->puzzle == downNode->puzzle)
            //     {
            //         inClosedList = true;
            //         if (closedList[i]->cost > downNode->cost)
            //         {
            //             closedList[i]->cost = downNode->cost;
            //             closedList[i]->parent = downNode->parent;
            //             closedList[i]->depth = downNode->depth;
            //             closedList[i]->direction = downNode->direction;
            //         }
            //     }
            // }
            if (inOpenList == false && inClosedList == false)
                openList.push(downNode);
            else
                delete downNode;
        }
        if (leftCost != -1)
        {
            Node *leftNode = new Node;
            leftNode->puzzle = leftPuzzleResult;
            leftNode->parent = currentNode;
            leftNode->cost = leftCost;
            leftNode->depth = currentNode->depth + 1;
            leftNode->direction = 'L';
            bool inOpenList = false;
            if (addedNodes.find(leftNode->puzzle) != addedNodes.end())
            {
                Node *node = *addedNodes[leftNode->puzzle];
                if (node->direction == leftNode->direction)
                    inOpenList = true;
                if (inOpenList == true && node->cost > leftNode->cost)
                {
                    node->cost = leftNode->cost;
                    node->parent = leftNode->parent;
                    node->depth = leftNode->depth;
                    node->direction = leftNode->direction;
                }
            }
            bool inClosedList = false;
            if (closedList.find(leftNode->puzzle) != closedList.end()) {
                Node *node = *closedList[leftNode->puzzle];
                inClosedList = true;
                if (inClosedList == true && node->cost > leftNode->cost)
                {
                    node->cost = leftNode->cost;
                    node->parent = leftNode->parent;
                    node->depth = leftNode->depth;
                    node->direction = leftNode->direction;
                }
            }
            // for (int i = 0; i < closedList.size(); i++)
            // {
            //     if (closedList[i]->puzzle == leftNode->puzzle)
            //     {
            //         inClosedList = true;
            //         if (closedList[i]->cost > leftNode->cost)
            //         {
            //             closedList[i]->cost = leftNode->cost;
            //             closedList[i]->parent = leftNode->parent;
            //             closedList[i]->depth = leftNode->depth;
            //             closedList[i]->direction = leftNode->direction;
            //         }
            //     }
            // }
            if (inOpenList == false && inClosedList == false)
                openList.push(leftNode);
            else
                delete leftNode;
        }
    }
    else if (currentNode->direction == 'R')
    {
        int upCost = -1;
        int downCost = -1;
        int rightCost = -1;
        std::vector<std::vector<int> > upPuzzleResult = upPuzzle(puzzle);
        if (upPuzzleResult != puzzle)
            upCost = currentNode->depth + 1 + (this->*heuristic)(upPuzzleResult);
        std::vector<std::vector<int> > downPuzzleResult = downPuzzle(puzzle);
        if (downPuzzleResult != puzzle)
            downCost = currentNode->depth + 1 + (this->*heuristic)(downPuzzleResult);
        std::vector<std::vector<int> > rightPuzzleResult = rightPuzzle(puzzle);
        if (rightPuzzleResult != puzzle)
            rightCost = currentNode->depth + 1 + (this->*heuristic)(rightPuzzleResult);
        if (upCost != -1)
        {
            Node *upNode = new Node;
            upNode->puzzle = upPuzzleResult;
            upNode->parent = currentNode;
            upNode->cost = upCost;
            upNode->depth = currentNode->depth + 1;
            upNode->direction = 'U';
            bool inOpenList = false;
            if (addedNodes.find(upNode->puzzle) != addedNodes.end())
            {
                Node *node = *addedNodes[upNode->puzzle];
                if (node->direction == upNode->direction)
                    inOpenList = true;
                if (inOpenList == true && node->cost > upNode->cost)
                {
                    node->cost = upNode->cost;
                    node->parent = upNode->parent;
                    node->depth = upNode->depth;
                    node->direction = upNode->direction;
                }
            }
            bool inClosedList = false;
            if (closedList.find(upNode->puzzle) != closedList.end()) {
                Node *node = *closedList[upNode->puzzle];
                inClosedList = true;
                if (inClosedList == true && node->cost > upNode->cost)
                {
                    node->cost = upNode->cost;
                    node->parent = upNode->parent;
                    node->depth = upNode->depth;
                    node->direction = upNode->direction;
                }
            }
            // for (int i = 0; i < closedList.size(); i++)
            // {
            //     if (closedList[i]->puzzle == upNode->puzzle)
            //     {
            //         inClosedList = true;
            //         if (closedList[i]->cost > upNode->cost)
            //         {
            //             closedList[i]->cost = upNode->cost;
            //             closedList[i]->parent = upNode->parent;
            //             closedList[i]->depth = upNode->depth;
            //             closedList[i]->direction = upNode->direction;
            //         }
            //     }
            // }
            if (inOpenList == false && inClosedList == false)
                openList.push(upNode);
            else
                delete upNode;
        }
        if (downCost != -1)
        {
            Node *downNode = new Node;
            downNode->puzzle = downPuzzleResult;
            downNode->parent = currentNode;
            downNode->cost = downCost;
            downNode->depth = currentNode->depth + 1;
            downNode->direction = 'D';
            bool inOpenList = false;
            if (addedNodes.find(downNode->puzzle) != addedNodes.end())
            {
                Node *node = *addedNodes[downNode->puzzle];
                if (node->direction == downNode->direction)
                    inOpenList = true;
                if (inOpenList == true && node->cost > downNode->cost)
                {
                    node->cost = downNode->cost;
                    node->parent = downNode->parent;
                    node->depth = downNode->depth;
                    node->direction = downNode->direction;
                }
            }
            bool inClosedList = false;
            if (closedList.find(downNode->puzzle) != closedList.end()) {
                Node *node = *closedList[downNode->puzzle];
                inClosedList = true;
                if (inClosedList == true && node->cost > downNode->cost)
                {
                    node->cost = downNode->cost;
                    node->parent = downNode->parent;
                    node->depth = downNode->depth;
                    node->direction = downNode->direction;
                }
            }
            // for (int i = 0; i < closedList.size(); i++)
            // {
            //     if (closedList[i]->puzzle == downNode->puzzle)
            //     {
            //         inClosedList = true;
            //         if (closedList[i]->cost > downNode->cost)
            //         {
            //             closedList[i]->cost = downNode->cost;
            //             closedList[i]->parent = downNode->parent;
            //             closedList[i]->depth = downNode->depth;
            //             closedList[i]->direction = downNode->direction;
            //         }
            //     }
            // }
            if (inOpenList == false && inClosedList == false)
                openList.push(downNode);
            else
                delete downNode;
        }
        if (rightCost != -1)
        {
            Node *rightNode = new Node;
            rightNode->puzzle = rightPuzzleResult;
            rightNode->parent = currentNode;
            rightNode->cost = rightCost;
            rightNode->depth = currentNode->depth + 1;
            rightNode->direction = 'R';
            bool inOpenList = false;
            if (addedNodes.find(rightNode->puzzle) != addedNodes.end())
            {
                Node *node = *addedNodes[rightNode->puzzle];
                if (node->direction == rightNode->direction)
                    inOpenList = true;
                if (inOpenList == true && node->cost > rightNode->cost)
                {
                    node->cost = rightNode->cost;
                    node->parent = rightNode->parent;
                    node->depth = rightNode->depth;
                    node->direction = rightNode->direction;
                }
            }
            bool inClosedList = false;
            if (closedList.find(rightNode->puzzle) != closedList.end()) {
                Node *node = *closedList[rightNode->puzzle];
                inClosedList = true;
                if (inClosedList == true && node->cost > rightNode->cost)
                {
                    node->cost = rightNode->cost;
                    node->parent = rightNode->parent;
                    node->depth = rightNode->depth;
                    node->direction = rightNode->direction;
                }
            }
            // for (int i = 0; i < closedList.size(); i++)
            // {
            //     if (closedList[i]->puzzle == rightNode->puzzle)
            //     {
            //         inClosedList = true;
            //         if (closedList[i]->cost > rightNode->cost)
            //         {
            //             closedList[i]->cost = rightNode->cost;
            //             closedList[i]->parent = rightNode->parent;
            //             closedList[i]->depth = rightNode->depth;
            //             closedList[i]->direction = rightNode->direction;
            //         }
            //     }
            // }
            if (inOpenList == false && inClosedList == false)
                openList.push(rightNode);
            else
                delete rightNode;
        }
    }
    else
    {
        throw std::invalid_argument("Invalid direction");
    }
}

int PuzzleSolver::euclidianDistance(std::vector<std::vector<int> > puzzle)
{
    int distance = 0;

    for (int i = 0; i < puzzle.size(); i++)
    {
        for (int j = 0; j < puzzle[i].size(); j++)
        {
            int value = puzzle[i][j];
            if (value == 0)
                continue; // Skip the blank space

            auto expectedPos = _snailPositions[value];
            distance += sqrt(pow(i - expectedPos.first, 2) + pow(j - expectedPos.second, 2));
        }
    }

    return distance;
}


int PuzzleSolver::linearConflict(std::vector<std::vector<int> > puzzle)
{
    int linearConflict = manhattanDistance(puzzle);

    std::vector<int> tiles;
    int total = _lineLength * _lineLength;
    int size = _lineLength;
    int column = 0;
    int row = -1;

    for (int i = 0; total != 0; i++) {
        if (i % 4 == 0) {
            size--;
            for (;row < size; total--)
                tiles.push_back(puzzle[column][row++]);
        }
        else if (i % 4 == 1)
            for (;column < size; total--)
                tiles.push_back(puzzle[column++][row]);
        else if (i % 4 == 2)
            for (;row >= _lineLength - size; total--)
                tiles.push_back(puzzle[column][row--]);
        else
            for (;column >= _lineLength - size; total--)
                tiles.push_back(puzzle[column--][row]);
    }

    for (int column = 0; column < _lineLength; column++)
    {
        for (int row = 0; row < _lineLength; row++)
        {
            int tile = tiles[row + column * _lineLength];
            if (tile == 0)
                continue;
            else {
                for (int otherTiles = 0; otherTiles < _lineLength; otherTiles++)
                {
                    if (otherTiles == row)
                        continue;
                    int otherTile = tiles[otherTiles + column * _lineLength];
                    if (otherTile == 0)
                        continue;
                    else
                    {
                        if ((tile > otherTile && row < otherTiles && tile < row + 1) || (tile < otherTile && row > otherTiles && tile > row + 1))
                            linearConflict++;
                    }
                }
            }
        }
    }

    for (int row = 0; row < _lineLength; row++)
    {
        for (int column = 0; column < _lineLength; column++)
        {
            int tile = tiles[row + column * _lineLength];
            if (tile == 0)
                continue;
            else {
                for (int otherTiles = 0; otherTiles < _lineLength; otherTiles++)
                {
                    if (otherTiles == column)
                        continue;
                    int otherTile = tiles[row + otherTiles * _lineLength];
                    if (otherTile == 0)
                        continue;
                    else
                    {
                        if ((tile > otherTile && column < otherTiles && tile < column + 1) || (tile < otherTile && column > otherTiles && tile > column + 1))
                            linearConflict++;
                    }
                }
            }
        }
    }

    return linearConflict;
}

int PuzzleSolver::manhattanDistance(std::vector<std::vector<int> > puzzle)
{

    int distance = 0;

    for (int i = 0; i < puzzle.size(); i++)
    {
        for (int j = 0; j < puzzle[i].size(); j++)
        {
            int value = puzzle[i][j];
            if (value == 0)
                continue; // Skip the blank space

            auto expectedPos = _snailPositions[value];
            distance += abs(i - expectedPos.first) + abs(j - expectedPos.second);
        }
    }

    return distance;
}

std::vector<std::vector<int> > PuzzleSolver::upPuzzle(std::vector<std::vector<int> > puzzle)
{
    std::vector<std::vector<int> > upPuzzle = puzzle;
    for (int i = 0; i < upPuzzle.size(); i++)
    {
        for (int j = 0; j < upPuzzle[i].size(); j++)
        {
            if (upPuzzle[i][j] == 0)
            {
                if (i == 0)
                {
                    return puzzle;
                }
                else
                {
                    upPuzzle[i][j] = upPuzzle[i - 1][j];
                    upPuzzle[i - 1][j] = 0;
                    return upPuzzle;
                }
            }
        }
    }
    return upPuzzle;
}

std::vector<std::vector<int> > PuzzleSolver::downPuzzle(std::vector<std::vector<int> > puzzle)
{
    std::vector<std::vector<int> > downPuzzle = puzzle;

    for (int i = 0; i < downPuzzle.size(); i++)
    {
        for (int j = 0; j < downPuzzle[i].size(); j++)
        {
            if (downPuzzle[i][j] == 0)
            {
                if (i == downPuzzle.size() - 1)
                {
                    return puzzle;
                }
                else
                {
                    downPuzzle[i][j] = downPuzzle[i + 1][j];
                    downPuzzle[i + 1][j] = 0;
                    return downPuzzle;
                }
            }
        }
    }
    return downPuzzle;
}

std::vector<std::vector<int> > PuzzleSolver::leftPuzzle(std::vector<std::vector<int> > puzzle)
{
    std::vector<std::vector<int> > leftPuzzle = puzzle;
    for (int i = 0; i < leftPuzzle.size(); i++)
    {
        for (int j = 0; j < leftPuzzle[i].size(); j++)
        {
            if (leftPuzzle[i][j] == 0)
            {
                if (j == 0)
                {
                    return puzzle;
                }
                else
                {
                    leftPuzzle[i][j] = leftPuzzle[i][j - 1];
                    leftPuzzle[i][j - 1] = 0;
                    return leftPuzzle;
                }
            }
        }
    }
    return leftPuzzle;
}

std::vector<std::vector<int> > PuzzleSolver::rightPuzzle(std::vector<std::vector<int> > puzzle)
{
    std::vector<std::vector<int> > rightPuzzle = puzzle;
    for (int i = 0; i < rightPuzzle.size(); i++)
    {
        for (int j = 0; j < rightPuzzle[i].size(); j++)
        {
            if (rightPuzzle[i][j] == 0)
            {
                if (j == rightPuzzle[i].size() - 1)
                {
                    return puzzle;
                }
                else
                {
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

std::vector<std::vector<int> > PuzzleSolver::getPuzzle() const
{
    return _puzzle;
}

// Setters

void PuzzleSolver::setLineLength(int lineLength)
{
    this->_lineLength = lineLength;
}