/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 11:45:11 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/12 18:06:14 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

static void	check_player(t_state *s, t_player *player, int fd, struct timespec *t)
{
	if (player->resolution_time.tv_sec &&
		compare_time(&player->resolution_time, t) < 0)
	{
		player->next_action(s, fd, player->option);
		player->option = NULL;
		player->next_action = NULL;
		bzero(&player->resolution_time, sizeof(struct timespec));
	}
}

static void check_food(t_state *s, t_player *player, int fd, struct timespec *t)
{
	if (player->death_time.tv_sec &&
		compare_time(&player->death_time, t) < 0)
	{
		if (player->inventory[FOOD])
		{
			add_time(&player->death_time, LIFE_DURATION / s->time);
			player->inventory[FOOD]--;
		}
		else
			player_death(s, fd);
	}
}

//TODO: shorten
static void	check_eggs(t_state *s, struct timespec *t)
{
	size_t 	i;
	t_list	*egg_list;
	t_egg	*egg;

	i = -1UL;
	while (++i < s->n_teams)
	{
		egg_list = s->teams[i].eggs;
		while (egg_list)
		{
			egg = egg_list->content;
			if (egg->hatch_time.tv_sec && egg->hatch_time.tv_nsec &&
				compare_time(&egg->hatch_time, t) < 0)
			{
				bzero(&egg->hatch_time, sizeof(egg));
				monitor_eht(s, egg);
				s->teams[i].nb_client++;
			}
			if (compare_time(&egg->death_time, t) < 0)
			{
				s->teams[i].eggs = egg_list->next;
				monitor_edi(s, egg);
				free(egg_list->content);
				free(egg_list);
				egg_list = s->teams[i].eggs;
				continue ;
			}
			egg_list = egg_list->next;
		}
	}
}

void		execute(t_state *s)
{
	int				i;
	struct timespec	t;
	t_player		*player;

	i = -1;
	clock_gettime(CLOCK_MONOTONIC, &t);
	while (++i <= s->max_fd)
	{
		if (s->clients[i].type != PLAYER)
			continue ;
		player = s->clients[i].player;
		check_player(s, player, i, &t);
		check_food(s, player, i, &t);
	}
	check_eggs(s, &t);
}
