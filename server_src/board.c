/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 15:54:31 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/09 11:56:48 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

static unsigned int	***allocate_board(size_t x, size_t y)
{
	size_t			i;
	size_t			j;
	unsigned int	***board;

	board = (unsigned int ***)malloc(x * sizeof (unsigned int **));
	i = -1;
	while (++i < x)
	{
		board[i] = (unsigned int **)malloc(y * sizeof (unsigned int *));
		j = -1;
		while (++j < y)
		{
			board[i][j] = (unsigned int *)malloc(
				NUM_RESOURCES * sizeof(unsigned int));
			bzero(board[i][j], NUM_RESOURCES * sizeof(unsigned int));
		}
	}
	return board;
}

void				gen_board(t_state *s)
{
	size_t	i;
	size_t	j;
	size_t	num;

	s->board = allocate_board(s->size_x, s->size_y);
	i = -1;
	while (++i < NUM_RESOURCES)
	{
		j = -1;
		if (!i)
			num = (s->size_x * s->size_y) /
				LIFE_DURATION * TEAM_MAX_SIZE * FOOD_MULTIPLIER;
		else
			num = g_resources_info[i].team_requirement *
				RESOURCE_MULTIPLIER * s->n_teams;
		while (++j < num)
			(s->board[RAND(s->size_x)][RAND(s->size_y)][i])++;
	}
}
