#include "puzzle.h"

void	serchEmptyTile(puzzle *puz, int *zp)
{
	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			if (puz->field[i][j] == 0)
			{
				zp[0] = i, zp[1] = j;
				return ;
			}

}

int 	*tilePosition(puzzle *puz, int i, int j)
{
	double			 tmp;
	static int		tilePosition[2];


	tmp = (double)puz->field[i][j] / 4;
	if (!(tmp - (int)tmp))
	{
		tilePosition[0] = (int)tmp - 1;
		tilePosition[1] = 3;
	}
	else
	{
		tilePosition[0] = (int)tmp;
		tmp -= tilePosition[0];
		tilePosition[1] = tmp * 4 - 1;
	}
	return tilePosition;
}

int     linearConflict(puzzle *puz)
{
	int lc = 0;


	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			for (int k = j + 1; k < 4; k++)
				if (tilePosition(puz, i, j)[0] == tilePosition(puz, i, k)[0])
					if (puz->field[i][j] > puz->field[i][k])
						lc += 2;
	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
			for (int k = i + 1; k < 4; k++)
				if (puz->field[k][j] &&
					tilePosition(puz, i, j)[1] == tilePosition(puz, k, j)[1])
					if (puz->field[i][j] > puz->field[k][j])
						lc += 2;
	return lc;
}

int		stepsToPoint(int i, int j, int tilePosition[])
{
	int ways = 0;

	ways += _ABS(tilePosition[0] - i);
	ways += _ABS(tilePosition[1] - j);
	//(ways) ? ways += 1 : 0;
	return ways;
}

int		calcHeuristic(puzzle *puz) /* Manhattan Distance */
{
	int		tilePosition[2]; /* x, y */
	int		heuristic = 0;
	double	tmp;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
		{
			tmp = (double)puz->field[i][j] / 4;
			if (!(tmp - (int)tmp))
			{
				tilePosition[0] = (int)tmp - 1;
				tilePosition[1] = 3;
			}
			else
			{
				tilePosition[0] = (int)tmp;
				tmp -= tilePosition[0];
				tilePosition[1] = tmp * 4 - 1;
			}
			if (puz->field[i][j]) /* if not empty tile */
				heuristic += stepsToPoint(i, j, tilePosition);
		}
	heuristic += linearConflict(puz);
	return heuristic;
}

unsigned int	fieldHash(puzzle *puz)
{
	unsigned int hash = 7;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			hash = hash * 17 + puz->field[i][j];
	return hash;
}

coList *checkInList(coList *open, UI hash)
{
	coList *tmp;

	tmp = open;
	while (tmp)
	{
		if (tmp->hash == hash)
			return tmp;
		tmp = tmp->next;
	}
	return NULL;
}

coList *getSmallHeuristic(coList *list)
{
	coList *tmp;
	coList *small = NULL;

	tmp = list;
	small = tmp;
	while (tmp)
	{
		if (small->heuristic > tmp->heuristic)
			small = tmp;
		tmp = tmp->next;
	}
	return small;
}


void	addToOpen(coList **open, coList *close, puzzle *puz, coList *parent)
{
	coList *tmp;
	UI		hash = fieldHash(puz);
	int		hr = calcHeuristic(puz);

	if (checkInList(close, hash))
		return ;
	if (!(tmp = checkInList(*open, hash)))
	{
		if (!(tmp = (coList*)malloc(sizeof(coList))))
		{
			printf("ERORR: Memory allocation error.\n");
			exit(0);
		}
		for (int i = 0; i < 4; i++)
			memcpy(tmp->field[i], puz->field[i], 4 * sizeof(int));
		tmp->hash = hash;
		if (parent)
			tmp->move_nb = parent->move_nb + 1;
		else
			tmp->move_nb = 0;
		tmp->heuristic = hr + tmp->move_nb;
		tmp->next = (*open);
		tmp->parent = parent;
		(*open) = tmp;
	}
	else
	{
		if (tmp->heuristic > hr)
		{
			tmp->heuristic = hr;
			tmp->parent = parent;
		}
	}
}

