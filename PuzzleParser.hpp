#include <string>
#include <fstream>
#include <iostream>
#include <stdexcept>

class PuzzleParser
{

private:
    std::string _fileName;
    int lineLength;

public:
    PuzzleParser(std::string fileName);
    ~PuzzleParser();

    bool isCommentLine(std::string line);
    void parse();
};