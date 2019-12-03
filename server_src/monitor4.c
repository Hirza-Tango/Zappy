/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/26 15:33:53 by dslogrov          #+#    #+#             */
/*   Updated: 2019/12/03 10:50:31 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

void	monitor_pdr(t_state *s, t_player *p, int i)
{
	char	buff[STRBUFF_SIZE];

	snprintf(buff, STRBUFF_SIZE, "pdr #%zu %u\n", p->player_no, i);
	send_all_monitors(s, buff);
}

void	monitor_pgt(t_state *s, t_player *p, int i)
{
	char	buff[STRBUFF_SIZE];

	snprintf(buff, STRBUFF_SIZE, "pgt #%zu %u\n", p->player_no, i);
	send_all_monitors(s, buff);
}

void	monitor_pdi(t_state *s, t_player *p)
{
	char	buff[STRBUFF_SIZE];

	snprintf(buff, STRBUFF_SIZE, "pdi #%zu\n", p->player_no);
	send_all_monitors(s, buff);
}

void	monitor_eht(t_state *s, t_egg *e)
{
	char	buff[STRBUFF_SIZE];

	snprintf(buff, STRBUFF_SIZE, "eht #%u\n", e->egg_no);
	send_all_monitors(s, buff);
}


void	monitor_edi(t_state *s, t_egg *e)
{
	char	buff[STRBUFF_SIZE];

	snprintf(buff, STRBUFF_SIZE, "edi #%u\n", e->egg_no);
	send_all_monitors(s, buff);
}
