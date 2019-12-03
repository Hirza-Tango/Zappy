/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/03 10:52:30 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/03 14:53:25 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

t_player	*new_player(t_state *s, int fd, char *buff, t_egg *egg)
{
	t_player	*player;
	int			i;

	player = (t_player *)malloc(sizeof(t_player));
	bzero(&player, sizeof(player));
	player->player_no = s->n_players++;
	i = 0;
	while (strcmp(s->teams[i].name, buff))
		i++;
	s->teams[i].nb_client--;
	player->team_no = i;
	player->direction = RAND(4);
	player->level = 1;
	player->x = egg ? egg->x : RAND(s->size_x);
	player->y = egg ? egg->y : RAND(s->size_y);
	return (player);
}

/*
**	Directions:
**	0		1		2		3
**	North	East	South	West
*/

void		player_advance(t_state *s, int fd, void *unused)
{
	t_player *player;

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
}

void		player_left(t_state *s, int fd, void *unused)
{
	(void)unused;
	s->clients[fd].player->direction =
		(s->clients[fd].player->direction - 1) % 4;
	send(fd, "ok\n", 3, 0);
}

void		player_right(t_state *s, int fd, void *unused)
{
	(void)unused;
	s->clients[fd].player->direction =
		(s->clients[fd].player->direction + 1) % 4;
	send(fd, "ok\n", 3, 0);
}

void		see_square(t_state *s, int fd, int x, int y)
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

void		player_see(t_state *s, int fd, void *unused)
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
}

void		player_incantation_start(t_state *s, int fd, void *unused)
{
	//TODO:this
}

void		player_incantation_end(t_state *s, int fd, void *unused)
{
	//TODO:this
}

void		get_kicked(t_state *s, char direction, int fd)
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
	i = -1;
	while (++i < s->max_fd)
		if (s->clients[i].type == PLAYER && s->clients[i].player->x
			== player->x && s->clients[i].player->y == player->y)
			get_kicked(s, player->direction, i);
	send(fd, "ok\n", 3, 0);
}

void		player_broadcast(t_state *s, int fd, void *message)
{
	t_player	*player;
	char		buff[STRBUFF_SIZE];
	int			i;
	char		direction;

	player = s->clients[fd].player;
	i = -1;
	while (++i)
		if (s->clients[i].type == PLAYER)
		{
			send(i, "message ", 8, 0);
			direction = sound_direction(player, s->clients[i].player, s) + '0';
			send(i, &direction, 1, 0);
			send(i, ",", 1, 0);
			send(i, message, strlen(message), 0);
			send(i, "\n", 1, 0);
		}
	send(fd, "ok\n", 3, 0);
}

void		player_fork(t_state *s, int fd, void *unused)
{
	//TODO:this
}

void		player_connect_nbr(t_state *s, int fd, void *unused)
{
	char	buff[STRBUFF_SIZE];

	(void)unused;
	snprintf(buff, STRBUFF_SIZE, "%u\n",
		s->teams[s->clients[fd].player->team_no]);
	send(fd, buff, strlen(buff), 0);
}
