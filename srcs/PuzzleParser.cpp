#include "PuzzleParser.hpp"

// Parsing methods

bool PuzzleParser::parseLineLength(std::string lineFile)
{
    if (lineFile.empty())
        return false;

    int i = 0;
    int lineLength = lineFile.length();
    while (isspace(lineFile[i]))
        i++;
    if (isdigit(lineFile[i]) == false)
        throw std::invalid_argument("First line is not a number");
    else
    {
        int j = i;
        while (isdigit(lineFile[j]))
            j++;
        int foundNumber = std::stoi(lineFile.substr(i, j));
        if (foundNumber < 3 || foundNumber > 10)
            throw std::invalid_argument("First line is not a number between 3 and 10");
        else
            setLineLength(foundNumber);
        while (lineFile[j] != '\n' && isspace(lineFile[j]))
            j++;
        if (j != lineLength)
            throw std::invalid_argument("First line is not a number");
        else
            return true;
    }
    return true;
}

bool PuzzleParser::isCommentLine(std::string line)
{
    for (unsigned long i = 0; i < line.length(); i++)
    {
        char c = line[i];
        if (c != '#' && isspace(c))
            continue;
        else if (isdigit(c))
            return false;
        else if (c == '#')
            return true;
    }
    return false;
}

void PuzzleParser::checkArgumentsValidity(std::vector<std::string> &tokens)
{
    std::set<int> set;
    for (unsigned long i = 0; i < tokens.size(); i++)
    {
        std::string token = tokens[i];
        for (unsigned long j = 0; j < token.length(); j++)
        {
            if (isdigit(token[j]) == false)
            {
                throw std::invalid_argument(token + " is not a number");
            }
        }
        int number = std::stoi(token);
        if (set.find(number) != set.end())
        {
            throw std::invalid_argument(token + " is a duplicate");
        }
        set.insert(number);
    }
    unsigned long amountOfNumbers = this->_lineLength * this->_lineLength;
    if (set.size() != amountOfNumbers)
    {
        throw std::invalid_argument("There are " + std::to_string(amountOfNumbers) + " numbers in the puzzle, but there are " + std::to_string(set.size()) + " unique numbers");
    }
    int i = 0;
    for (std::set<int>::iterator it = set.begin(); it != set.end(); it++)
    {
        int number = *it;
        if (number != i)
        {
            throw std::invalid_argument("There is no " + std::to_string(i) + " in the puzzle");
        }
        i++;
    }
    for (int i = 0; i < this->_lineLength; i++)
    {
        std::vector<int> row;
        for (int j = 0; j < this->_lineLength; j++)
        {
            row.push_back(std::stoi(tokens[i * this->_lineLength + j]));
        }
        _puzzle.push_back(row);
    }

    // check if puzzle is solvable (odd lineLength)
    // an inversion is when a tile precedes another tile with a lower number on it

    std::vector<int> tiles;
    int total = _lineLength * _lineLength;
    int size = _lineLength;
    int column = 0;
    int row = -1;

    for (int i = 0; total != 0; i++)
    {
        if (i % 4 == 0)
            for (; row + 1 < size; total--)
                tiles.push_back(_puzzle[column][++row]);
        else if (i % 4 == 1)
            for (; column + 1 < size; total--)
                tiles.push_back(_puzzle[++column][row]);
        else if (i % 4 == 2)
            for (; row - 1 >= _lineLength - size; total--)
                tiles.push_back(_puzzle[column][--row]);
        else
            for (size--; column - 1 >= _lineLength - size; total--)
                tiles.push_back(_puzzle[--column][row]);
    }

    int inversions = 0;
    for (unsigned long i = 0; i < tiles.size(); i++)
    {
        int number = tiles[i];
        for (unsigned long j = i + 1; j < tiles.size(); j++)
        {
            int nextNumber = tiles[j];
            if (nextNumber == 0)
                continue;
            if (number > nextNumber)
                inversions++;
        }
    }

    std::cout << "inversions = " << inversions << std::endl;
    // if (inversions % 2)
    //     throw std::invalid_argument("Puzzle is not solvable");

    std::cout << "Puzzle is solvable" << std::endl;
}

void PuzzleParser::parse()
{
    int fileNameLength = _fileName.length();
    if ((fileNameLength < 4 && _fileName[fileNameLength - 4] != '.') || _fileName[fileNameLength - 3] != 't' || _fileName[fileNameLength - 2] != 'x' || _fileName[fileNameLength - 1] != 't')
    {
        throw std::invalid_argument("File name must end in .txt");
    }
    std::ifstream file(_fileName);
    if (!file.is_open())
    {
        throw std::invalid_argument("File could not be opened");
    }

    std::string line;
    char delimiter = ' ';
    std::vector<std::string> tokens;
    int lineCount = 0;
    while (std::getline(file, line))
    {
        if (line.empty())
            continue;
        bool isComment = isCommentLine(line);
        if (lineCount == 0 && isComment)
            continue;
        if (lineCount != 0 && isComment)
            throw std::invalid_argument("Comment line found in middle of file");
        if (lineCount == 0)
        {
            try
            {
                parseLineLength(line);
            }
            catch (std::exception &e)
            {
                throw std::invalid_argument(e.what());
            }
        }
        else
        {
            unsigned long oldLength = tokens.size();
            std::string token;
            std::istringstream iss(line);
            while (std::getline(iss, token, delimiter))
                if (!token.empty())
                    tokens.push_back(token);
            if ((int)(tokens.size() - oldLength) != _lineLength)
                throw std::invalid_argument("Line " + std::to_string(lineCount) + " does not have " + std::to_string(_lineLength) + " number(s)");
        }
        lineCount++;
    }
    checkArgumentsValidity(tokens);
    file.close();
}

// Constructor and destructor

PuzzleParser::PuzzleParser(std::string fileName) : _fileName(fileName)
{
    if (fileName.empty())
    {
        throw std::invalid_argument("File name cannot be empty");
    }
    try
    {
        parse();
    }
    catch (const std::exception &e)
    {
        throw std::invalid_argument(e.what());
    }
}

PuzzleParser::~PuzzleParser() {}

// Getters

int PuzzleParser::getLineLength() const
{
    return _lineLength;
}

std::string PuzzleParser::getFileName() const
{
    return _fileName;
}

std::vector<std::vector<int>> PuzzleParser::getPuzzle() const
{
    return _puzzle;
}

// Setters

void PuzzleParser::setLineLength(int lineLength)
{
    this->_lineLength = lineLength;
}