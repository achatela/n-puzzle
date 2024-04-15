# N-Puzzle Solver

## Project Overview
This repository contains a C++ implementation of the classic N-puzzle game solver using A* search algorithm and its variants. The N-puzzle game involves a board with N*N cells, one of which is empty, allowing the movement of adjacent numbered tiles into the empty space. The objective is to transition from a given random initial state to a goal state in a sequence that minimizes the number of moves. The goal state is defined as a "snail solution" where the numbers are arranged in a spiral pattern.

## Features
- **Multiple Puzzle Sizes**: Supports solving N-puzzles of various sizes (e.g., 3x3, 4x4, 5x5, up to 17x17).
- **Different Starting States**: Can handle both randomly generated puzzle configurations and predefined states loaded from input files.
- **Search Algorithms**: Implements the A* search algorithm with options to switch to Uniform Cost Search and Greedy Search for comparative performance analysis.
- **Heuristics**: Offers at least three heuristic functions for A*:
  - Manhattan Distance (mandatory)
  - Linear Conflict
  - Euclidean Distance
  - Uniform Cost Search (no heuristic)
- **Performance Metrics**: Provides detailed metrics, including:
  - Total number of states selected (time complexity)
  - Maximum number of states held in memory simultaneously (space complexity)
  - Number of moves to solve the puzzle
  - Ordered sequence of states from the initial to the final state

## Unsolvability Detection
The program can determine if a puzzle is unsolvable and will notify the user, terminating the process accordingly.

## Build and Execution
Make sure to use the provided Makefile for compiling the project with the following commands:
```bash
make        # Compiles the project
make clean  # Removes compiled files
```


## Usage
To run the program, use the following command:
```bash
./n-puzzle # Will generate a random 3x3 puzzle and solve it using A* with Manhattan Distance heuristic if possible
```

To specify the puzzle size and the search algorithm, use the following command:
```bash
python3 generate.py -s 3 > puzzle.txt # 3x3 puzzle
./n-puzzle puzzle.txt <linear|manhattan|euclidean|bonus_uniform> <astar|greedy> # Default is manhattan and astar
```