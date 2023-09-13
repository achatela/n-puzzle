#include "PuzzleParser.hpp"

int main(int argc, char **argv)
{
    try
    {
        PuzzleParser parser(argv[1]);

        std::cout << parser.getFileName() << std::endl;
        std::cout << parser.getLineLength() << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    // std::cout << std::endl;
    return 0;
}