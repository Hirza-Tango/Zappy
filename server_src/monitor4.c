/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 15:33:53 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/03 10:18:14 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

void	monitor_pdr(t_state *s, t_player *p, int i)
{
	char	buff[256];

	snprintf(buff, 256, "pdr #%zu %u\n", p->player_no, i);
	send_all_monitors(s, buff);
}

void	monitor_pgt(t_state *s, t_player *p, int i)
{
	char	buff[256];

	snprintf(buff, 256, "pgt #%zu %u\n", p->player_no, i);
	send_all_monitors(s, buff);
}

void	monitor_pdi(t_state *s, t_player *p)
{
	char	buff[256];

	snprintf(buff, 256, "pdi #%zu\n", p->player_no);
	send_all_monitors(s, buff);
}

void	monitor_eht(t_state *s, t_egg *e)
{
	char	buff[256];

	snprintf(buff, 256, "eht #%u\n", e->egg_no);
	send_all_monitors(s, buff);
}


void	monitor_edi(t_state *s, t_egg *e)
{
	char	buff[256];

	snprintf(buff, 256, "edi #%u\n", e->egg_no);
	send_all_monitors(s, buff);
}
