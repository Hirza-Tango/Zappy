/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 15:25:53 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/04 17:07:55 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"
#include <math.h>

void	exit_error(char *message, int code)
{
	fprintf(stderr, "%d: %s\n", code, message);
	exit(code);
}

struct timespec	*add_time(struct timespec *a, long double b)
{
	long		nanoseconds;
	long double	seconds;

	nanoseconds = (long)(modfl(b, &seconds) * 1E9);
	if (a->tv_nsec + nanoseconds >= BILLION)
	{
		a->tv_sec = a->tv_sec + (time_t)seconds + 1;
		a->tv_nsec = a->tv_nsec + nanoseconds - BILLION;
	}
	else
	{
		a->tv_sec = a->tv_sec + (time_t)seconds;
		a->tv_nsec = a->tv_nsec + nanoseconds;
	}
	return (a);
}
