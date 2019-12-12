/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 11:33:11 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/12 16:12:12 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

void	send_all_monitors(t_state *s, char *buffer)
{
	int	i;

	i = -1;
	while (++i <= s->max_fd)
		if (s->clients[i].type == MONITOR)
			send(i, buffer, strlen(buffer), 0);
}

void	monitor_seg(t_state *s, char *team)
{
	char	buff[STRBUFF_SIZE];

	snprintf(buff, STRBUFF_SIZE, "seg %s\n", team);
	send_all_monitors(s, buff);
}

void	monitor_smg(t_state *s, char *message)
{
	char	buff[STRBUFF_SIZE];

	snprintf(buff, STRBUFF_SIZE, "smg %s\n", message);
	send_all_monitors(s, buff);
}

void	monitor_sst(t_state *s, int fd, int time)
{
	if (time > 0)
	{
		s->time = time;
		monitor_sgt(s, -1);
	}
	else
		send(fd, "sbp\n", 4, 0);
}

void	init_monitor(t_state *s, int fd)
{
	t_list	*list;
	int		i;

	s->clients[fd].type = MONITOR;
	monitor_msz(s, fd);
	monitor_sgt(s, fd);
	monitor_mct(s, fd);
	monitor_tna(s, fd);
	i = -1;
	while (++i <= s->max_fd)
		if (s->clients[i].type == PLAYER)
			monitor_pnw(s, fd, s->clients[i].player);
	i = -1;
	while (++i < s->n_teams)
	{
		list = s->teams[i].eggs;
		while (list)
		{
			monitor_enw(s, fd, list->content);
			list = list->next;
		}
	}
}
