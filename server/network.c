/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 14:55:59 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/13 15:48:45 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

static void				*get_ip(struct sockaddr *a)
{
	if (a->sa_family == AF_INET)
		return (&((struct sockaddr_in *)a)->sin_addr);
	else
		return (&((struct sockaddr_in6 *)a)->sin6_addr);
}

static struct addrinfo	*get_info(t_server_state *s, struct addrinfo **ai)
{
	struct addrinfo	hints;
	int				err;

	bzero(&hints, sizeof(hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((err = getaddrinfo(NULL, s->port, &hints, ai)) != 0)
	{
		perror("WTF");
		exit_error("Could not obtain address info", BAD_ADDR);
	}
	return (*ai);
}

void					create_listener(t_server_state *s)
{
	struct addrinfo *ai;
	struct addrinfo *p;
	int				o;

	o = 1;
	p = get_info(s, &ai);
	while (p)
	{
		s->fd_listen = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (s->fd_listen >= 0)
		{
			setsockopt(s->fd_listen, SOL_SOCKET, SO_REUSEADDR, &o, sizeof(int));
			if (bind(s->fd_listen, p->ai_addr, p->ai_addrlen) < 0)
				close(s->fd_listen);
			else
				break ;
		}
		p = p->ai_next;
	}
	if (!p)
		exit_error("Could not bind to socket", BIND_FAIL);
	if (listen(s->fd_listen, 12) == -1)
		exit_error("Could not listen on bound port", LISTEN_FAIL);
	freeaddrinfo(ai);
	FD_SET(s->fd_listen, &s->fd_read);
}

static void				create_connection(t_server_state *s)
{
	socklen_t				addr_len;
	struct sockaddr_storage	remote_addr;
	int						fd;
	char					ip[INET6_ADDRSTRLEN];

	addr_len = sizeof(remote_addr);
	//we need a datastructure to keep track of clients/guis
	fd = accept(s->fd_listen, (struct sockaddr *)&remote_addr, &addr_len);
	if (fd == -1)
		puts("Failed to accept incoming connection");
	else
	{
		FD_SET(fd, &s->fd_read);
		s->max_fd = MAX(s->max_fd, fd);
		printf("New connection from %s on fd %d\n", inet_ntop(
			remote_addr.ss_family, get_ip((struct sockaddr *)&remote_addr),
			ip, INET6_ADDRSTRLEN), fd);
	}
}

/*
**	TODO: flesh this out properly
*/

void					handle_command(t_server_state *s, int fd)
{
	char	buff[256];

	buff[recv(fd, buff, 255, 0)] = 0;
	send(fd, buff, strlen(buff), 0);
}

void					communicate(t_server_state *s)
{
	int				i;
	fd_set			fds;
	struct timeval	timeout;
	int				r;

	bzero(&timeout, sizeof(timeout));
	fds = s->fd_read;
	if ((r = select(s->max_fd + 1, &fds, NULL, NULL, &timeout)) == -1)
		exit_error("Select failed", SELECT_FAIL);
	i = 0;
	while (i <= s->max_fd && r)
	{
		if (FD_ISSET(i, &fds))
		{
			if (i == s->fd_listen)
				create_connection(s);
			else
				handle_command(s, i);
			r--;
		}
		i++;
	}
}