/* add to close and delete from open */
void	addToClose(coList **open, coList **close, coList *parent)
{
	coList *tmp;
	coList *start;

	tmp = (*open);
	start = (*open);
	if (tmp->hash == parent->hash)
	{
		(*open) = (*open)->next;
		tmp->next = (*close);
		(*close) = tmp;
	}
	else
	{
		while ((*open)->next)
		{
			if ((*open)->next->hash == parent->hash)
			{
				tmp = (*open)->next;
				(*open)->next = (*open)->next->next;
				tmp->next = (*close);
				(*close) = tmp;
				break ;
			}
			(*open) = (*open)->next;
		}
		(*open) = start;
	}
}


void	puzzleSolver(puzzle *puz)
{
	int zp[2];	/* zero (empty) position */
	coList *parent;
	coList *close = NULL;
	coList *open = NULL;


int steps = 1;
	serchEmptyTile(puz, zp);
	addToOpen(&open, close, puz, NULL);
	parent = getSmallHeuristic(open);

/*********************************** LOOP ***********************************/
	while (open && parent->hash != 3911222783)
	{

		for (int i = 0; i < 4; i++)
			memcpy(puz->field[i], parent->field[i], 4 * sizeof(int));
		serchEmptyTile(puz, zp);

		if ((zp[1] + 1) <= 3)
		{
			puz->field[zp[0]][zp[1]] = puz->field[zp[0]][zp[1] + 1];
			puz->field[zp[0]][zp[1] + 1] = 0;
			addToOpen(&open, close, puz, parent);
			puz->field[zp[0]][zp[1] + 1] = puz->field[zp[0]][zp[1]];
			puz->field[zp[0]][zp[1]] = 0;
		}

		if ((zp[1] - 1) >= 0)
		{
			puz->field[zp[0]][zp[1]] = puz->field[zp[0]][zp[1] - 1];
			puz->field[zp[0]][zp[1] - 1] = 0;
			addToOpen(&open, close, puz, parent);
			puz->field[zp[0]][zp[1] - 1] = puz->field[zp[0]][zp[1]];
			puz->field[zp[0]][zp[1]] = 0;
		}

		if ((zp[0] + 1) <= 3)
		{
			puz->field[zp[0]][zp[1]] = puz->field[zp[0] + 1][zp[1]];
			puz->field[zp[0] + 1][zp[1]] = 0;
			addToOpen(&open, close, puz, parent);
			puz->field[zp[0] + 1][zp[1]] = puz->field[zp[0]][zp[1]];
			puz->field[zp[0]][zp[1]] = 0;
		}

		if ((zp[0] - 1) >= 0)
		{
			puz->field[zp[0]][zp[1]] = puz->field[zp[0] - 1][zp[1]];
			puz->field[zp[0] - 1][zp[1]] = 0;
			addToOpen(&open, close, puz, parent);
			puz->field[zp[0] - 1][zp[1]] = puz->field[zp[0]][zp[1]];
			puz->field[zp[0]][zp[1]] = 0;
		}

		addToClose(&open, &close, parent);

		parent = getSmallHeuristic(open);

	}
	steps = 0;
	while (parent)
	{
		for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
					printf("%d\t", parent->field[i][j]);
				printf("\n\n");
			}
			printf("heuristic = %d\n", parent->heuristic);
			printf("==================== %d\n", steps++);
		parent = parent->parent;
	}

}

int		main(int ac, char **av)
{

	puzzle puz = {.field = {
				{1, 3, 7, 4},
				{5, 2, 10, 8},
				{9, 6, 0, 12},
				{13, 14, 11, 15}
	}};

	// puzzle puz = {.field = {
	// {14, 10, 3, 4},
	// {1, 2, 0, 6},
	// {9, 15, 5, 12},
	// {7, 8, 13, 11},
	// }};

	puzzleSolver(&puz);

	return 0;
}
