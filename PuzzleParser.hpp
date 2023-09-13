#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <stdexcept>
#include <string.h>
#include <set>
#include <sstream>

class PuzzleParser
{

private:
    std::string _fileName;
    int _lineLength;

public:
    // Constructor and destructor
    PuzzleParser(std::string fileName);
    ~PuzzleParser();

    // Parsing methods
    bool isCommentLine(std::string line);
    bool parseLineLength(std::string line);
    void parse();
    void checkArgumentsValidity(std::vector<std::string> &tokens);

    // Getters
    int getLineLength() const;
    std::string getFileName() const;

    // Setters
    void setLineLength(int lineLength);
};