/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 15:31:25 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/26 15:31:58 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"


void	monitor_msz(t_state *s, int fd)
{
	char	buff[256];

	snprintf(buff, 256, "msz %u %u\n", s->size_x, s->size_y);
	send(fd, buff, strlen(buff), 0);
}

void	monitor_bct(t_state *s, int fd, unsigned int x, unsigned int y)
{
	char	buff[256];

	snprintf(buff, 256, "bct %u %u %u %u %u %u %u %u %u\n", x,
		y, s->board[x][y][0], s->board[x][y][1], s->board[x][y][2],
		s->board[x][y][3], s->board[x][y][4], s->board[x][y][5],
		s->board[x][y][6]);
	send(fd, buff, strlen(buff), 0);
}

void	monitor_mct(t_state *s, int fd)
{
	unsigned int i;
	unsigned int j;

	i = -1;
	while (++ i < s->size_x)
	{
		j = -1;
		while (++j < s->size_y)
			monitor_bct(s, fd, i, j);
	}
}

void	monitor_tna(t_state *s, int fd)
{
	char	buff[256];
	size_t	i;

	i = -1;
	while (++i < s->n_teams)
	{
		snprintf(buff, 256, "tna %s\n", s->teams[i].name);
		send(fd, buff, strlen(buff), 0);
	}
}

void	monitor_sgt(t_state *s, int fd)
{
	char	buff[256];

	snprintf(buff, 256, "sgt %zu\n", s->time);
	send(fd, buff, strlen(buff), 0);
}
