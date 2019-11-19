/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 14:44:49 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/19 18:47:49 by dslogrov         ###   ########.fr       */
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

static void	validate_args(t_game_state *game_state,
	t_server_state *server_state)
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
	int				flag;
	struct rlimit	rlp;

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
			exit_error("Invalid flag", INVALID_FLAG);
	}
	getrlimit(RLIMIT_NOFILE, &rlp);
	validate_args(game_state, server_state);
	server_state->max_clients = rlp.rlim_cur;
	server_state->clients = (t_client *)malloc(rlp.rlim_cur * sizeof(t_client));
	server_state->current_players = 0;
	server_state->n_teams = game_state->n_teams;
	server_state->teams = game_state->n_teams;
}
