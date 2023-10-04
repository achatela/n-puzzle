CC = c++
CFLAGS = -O3 -Wall -Wextra -Werror -std=c++11
INCLUDES = -I./includes
SRCS = ./srcs/main.cpp ./srcs/PuzzleParser.cpp ./srcs/PuzzleSolver.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = n-puzzle
HEADERS = ./includes/PuzzleParser.hpp ./includes/PuzzleSolver.hpp

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

all: $(OBJS) $(TARGET)

re: fclean all

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(TARGET) *.txt
