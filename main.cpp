#include "PuzzleParser.hpp"
#include "PuzzleSolver.hpp"

int main(int argc, char **argv)
{
    try
    {
        PuzzleParser parser(argv[1]);

        std::cout << parser.getFileName() << std::endl;
        std::cout << parser.getLineLength() << std::endl;

        std::vector<std::vector<int>> puzzle = parser.getPuzzle();
        std::cout << std::endl;
        for (int i = 0; i < puzzle.size(); i++)
        {
            for (int j = 0; j < puzzle[i].size(); j++)
            {
                if (j == puzzle[i].size() - 1)
                    std::cout << puzzle[i][j];
                else
                    std::cout << puzzle[i][j] << " ";
            }
            std::cout << std::endl;
        }

        PuzzleSolver solver(parser.getPuzzle(), parser.getLineLength());
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    // std::cout << std::endl;
    return 0;
}