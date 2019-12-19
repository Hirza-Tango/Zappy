/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 15:27:24 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/19 17:45:55 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

void		player_incantation_start(t_state *s, int fd)
{
	int	i;

	set_action(s->clients[fd].player, player_incantation_end, 300.0 / s->time, 0);
	i = 0;
	while (i < s->max_fd)
	{
		if (s->clients[i].type == PLAYER &&
			s->clients[i].player->x == s->clients[fd].player->x &&
			s->clients[i].player->y == s->clients[fd].player->y &&
			i != fd)
			add_time(&s->clients[i].player->resolution_time, 300.0 / s->time);
	}
	monitor_pic(s, s->clients[fd].player);
}

static int	check_result(t_state *s, unsigned x, unsigned y, unsigned level)
{
	int i;

	i = 0;
	while (++i < NUM_RESOURCES)
		if (s->board[x][y][i] < g_resources_info[i].level_requirements[level])
			return (0);
	i = -1;
	while (++i < s->max_fd)
		if (s->clients[i].type == PLAYER && s->clients[i].player->x == x &&
			s->clients[i].player->y == y &&
			s->clients[i].player->level != level)
			return (0);
	i = -1;
	while (++i < s->max_fd)
		if (s->clients[i].type == PLAYER && s->clients[i].player->x == x &&
			s->clients[i].player->y == y)
		{
			s->clients[i].player->level++;
			monitor_plv(s, -1, s->clients[i].player->player_no);
		}
	i = 0;
	while (++i < NUM_RESOURCES)
		s->board[x][y][i] -= g_resources_info[i].level_requirements[level];
	return (1);
}

void		player_incantation_end(t_state *s, int fd, void *unused)
{
	t_player	*player;
	int			result;

	result = 0;
	player = s->clients[fd].player;
	(void)unused;
	result = check_result(s, player->x, player->y, player->level);
	monitor_pie(s, player->x, player->y, result);
	if (!result)
		return ;
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

	(void)unused;
	player = s->clients[fd].player;
	i = -1;
	while (++i <= s->max_fd)
		if (s->clients[i].type == PLAYER &&
			s->clients[i].player->next_action == player_incantation_end)
			return ((void)send(fd, "ko\n", 3, 0));
	monitor_pex(s, player);
	i = -1;
	while (++i <= s->max_fd)
		if (s->clients[i].type == PLAYER && s->clients[i].player->x
			== player->x && s->clients[i].player->y == player->y)
			get_kicked(s, player->direction, i);
	send(fd, "ok\n", 3, 0);
}
