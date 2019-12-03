/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 15:32:24 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/03 14:06:22 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"


void	monitor_pnw(t_state *s, int fd, t_player *player)
{
	char		buff[STRBUFF_SIZE];

	snprintf(buff, STRBUFF_SIZE, "pnw #%zu %u %u %u %u %s\n", player->player_no,
		player->x, player->y, player->direction, player->level,
		s->teams[player->team_no].name);
	if (fd < 0)
		send_all_monitors(s, buff);
	else
		send(fd, buff, strlen(buff), 0);
}

void	monitor_enw(t_state *s, int fd, t_egg *egg)
{
	char	buff[STRBUFF_SIZE];

	snprintf(buff, STRBUFF_SIZE, "enw #%u #%zu %u %u\n", egg->egg_no, egg->player_no,
		egg->x, egg->y);
	if (fd < 0)
		send_all_monitors(s, buff);
	else
		send(fd, buff, strlen(buff), 0);
}

void	monitor_ppo(t_state *s, int fd, unsigned int player_no)
{
	char		buff[STRBUFF_SIZE];
	t_player	*player;
	size_t		i;

	i = -1;
	while (++i < s->max_fd)
	{
		if (s->clients[i].type == PLAYER &&
			s->clients[i].player->player_no == player_no)
		{
			player = s->clients[i].player;
			break ;
		}
	}
	if (!player)
	{
		send(fd, "sbp\n", 4, 0);
		return ;
	}
	snprintf(buff, STRBUFF_SIZE, "ppo #%u %u %u %u\n", player_no, player->x, player->y,
		player->direction);
	send(fd, buff, strlen(buff), 0);
}

void	monitor_plv(t_state *s, int fd, unsigned int player_no)
{
	char		buff[STRBUFF_SIZE];
	t_player	*player;
	size_t		i;

	i = -1;
	while (++i < s->max_fd)
	{
		if (s->clients[i].type == PLAYER &&
			s->clients[i].player->player_no == player_no)
		{
			player = s->clients[i].player;
			break ;
		}
	}
	if (!player)
	{
		send(fd, "sbp\n", 4, 0);
		return ;
	}
	snprintf(buff, STRBUFF_SIZE, "plv #%u %u\n", player_no, player->level);
	send(fd, buff, strlen(buff), 0);
}

void	monitor_pin(t_state *s, int fd, unsigned int player_no)
{
	char		buff[STRBUFF_SIZE];
	t_player	*player;
	size_t		i;

	i = -1;
	player = NULL;
	while (++i < s->max_fd)
	{
		if (s->clients[i].type == PLAYER &&
			s->clients[i].player->player_no == player_no)
		{
			player = s->clients[i].player;
			break ;
		}
	}
	if (!player)
	{
		send(fd, "sbp\n", 4, 0);
		return ;
	}
	snprintf(buff, STRBUFF_SIZE, "pin #%u %u %u %u %u %u %u %u %u %u\n", player_no,
		player->x, player->y, player->inventory[0], player->inventory[1],
		player->inventory[2], player->inventory[3], player->inventory[4],
		player->inventory[5], player->inventory[6]);
	send(fd, buff, strlen(buff), 0);
}
