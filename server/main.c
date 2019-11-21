/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 16:06:51 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/21 09:25:18 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

int		main(int argc, char **argv)
{
	t_state	state;

	parse_args(argc, argv, &state);
	gen_board(&state);
	create_listener(&state);
	while (1)
	{
		communicate(&state);
		//handle message instructions
		//handle game events
	}
}
