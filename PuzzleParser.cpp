#include "PuzzleParser.hpp"

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
    }
}