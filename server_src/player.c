/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 10:52:30 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/12 16:59:11 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

t_player	*new_player(t_state *s, char *buff, t_egg *egg)
{
	t_player		*player;
	int				i;

	player = (t_player *)malloc(sizeof(t_player));
	bzero(player, sizeof(t_player));
	player->player_no = egg ? egg->player_no : s->n_players++;
	i = 0;
	while (strcmp(s->teams[i].name, buff))
		i++;
	s->teams[i].nb_client--;
	player->team_no = i;
	player->direction = RAND(4);
	player->level = 1;
	player->x = egg ? egg->x : RAND(s->size_x);
	player->y = egg ? egg->y : RAND(s->size_y);
	egg ? (void)(player->death_time = egg->death_time) :
		clock_gettime(CLOCK_MONOTONIC, &player->death_time);
	egg ? monitor_ebo(s, egg->egg_no) :
		add_time(&player->death_time, (LIFE_DURATION * 10)/s->time);
	monitor_pnw(s, -1, player);
	return (player);
}


void	set_action(t_player *player, void (f)(t_state *, int, void *),
	double t, void *option)
{
	player->next_action = f;
	clock_gettime(CLOCK_MONOTONIC, &player->resolution_time);
	add_time(&player->resolution_time, t);
	player->option = option;
}

/*
**	Directions:
**	0		1		2		3
**	North	East	South	West
*/

void		player_advance(t_state *s, int fd, void *unused)
{
	t_player	*player;

	(void)unused;
	player = s->clients[fd].player;
	if (player->direction == 0)
		player->y = (player->y - 1) % s->size_y;
	else if (player->direction == 1)
		player->x = (player->x + 1) % s->size_x;
	else if (player->direction == 2)
		player->y = (player->y + 1) % s->size_y;
	else if (player->direction == 3)
		player->x = (player->x - 1) % s->size_x;
	send(fd, "ok\n", 3, 0);
	monitor_ppo(s, -1, player->player_no);
}

void		player_left(t_state *s, int fd, void *unused)
{
	(void)unused;
	s->clients[fd].player->direction =
		(s->clients[fd].player->direction + 3) % 4;
	send(fd, "ok\n", 3, 0);
	monitor_ppo(s, -1, s->clients[fd].player->player_no);
}

void		player_right(t_state *s, int fd, void *unused)
{
	(void)unused;
	s->clients[fd].player->direction =
		(s->clients[fd].player->direction + 1) % 4;
	send(fd, "ok\n", 3, 0);
	monitor_ppo(s, -1, s->clients[fd].player->player_no);
}
