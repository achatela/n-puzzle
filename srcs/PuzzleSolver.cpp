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

    openList.push(_root);
    Node *currentNode = openList.top();
    openList.pop();
    closedList[currentNode->puzzle] = currentNode;

    int timeComplexity = 0;

    setSnailPuzzle();
    while (currentNode->puzzle != _snailSolution)
    {
        addNeighbours(currentNode, openList, closedList, heuristic);
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
            closedNode->direction = direction;
            closedNode->cost = cost;
            closedNode->depth = parentNode->depth + 1;
            closedNode->parent = parentNode;
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
    std::set<int> conflicts;

    for (int y = 0; y < _lineLength; y++)
    {
        for (int x = 0; x < _lineLength; x++)
        {
            int tile = puzzle[y][x];
            if (tile == 0)
                continue;
            std::pair<int, int> expectedPos = _snailPositions[tile];
            if (y != expectedPos.first)
                continue;
            for (int x2 = 0; x2 < _lineLength; x2++)
            {
                if (x2 == x)
                    continue;
                int otherTile = puzzle[y][x2];
                if (otherTile == 0)
                    continue;
                std::pair<int, int> otherExpectedPos = _snailPositions[otherTile];
                if (otherExpectedPos.first != y || (otherExpectedPos.second <= expectedPos.second && x2 <= x) || (otherExpectedPos.second >= expectedPos.second && x2 >= x))
                    continue;
                conflicts.insert(otherTile);
            }
        }
        linearConflict += conflicts.size();
        conflicts.clear();
    }

    for (int x = 0; x < _lineLength; x++)
    {
        for (int y = 0; y < _lineLength; y++)
        {
            int tile = puzzle[y][x];
            if (tile == 0)
                continue;
            std::pair<int, int> expectedPos = _snailPositions[tile];
            if (x != expectedPos.second)
                continue;
            for (int y2 = 0; y2 < _lineLength; y2++)
            {
                if (y2 == y)
                    continue;
                int otherTile = puzzle[y2][x];
                if (otherTile == 0)
                    continue;
                std::pair<int, int> otherExpectedPos = _snailPositions[otherTile];
                if (otherExpectedPos.second != x || (otherExpectedPos.first <= expectedPos.first && y2 <= y) || (otherExpectedPos.first >= expectedPos.first && y2 >= y))
                    continue;
                conflicts.insert(otherTile);
            }
        }
        linearConflict += conflicts.size();
        conflicts.clear();
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
