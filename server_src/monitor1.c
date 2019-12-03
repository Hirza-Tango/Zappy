/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 15:31:25 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/03 16:36:52 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"


void	monitor_msz(t_state *s, int fd)
{
	char	buff[STRBUFF_SIZE];

	snprintf(buff, STRBUFF_SIZE, "msz %u %u\n", s->size_x, s->size_y);
	send(fd, buff, strlen(buff), 0);
}

void	monitor_bct(t_state *s, int fd, unsigned int x, unsigned int y)
{
	char	buff[STRBUFF_SIZE];

	snprintf(buff, STRBUFF_SIZE, "bct %u %u %u %u %u %u %u %u %u\n", x,
		y, s->board[x][y][0], s->board[x][y][1], s->board[x][y][2],
		s->board[x][y][3], s->board[x][y][4], s->board[x][y][5],
		s->board[x][y][6]);
	if (fd < 0)
		send_all_monitors(s, buff);
	else
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
	char	buff[STRBUFF_SIZE];
	size_t	i;

	i = -1;
	while (++i < s->n_teams)
	{
		snprintf(buff, STRBUFF_SIZE, "tna %s\n", s->teams[i].name);
		send(fd, buff, strlen(buff), 0);
	}
}

void	monitor_sgt(t_state *s, int fd)
{
	char	buff[STRBUFF_SIZE];

	snprintf(buff, STRBUFF_SIZE, "sgt %zu\n", s->time);
	if (fd < 0)
		send_all_monitors(s, buff);
	else
		send(fd, buff, strlen(buff), 0);
}
