/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 15:27:24 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/04 13:21:05 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

void		player_incantation_start(t_state *s, int fd)
{
	set_action(s->clients[fd].player, player_incantation_end, 300 / s->time, 0);
	//TODO:this
	monitor_pic(s, s->clients[fd].player);
}

void		player_incantation_end(t_state *s, int fd, void *unused)
{
	t_player	*player;
	int			result;
	int			i;
	int			j;

	result = 0;
	player = s->clients[fd].player;
	//TODO:this
	monitor_pie(s, player->x, player->y, result);
	if (!result)
		return ;
	i = -1;
	while (++i < s->max_fd)
	{
		if (s->clients[i].type != PLAYER || s->clients[i].player->x != player->x
			|| s->clients[i].player->x != player->y)
			continue ;
		monitor_plv(s, -1, s->clients[i].player->player_no);
	}
	monitor_bct(s, -1, player->x, player->y);
}

static void	get_kicked(t_state *s, char direction, int fd)
{
	t_player	*player;

	player = s->clients[fd].player;
	if (direction == 0)
		player->y = player->y - 1 % s->size_y;
	else if (direction == 1)
		player->x = player->x + 1 % s->size_x;
	else if (direction == 2)
		player->y = player->y + 1 % s->size_y;
	else
		player->x = player->x - 1 % s->size_x;
	send(fd, "moving ", 7, 0);
	direction += '0';
	send(fd, &direction, 1, 0);
	send(fd, "\n", 1, 0);
	monitor_ppo(s, -1, player->player_no);
}

void		player_kick(t_state *s, int fd, void *unused)
{
	t_player	*player;
	int			i;

	player = s->clients[fd].player;
	i = -1;
	while (++i < s->max_fd)
		if (s->clients[i].type == PLAYER &&
			s->clients[i].player->next_action == player_incantation_end)
			return ((void)send(fd, "ko\n", 3, 0));
	monitor_pex(s, player);
	i = -1;
	while (++i < s->max_fd)
		if (s->clients[i].type == PLAYER && s->clients[i].player->x
			== player->x && s->clients[i].player->y == player->y)
			get_kicked(s, player->direction, i);
	send(fd, "ok\n", 3, 0);
}

void		player_death(t_state *s, int fd)
{
	s->teams[s->clients[fd].player->team_no].nb_client++;
	s->n_players--;
	s->clients[fd].type = UNKNOWN;
	send(fd, "death\n", 6, 0);
	monitor_pdi(s, s->clients[fd].player);
	free(s->clients[fd].player);
	s->clients[fd].player = NULL;
	//TODO: drop stuff?
}
