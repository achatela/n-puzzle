#include "PuzzleParser.hpp"
#include "PuzzleSolver.hpp"
// #include <bits/stdc++.h>

const int ch_MAX = 62;

std::string RandomString(int ch)
{
    char alpha[ch_MAX] = {'a', 'b', 'c', 'd', 'e', 'f', 'g',
                          'h', 'i', 'j', 'k', 'l', 'm', 'n',
                          'o', 'p', 'q', 'r', 's', 't', 'u',
                          'v', 'w', 'x', 'y', 'z',
                          'A', 'B', 'C', 'D', 'E', 'F', 'G',
                          'H', 'I', 'J', 'K', 'L', 'M', 'N',
                          'O', 'P', 'Q', 'R', 'S', 'T', 'U',
                          'V', 'W', 'X', 'Y', 'Z',
                          '1', '2', '3', '4', '5', '6', '7',
                          '8', '9', '0'};
    std::string result = "";
    for (int i = 0; i < ch; i++)
        result = result + alpha[rand() % ch_MAX];

    return result;
}

std::string generatePuzzle()
{
    std::string fileName = RandomString(24) + ".txt";
    std::ofstream file(fileName);
    std::vector<int> vec = {1, 2, 3, 4, 5, 6, 7, 8, 0};
    file << '3' << '\n'
         << std::endl;
    for (int i = 0; i < 3; i++)
    {
        int j = 0;
        for (; j < 3; j++)
        {
            int index = rand() % vec.size();
            if (j == 2)
                file << vec[index] << std::endl;
            else
                file << vec[index] << " ";
            vec.erase(vec.begin() + index);
        }
        file << std::endl;
    }

    return fileName;
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    try
    {
        if (argc != 3 && argc != 1)
            throw std::invalid_argument("Usage: ./n-puzzle <file>.txt <heuristic> (manhattan|linear)");
        else if (argc == 1)
        {
            std::string fileName = generatePuzzle();

            PuzzleParser parser(fileName);

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

            PuzzleSolver solver(parser.getPuzzle(), parser.getLineLength(), std::string("manhattan"), std::string("a*"));
        }
        else
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
            std::string algorithm;
            if (argc == 4)
                algorithm = std::string(argv[3]);
            else
                algorithm = std::string("a*");

            PuzzleSolver solver(parser.getPuzzle(), parser.getLineLength(), std::string(argv[2]), algorithm);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    // std::cout << std::endl;
    return 0;
}