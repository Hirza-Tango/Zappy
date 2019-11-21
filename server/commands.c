/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 12:12:12 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/21 09:30:00 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

void	handle_monitor(t_state *s, int fd)
{

}

void	handle_player(t_state *s, int fd)
{

}

void	handle_unknown(t_state *s, int fd)
{
	static char	buff[256];
	size_t i;

	if (!cbuff_read(s->clients[fd].buf_read, buff))
		return ;
	if (!strcmp(buff, "GRAPHIC\n"))
	{
		s->clients[fd].type = MONITOR;
		s->clients[fd].handler = handle_monitor;
		//TODO: send stuff
		return ;
	}
	i = 0;
	while (i < s->n_teams)
	{
		if (!strcmp(s->teams[i].name, buff))
		{
			if (s->teams[i].nb_client < 1)
				send(fd, "Team is full\n", 13, 0);
			else
			{
				//TODO: if egg is available, spawn at egg
				s->clients[fd].handler = handle_player;
				s->clients[fd].type = PLAYER;
				s->clients[fd].player = new_player(fd, buff);
				send(fd, buff, snprintf(buff, 256, "%u\n%u %u\n", s->teams[i]		.nb_client, s->size_x, s->size_y), 0);
			}
			return ;
		}
		i++;
	}
	send(fd, "Unknown team\n", 13, 0);
}

void	client_read(t_state *s, int fd)
{
	char	buff[256];
	int		r;

	recv(fd, buff, 255, 0);
	if (r <= 0)
	{
		close(fd);
		FD_CLR(fd, &(s->fd_read));
		cbuff_destroy(s->clients[fd].buf_read);
		bzero(&s->clients[fd], sizeof(t_client));
		printf("client #%d disconnected\n", fd);
		return ;
	}
	buff[r] = 0;
	//TODO: handle newlines
	// Perhaps stringsplit?
	cbuff_write(s->clients[fd].buf_read, buff);
}
