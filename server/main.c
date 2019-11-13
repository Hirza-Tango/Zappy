/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/13 16:06:51 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/13 16:07:08 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

int		main(int argc, char **argv)
{
	t_game_state	game_state;
	t_server_state	server_state;

	parse_args(argc, argv, &game_state, &server_state);
	//generate gameboard
	create_listener(&server_state);
	server_state.max_fd = server_state.fd_listen;
	while (1)
	{
		communicate(&server_state);
		//handle message instructions
		//handle game events
	}
}
