/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 15:25:47 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/03 16:38:10 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

static void		see_square(t_state *s, int fd, int x, int y)
{
	unsigned int	i;
	int				j;
	char			started;

	i = -1;
	started = 0;
	while (++i < NUM_RESOURCES)
	{
		j = -1;
		while (++j < s->board[x][y][i])
		{
			started ? send(fd, " ", 1, 0) : (started = 1);
			send(fd, g_resources_info[i].name, strlen(g_resources_info[i].name),
				0);
		}
	}
	i = -1;
	while (++i < s->max_fd)
	{
		if (s->clients[i].type == PLAYER && i != fd)
		{
			started ? send(fd, " ", 1, 0) : (started = 1);
			send(fd, "player", 6, 0);
		}
	}
}

void			player_see(t_state *s, int fd, void *unused)
{
	int			i;
	int			j;
	t_player	*player;

	(void)unused;
	send(fd, "{", 1, 0);
	player = s->clients[fd].player;
	i = -1;
	while (++i <= player->level)
	{
		j = player->x - i - 1;
		while (++j <= player->x + i)
		{
			if (player->direction == 0)
				see_square(s, fd, j % s->size_x,(player->y - i) % s->size_y);
			else if (player->direction == 1)
				see_square(s, fd, j % s->size_y,(player->x + i) % s->size_x);
			else if (player->direction == 2)
				see_square(s, fd, j % s->size_x,(player->y + i) % s->size_y);
			else
				see_square(s, fd, j % s->size_y,(player->x - i) % s->size_x);
		}
		send(fd, ", ", 1, 0);
	}
	send(fd, "}\n", 2, 0);
}

void		player_inventory(t_state *s, int fd, void *unused)
{
	char			buff[4096];
	unsigned int	*inv;

	inv = s->clients[fd].player->inventory;
	snprintf(buff, 4096, "{food %u, linemate %u, deraumere %u, sibur %u, "
		"mendiane %u, phiras %u, thystamine %u}\n", inv[0], inv[1], inv[2],
		inv[3], inv[4], inv[5], inv[6]);
	send(fd, buff, strlen(buff), 0);
}

void		player_take(t_state *s, int fd, void *num)
{
	t_player	*player;

	player = s->clients[fd].player;
	if (!s->board[player->x][player->y][atoi(num)])
		return ((void)send(fd, "ko\n", 3, 0));
	s->board[player->x][player->y][atoi(num)]--;
	player->inventory[atoi(num)]++;
	send(fd, "ok\n", 3, 0);
	monitor_pgt(s, player, atoi(num));
	monitor_pin(s, -1, player->player_no);
	monitor_bct(s, -1, player->x, player->y);
}

void		player_put(t_state *s, int fd, void *num)
{
	t_player	*player;

	player = s->clients[fd].player;
	if (!player->inventory[atoi(num)])
		return ((void)send(fd, "ko\n", 3, 0));
	s->board[player->x][player->y][atoi(num)]++;
	player->inventory[atoi(num)]--;
	send(fd, "ok\n", 3, 0);
	monitor_pdr(s, player, atoi(num));
	monitor_pin(s, -1, player->player_no);
	monitor_bct(s, -1, player->x, player->y);
}
