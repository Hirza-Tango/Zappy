/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 15:54:31 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/26 13:27:16 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

long	player_compare(const void *a, const void *b)
{
	if (((t_player *)a)->x != ((t_player *)b)->x)
		return ((t_player *)a)->x - ((t_player *)b)->x;
	return (((t_player *)a)->y - ((t_player *)b)->y);
}

char	*look(t_state *s, unsigned x, unsigned y)
{
	//TODO: adjust for new resources
	return (NULL);
}

void	gen_board(t_state *s)
{
	size_t		i;
	size_t		j;

	s->board = (unsigned int ***)malloc(s->size_x * sizeof (unsigned int **));
	i = -1;
	while (++i < s->size_x)
	{
		s->board[i] = (unsigned int **)malloc(s->size_y * sizeof (unsigned int *));
		j = -1;
		while (++j < s->size_y)
		{
			s->board[i][j] = (unsigned int *)malloc(NUM_RESOURCES * sizeof(unsigned int));
			bzero(s->board[i][j], NUM_RESOURCES * sizeof(unsigned int));
		}
	}
	((t_resource_entry *)g_resources_info)[0].team_requirement =
		(s->size_x * s->size_y) / LIFE_DURATION * TEAM_MAX_SIZE * FOOD_MULTIPLIER;
	//TODO: fix lack of food
	i = -1;
	while (++i < NUM_RESOURCES)
	{
		j = -1;
		while (++j < g_resources_info[i].team_requirement * RESOURCE_MULTIPLIER * s->n_teams)
			(s->board[RAND(s->size_x)][RAND(s->size_y)][i])++;
	}
}
