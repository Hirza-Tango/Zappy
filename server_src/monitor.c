/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 11:33:11 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/26 15:43:25 by dslogrov         ###   ########.fr       */
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
	char	buff[256];

	snprintf(buff, 256, "seg %s\n", team);
	send_all_monitors(s, buff);
}

void	monitor_smg(t_state *s, char *message)
{
	char	buff[256];

	snprintf(buff, 256, "smg %s\n", message);
	send_all_monitors(s, buff);
}

void	init_monitor(t_state *s, int fd)
{
	t_list *list;

	monitor_msz(s, fd);
	monitor_sgt(s, fd);
	monitor_mct(s, fd);
	monitor_tna(s, fd);
	list = s->players;
	while (list)
	{
		monitor_pnw(s, fd, list->content);
		list = list->next;
	}
	list = s->eggs;
	while (list)
	{
		monitor_enw(s, fd, list->content);
		list = list->next;
	}
}
