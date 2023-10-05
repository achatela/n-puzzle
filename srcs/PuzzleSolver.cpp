#include "PuzzleSolver.hpp"

// Constructor and destructor

PuzzleSolver::PuzzleSolver(std::vector<std::vector<int>> puzzle, int lineLength, std::string heuristicInput, std::string algorithmInput) : _puzzle(puzzle), _lineLength(lineLength)
{

    int field;
    if (algorithmInput == "greedy")
    {
        field = 1;
    }
    else if (algorithmInput == "astar")
    {
        field = 0;
    }
    else
        throw std::invalid_argument("Invalid algorithm");

    CompareNode cmp(field);

    std::priority_queue<Node *, std::vector<Node *>, CompareNode> openList(cmp);
    std::map<std::vector<std::vector<int>>, Node *> closedList;

    int (PuzzleSolver::*heuristic)(std::vector<std::vector<int>> puzzle);
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

    _root = new Node;
    _root->puzzle = puzzle;
    _root->parent = nullptr;
    _root->cost = 0;
    _root->depth = 0;
    _root->direction = 'N'; // none

    // closedList[puzzle] = _root;
    openList.push(_root);
    Node *currentNode = openList.top();
    openList.pop();
    closedList[currentNode->puzzle] = currentNode;

    int timeComplexity = 0;

    setSnailPuzzle();
    while (currentNode->puzzle != _snailSolution)
    {
        addNeighbours(currentNode, openList, closedList, heuristic);
        // currentNode->puzzle.clear();
        currentNode = openList.top();
        openList.pop();
        closedList[currentNode->puzzle] = currentNode;
        timeComplexity++;
    }

    std::cout << "Solved!" << std::endl;
    for (int i = 0; i < _lineLength; i++)
    {
        for (int j = 0; j < _lineLength; j++)
        {
            if (j == _lineLength - 1)
                std::cout << currentNode->puzzle[i][j];
            else
                std::cout << currentNode->puzzle[i][j] << " ";
        }
        std::cout << std::endl;
    }
    std::string solution;
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

    while (!openList.empty())
    {
        currentNode = openList.top();
        openList.pop();
        delete currentNode;
    }
    for (auto it = closedList.begin(); it != closedList.end(); it++)
    {
        delete it->second;
    }
}

PuzzleSolver::~PuzzleSolver() {}

void PuzzleSolver::setSnailPuzzle()
{

    // if (_lineLength == 3)
    // {
    //     _snailSolution = {{1, 2, 3}, {8, 0, 4}, {7, 6, 5}};
    // }
    // else if (_lineLength == 4)
    // {
    //     _snailSolution = {{1, 2, 3, 4}, {12, 13, 14, 5}, {11, 0, 15, 6}, {10, 9, 8, 7}};
    // }
    // else if (_lineLength == 5)
    // {
    //     _snailSolution = {{1, 2, 3, 4, 5}, {16, 17, 18, 19, 6}, {15, 24, 0, 20, 7}, {14, 23, 22, 21, 8}, {13, 12, 11, 10, 9}};
    // }
    // for (unsigned long i = 0; i < _snailSolution.size(); i++)
    // {
    //     for (unsigned long j = 0; j < _snailSolution[i].size(); j++)
    //     {
    //         _snailPositions[_snailSolution[i][j]] = std::make_pair(i, j);
    //     }
    // }
    int total = _lineLength * _lineLength;
    int size = _lineLength;
    int number = 1;
    int column = 0;
    int row = -1;

    for (int i = 0; i != _lineLength; i++)
    {
        _snailSolution.push_back(std::vector<int>());
        for (int j = 0; j != _lineLength; j++)
            _snailSolution[i].push_back(0);
    }

    for (int i = 0; number != total; i++)
    {
        if (i % 4 == 0)
            while (row + 1 < size)
                _snailSolution[column][++row] = number++;
        else if (i % 4 == 1)
            while (column + 1 < size)
                _snailSolution[++column][row] = number++;
        else if (i % 4 == 2)
            while (row - 1 >= _lineLength - size)
                _snailSolution[column][--row] = number++;
        else
            for (size--; column - 1 >= _lineLength - size;)
                _snailSolution[--column][row] = number++;
    }

    for (int i = 0; i < _lineLength; i++)
    {
        for (int j = 0; j < _lineLength; j++)
            _snailPositions[_snailSolution[i][j]] = std::make_pair(i, j);
    }

    // std::cout << "Snail solution look like:" << std::endl;
    // for (int i = 0; i < _lineLength; i++)
    // {
    //     for (int j = 0; j < _lineLength; j++)
    //     {
    //         if (j == _lineLength - 1)
    //             std::cout << _snailSolution[i][j];
    //         else
    //             std::cout << _snailSolution[i][j] << " ";
    //     }
    //     std::cout << std::endl;
    // }
}

void PuzzleSolver::addNeighbours(Node *currentNode, std::priority_queue<Node *, std::vector<Node *>, CompareNode> &openList, std::map<std::vector<std::vector<int>>, Node *> &closedList, int (PuzzleSolver::*heuristic)(std::vector<std::vector<int>> puzzle))
{
    if (currentNode->direction != 'D')
        pushNode('U', currentNode, heuristic, openList, closedList);
    if (currentNode->direction != 'U')
        pushNode('D', currentNode, heuristic, openList, closedList);
    if (currentNode->direction != 'R')
        pushNode('L', currentNode, heuristic, openList, closedList);
    if (currentNode->direction != 'L')
        pushNode('R', currentNode, heuristic, openList, closedList);
}

