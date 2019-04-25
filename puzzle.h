#ifndef PUZZLE_H
#define PUZZLE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define _ABS(var)  ((var) < 0) ? -(var) : (var)

typedef unsigned int UI;

typedef struct	puzzle	puzzle;
typedef struct	coList	coList;

struct		puzzle
{
	int		field[4][4];
};

struct		coList
{
	int					field[4][4];
	int					heuristic;
	int					move_nb;
	unsigned int		hash;
	coList			*parent;
	coList 			*next;
};

#endif
