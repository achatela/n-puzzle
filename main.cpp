#include "PuzzleParser.hpp"

int main(int argc, char **argv)
{
    try
    {
        PuzzleParser parser(argv[1]);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}