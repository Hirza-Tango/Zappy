/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   board.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 15:54:31 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/13 18:39:53 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

void	gen_board(t_game_state *s)
{
	size_t	i;
	size_t	j;

	((t_resource *)g_resources)[0].team_requirement = s->size_x * s->size_y
		/ LIFE_DURATION * TEAM_MAX_SIZE * FOOD_MULTIPLIER;
	s->board = (t_list ***)malloc(sizeof(t_list **) * s->size_x);
	i = 0;
	while (i < s->size_x)
	{
		s->board[i] = (t_list **)malloc(sizeof(t_list *) * s->size_y);
		bzero(s->board[i++], sizeof(t_list *) * s->size_y);
	}
	i = 0;
	while (i < NUM_RESOURCES)
	{
		j = 0;
		while (j < g_resources[i].team_requirement * RESOURCE_MULTIPLIER)
		{
			ft_lstadd(&s->board[RAND(s->size_x)][RAND(s->size_y)],
				ft_lstnew(&i, sizeof(i)));
			j++;
		}
		i++;
	}
}
