#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string.h>

class PuzzleParser
{

private:
    std::string _fileName;
    int lineLength;

public:
    // Constructor and destructor
    PuzzleParser(std::string fileName);
    ~PuzzleParser();

    // Parsing methods
    bool isCommentLine(std::string line);
    bool parseLineLength(std::string line);
    void parse();

    // Getters
    int getLineLength() const;
    std::string getFileName() const;

    // Setters
    void setLineLength(int lineLength);
};