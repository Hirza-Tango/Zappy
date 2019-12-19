/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player4.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/12 17:35:14 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/19 17:43:53 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

void			player_connect_nbr(t_state *s, int fd)
{
	char	buff[STRBUFF_SIZE];

	snprintf(buff, STRBUFF_SIZE, "%u\n",
		s->teams[s->clients[fd].player->team_no].nb_client);
	send(fd, buff, strlen(buff), 0);
}

void		player_death(t_state *s, int fd)
{
	int i;
	t_player *player = s->clients[fd].player;

	i = -1;
	while (++i < NUM_RESOURCES)
		s->board[player->x][player->y][i] += player->inventory[i];
	s->clients[fd].type = UNKNOWN;
	send(fd, "death\n", 6, 0);
	monitor_pdi(s, s->clients[fd].player);
	free(s->clients[fd].player);
	s->clients[fd].player = NULL;
}
