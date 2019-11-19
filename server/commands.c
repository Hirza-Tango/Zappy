/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 12:12:12 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/19 18:35:31 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

void	handle_unknown(t_server_state *s, int fd)
{
	static char	buff[256];
	int i;

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
		if (!strncmp(s->teams[i], buff, strlen(s->teams[i])))
		{
			//TODO: if not enough players, reject
			//TODO: if egg is available, spawn at egg
			s->clients[fd].handler = handle_player;
			s->clients[fd].type = PLAYER;
			s->clients[fd].player = new_player(fd, buff);
			//TODO: send stuff
			return ;
		}
		i++;
	}
	send(fd, "Unknown team\n", 13, 0);
}

void	handle_monitor(t_server_state *s, int fd)
{

}

void	handle_player(t_server_state *s, int fd)
{

}

void	client_read(t_server_state *s, int fd)
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
