/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 15:32:24 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/03 10:26:01 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"


void	monitor_pnw(t_state *s, int fd, t_player *player)
{
	char		buff[256];

	snprintf(buff, 256, "pnw #%zu %u %u %u %u %s\n", player->player_no,
		player->x, player->y, player->direction, player->level,
		s->teams[player->team_no].name);
	if (fd < 0)
		send_all_monitors(s, buff);
	else
		send(fd, buff, strlen(buff), 0);
}

void	monitor_enw(t_state *s, int fd, t_egg *egg)
{
	char	buff[256];

	snprintf(buff, 256, "enw #%u #%zu %u %u\n", egg->egg_no, egg->player_no,
		egg->x, egg->y);
	if (fd < 0)
		send_all_monitors(s, buff);
	else
		send(fd, buff, strlen(buff), 0);
}

void	monitor_ppo(t_state *s, int fd, unsigned int player_no)
{
	char		buff[256];
	t_player	*player;
	size_t		i;

	i = -1;
	while (++i < s->max_clients)
	{
		if (s->clients[i].type == PLAYER &&
			s->clients[i].player->player_no == player_no)
		{
			player = s->clients[i].player;
			break ;
		}
	}
	if (i == s->max_clients)
	{
		send(fd, "sbp\n", 4, 0);
		return ;
	}
	snprintf(buff, 256, "ppo #%u %u %u %u\n", player_no, player->x, player->y,
		player->direction);
	send(fd, buff, strlen(buff), 0);
}

void	monitor_plv(t_state *s, int fd, unsigned int player_no)
{
	char		buff[256];
	t_player	*player;
	size_t		i;

	i = -1;
	while (++i < s->max_clients)
	{
		if (s->clients[i].type == PLAYER &&
			s->clients[i].player->player_no == player_no)
		{
			player = s->clients[i].player;
			break ;
		}
	}
	if (i == s->max_clients)
	{
		send(fd, "sbp\n", 4, 0);
		return ;
	}
	snprintf(buff, 256, "plv #%u %u\n", player_no, player->level);
	send(fd, buff, strlen(buff), 0);
}

void	monitor_pin(t_state *s, int fd, unsigned int player_no)
{
	char		buff[256];
	t_player	*player;
	size_t		i;

	i = -1;
	while (++i < s->max_clients)
	{
		if (s->clients[i].type == PLAYER &&
			s->clients[i].player->player_no == player_no)
		{
			player = s->clients[i].player;
			break ;
		}
	}
	if (i == s->max_clients)
	{
		send(fd, "sbp\n", 4, 0);
		return ;
	}
	snprintf(buff, 256, "pin #%u %u %u %u %u %u %u %u %u %u\n", player_no,
		player->x, player->y, player->inventory[0], player->inventory[1],
		player->inventory[2], player->inventory[3], player->inventory[4],
		player->inventory[5], player->inventory[6]);
	send(fd, buff, strlen(buff), 0);
}