void PuzzleSolver::pushNode(char direction, Node *parentNode, int (PuzzleSolver::*heuristic)(std::vector<std::vector<int>> puzzle), std::priority_queue<Node *, std::vector<Node *>, CompareNode> &openList, std::map<std::vector<std::vector<int>>, Node *> &closedList)
{
    std::vector<std::vector<int>> puzzle = makePuzzle(direction, parentNode->puzzle);
    if (puzzle.empty())
        return;
    int cost = (this->*heuristic)(puzzle) + parentNode->depth + 1;
    if (closedList.find(puzzle) != closedList.end())
    {
        Node *closedNode = closedList[puzzle];
        if (cost < closedNode->cost)
        {
            std::cout << "This should not happen" << std::endl;
            //     closedNode->direction = direction;
            //     closedNode->cost = cost;
            //     closedNode->depth = parentNode->depth + 1;
            //     closedNode->parent = parentNode;
        }
        return;
    }
    if (_openListSet.insert(puzzle).second)
    {
        Node *newNode = new Node;
        newNode->direction = direction;
        newNode->puzzle = puzzle;
        newNode->cost = cost;
        newNode->depth = parentNode->depth + 1;
        newNode->parent = parentNode;
        openList.push(newNode);
    }
}

// Solving methods

int PuzzleSolver::manhattanDistance(std::vector<std::vector<int>> puzzle)
{
    int distance = 0;

    for (int i = 0; i < _lineLength; i++)
    {
        for (int j = 0; j < _lineLength; j++)
        {
            int value = puzzle[i][j];
            if (value == 0)
                continue;

            auto expectedPos = _snailPositions[value];
            distance += abs(i - expectedPos.first) + abs(j - expectedPos.second);
        }
    }

    return distance;
}

int PuzzleSolver::euclidianDistance(std::vector<std::vector<int>> puzzle)
{
    int distance = 0;

    for (int i = 0; i < _lineLength; i++)
    {
        for (int j = 0; j < _lineLength; j++)
        {
            int value = puzzle[i][j];
            if (value == 0)
                continue;

            auto expectedPos = _snailPositions[value];
            distance += sqrt(pow(i - expectedPos.first, 2) + pow(j - expectedPos.second, 2));
        }
    }

    return distance;
}

int PuzzleSolver::linearConflict(std::vector<std::vector<int>> puzzle)
{
    int linearConflict = manhattanDistance(puzzle);

    std::vector<int> tiles;
    int total = _lineLength * _lineLength;
    int size = _lineLength;
    int column = 0;
    int row = -1;

    for (int i = 0; total != 0; i++)
    {
        if (i % 4 == 0)
            for (; row + 1 < size; total--)
                tiles.push_back(puzzle[column][++row]);
        else if (i % 4 == 1)
            for (; column + 1 < size; total--)
                tiles.push_back(puzzle[++column][row]);
        else if (i % 4 == 2)
            for (; row - 1 >= _lineLength - size; total--)
                tiles.push_back(puzzle[column][--row]);
        else
            for (size--; column - 1 >= _lineLength - size; total--)
                tiles.push_back(puzzle[--column][row]);
    }

    for (int column = 0; column < _lineLength; column++)
    {
        for (int row = 0; row < _lineLength; row++)
        {
            int tile = tiles[row + column * _lineLength];
            if (tile == 0)
                continue;
            else
            {
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
            else
            {
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

int PuzzleSolver::uniformCostFakeHeuristic(std::vector<std::vector<int>> puzzle)
{
    (void)puzzle;
    return 0;
}

std::vector<std::vector<int>> PuzzleSolver::makePuzzle(char direction, std::vector<std::vector<int>> puzzle)
{
    for (int i = 0; i < _lineLength; i++)
    {
        for (int j = 0; j < _lineLength; j++)
        {
            if (puzzle[i][j] == 0)
            {
                if (direction == 'U')
                {
                    if (i == 0)
                        return std::vector<std::vector<int>>();
                    else
                    {
                        puzzle[i][j] = puzzle[i - 1][j];
                        puzzle[i - 1][j] = 0;
                    }
                }
                else if (direction == 'D')
                {
                    if (i == _lineLength - 1)
                        return std::vector<std::vector<int>>();
                    else
                    {
                        puzzle[i][j] = puzzle[i + 1][j];
                        puzzle[i + 1][j] = 0;
                    }
                }
                else if (direction == 'L')
                {
                    if (j == 0)
                        return std::vector<std::vector<int>>();
                    else
                    {
                        puzzle[i][j] = puzzle[i][j - 1];
                        puzzle[i][j - 1] = 0;
                    }
                }
                else
                {
                    if (j == _lineLength - 1)
                        return std::vector<std::vector<int>>();
                    else
                    {
                        puzzle[i][j] = puzzle[i][j + 1];
                        puzzle[i][j + 1] = 0;
                    }
                }
                return puzzle;
            }
        }
    }
    return std::vector<std::vector<int>>();
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
