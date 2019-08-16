#ifndef ZAPPY_SERVER_H
# define ZAPPY_SERVER_H

#include <stdlib.h>

/*
**	1 is north, increasing clockwise
*/

typedef struct	s_player
{
	unsigned int	x;
	unsigned int	y;
	unsigned char	direction;
}				t_player;

typedef struct	s_gamestate
{
	char			**board;
	unsigned int	size_x;
	unsigned int	size_y;
	unsigned char	max_players;
	unsigned char	current_players;
}				t_gamestate;


#endif
