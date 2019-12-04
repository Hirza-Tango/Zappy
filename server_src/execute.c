/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 11:45:11 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/04 13:22:45 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

static void	check_players(t_state *s)
{
	int				i;
	struct timespec	t;
	t_player		*player;

	i = -1;
	clock_gettime(CLOCK_MONOTONIC, &t);
	while (++i < s->max_fd)
	{
		if (s->clients[i].type != PLAYER)
			continue ;
		player = s->clients[i].player;
		if (player->resolution_time.tv_sec <= t.tv_sec &&
			player->resolution_time.tv_nsec <= t.tv_nsec)
		{
			player->next_action(s, i, player->option);
			player->option = NULL;
			player->next_action = NULL;
			bzero(&player->resolution_time, sizeof(struct timespec));
		}
	}
}

static void check_food(t_state *s)
{
	int				i;
	struct timespec	t;
	t_player		*player;

	i = -1;
	clock_gettime(CLOCK_MONOTONIC, &t);
	while (++i < s->max_fd)
	{
		if (s->clients[i].type != PLAYER)
			continue ;
		player = s->clients[i].player;
		if (player->death_time.tv_sec <= t.tv_sec &&
			player->death_time.tv_nsec <= t.tv_nsec)
		{
			if (player->inventory[FOOD])
			{
				add_time(&player->death_time, LIFE_DURATION / s->time);
				player->inventory[FOOD]--;
			}
			else
				player_death(s, i);
		}
	}
}

void		execute(t_state *s)
{
	check_players(s);
	//TODO: check all eggs for hatching
	check_food(s);
}
