/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 14:44:49 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/12 17:22:05 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

static void	parse_teams(int argc, char **argv, t_game_state *game_state)
{
	size_t	i;
	size_t	j;

	i = optind - 1;
	while (optind < argc && argv[optind][0] != '-')
		game_state->n_teams++ && optind++;
	game_state->teams = (char **)malloc(game_state->n_teams * sizeof(char *));
	j = 0;
	while (i < optind)
		game_state->teams[j++] = strdup(argv[i++]);
}

static void	validate_args(t_game_state *game_state, t_server_state *server_state)
{
	if (!server_state->port || !game_state->size_x || !game_state->size_y ||
		!game_state->n_teams || !server_state->allowed_players)
	{
		dprintf(2, "Missing option");
		exit(MISSING_OPTION);
	}
}

void		parse_args(int argc, char **argv,
	t_game_state *game_state, t_server_state *server_state)
{
	int flag;

	while ((flag = getopt(argc, argv, "p:x:y:n:c:t:")) != -1)
	{
		if (flag == 'p')
			server_state->port = optarg;
		else if (flag == 'x')
			game_state->size_x = atoi(optarg);
		else if (flag == 'y')
			game_state->size_y = atoi(optarg);
		else if (flag == 'c')
			server_state->allowed_players = atoi(optarg);
		else if (flag == 't')
			game_state->time = atoi(optarg);
		else if (flag == 'n')
			parse_teams(argc, argv, game_state);
		else
		{
			dprintf(2, "Invalid flag %i", flag);
			exit(INVALID_FLAG);
		}
	}
	server_state->current_players = 0;
	validate_args(game_state, server_state);
}
