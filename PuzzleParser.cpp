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
        return false;
    else
    {
        int j = i;
        while (isdigit(lineFile[j]))
            j++;
        setLineLength(std::stoi(lineFile.substr(i, j)));
        while (lineFile[j] != '\n' && isspace(lineFile[j]))
            j++;
        if (lineFile[j] != '\n')
            throw std::invalid_argument("Line length must be followed by a newline character");
    }
    return true;
}

bool PuzzleParser::isCommentLine(std::string line)
{
    for (int i = 0; i < line.length(); i++)
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

void PuzzleParser::parse()
{
    int fileNameLength = _fileName.length();
    if (fileNameLength < 4 && _fileName[fileNameLength - 4] != '.' || _fileName[fileNameLength - 3] != 't' || _fileName[fileNameLength - 2] != 'x' || _fileName[fileNameLength - 1] != 't')
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
            if (parseLineLength(line) == false)
                throw std::invalid_argument("Invalid line length");
        }
        else
        {
            const char *cstr = line.c_str();
            char *token = strtok((char *)cstr, &delimiter);
            while (token != NULL)
            {
                tokens.push_back(std::string(token));
                token = strtok(NULL, &delimiter);
            }
            // compter uniquement avant '#'
        }
        lineCount++;
    }
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
        std::cerr << e.what() << std::endl;
    }
}

PuzzleParser::~PuzzleParser() {}

// Getters

int PuzzleParser::getLineLength() const
{
    return lineLength;
}

std::string PuzzleParser::getFileName() const
{
    return _fileName;
}

// Setters

void PuzzleParser::setLineLength(int lineLength)
{
    this->lineLength = lineLength;
}