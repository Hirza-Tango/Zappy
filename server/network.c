/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   network.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 14:55:59 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/26 10:35:27 by dslogrov         ###   ########.fr       */
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

static struct addrinfo	*get_info(t_state *s, struct addrinfo **ai)
{
	struct addrinfo	hints;
	int				err;

	bzero(&hints, sizeof(hints));
	hints.ai_family = PF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if ((err = getaddrinfo(NULL, s->port, &hints, ai)) != 0)
		exit_error("Could not obtain address info", BAD_ADDR);
	return (*ai);
}

void					create_listener(t_state *s)
{
	struct addrinfo *ai;
	struct addrinfo *p;
	int				fd;

	p = get_info(s, &ai);
	while (p)
	{
		fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (fd >= 0)
		{
			if (bind(fd, p->ai_addr, p->ai_addrlen) >= 0)
				break ;
			close(fd);
		}
		p = p->ai_next;
	}
	if (!p)
		exit_error("Could not bind to socket", BIND_FAIL);
	if (listen(fd, 12) == -1)
		exit_error("Could not listen on bound port", LISTEN_FAIL);
	freeaddrinfo(ai);
	s->clients[fd].type = SERVER;
	FD_SET(fd, &s->fd_read);
}

static void				create_connection(t_state *s, int server_fd)
{
	socklen_t				addr_len;
	struct sockaddr_storage	remote_addr;
	int						fd;
	char					ip[INET6_ADDRSTRLEN];

	addr_len = sizeof(remote_addr);
	fd = accept(server_fd, (struct sockaddr *)&remote_addr, &addr_len);
	if (fd == -1)
		puts("Failed to accept incoming connection");
	else
	{
		FD_SET(fd, &s->fd_read);
		s->max_fd = MAX(s->max_fd, fd);
		printf("New connection from %s on fd %d\n", inet_ntop(
			remote_addr.ss_family, get_ip((struct sockaddr *)&remote_addr),
			ip, INET6_ADDRSTRLEN), fd);
		s->clients[fd].buf_read = cbuff_create(256, 10);
		s->clients[fd].type = UNKNOWN;
		send(fd, "BIENVENUE\n", 10, 0);
	}
}

void					communicate(t_state *s)
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
			if (s->clients[i].type == SERVER)
				create_connection(s, i);
			else
				client_read(s, i);
			r--;
		}
		i++;
	}
}
