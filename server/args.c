/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/12 14:44:49 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/26 10:36:47 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "zappy_server.h"

static void	parse_teams(int argc, char **argv, t_state *state)
{
	size_t	i;
	size_t	j;

	i = optind - 1;
	while (optind < argc && argv[optind][0] != '-')
		state->n_teams++ && optind++;
	state->teams = (t_team *)malloc(state->n_teams * sizeof(t_team));
	j = 0;
	while (i < optind)
		state->teams[j++].name = strdup(argv[i++]);
}

static void	validate_args(t_state *state)
{
	if (!state->port || !state->size_x || !state->size_y ||
		!state->n_teams || !state->allowed_players)
	{
		dprintf(2, "Missing option\n");
		exit(MISSING_OPTION);
	}
}

static void	init_states(t_state *state)
{
	struct rlimit	rlp;
	unsigned int	i;

	getrlimit(RLIMIT_NOFILE, &rlp);
	state->max_clients = rlp.rlim_cur;
	state->clients = (t_client *)malloc(rlp.rlim_cur * sizeof(t_client));
	i = 0;
	while (i < state->n_teams)
		state->teams[i++].nb_client = state->max_clients;
	state->n_teams = state->n_teams;
	state->teams = state->teams;
}

void		parse_args(int argc, char **argv, t_state *state)
{
	int				flag;

	while ((flag = getopt(argc, argv, "p:x:y:n:c:t:")) != -1)
	{
		if (flag == 'p')
			state->port = optarg;
		else if (flag == 'x')
			state->size_x = atoi(optarg);
		else if (flag == 'y')
			state->size_y = atoi(optarg);
		else if (flag == 'c')
			state->allowed_players = atoi(optarg);
		else if (flag == 't')
			state->time = atoi(optarg);
		else if (flag == 'n')
			parse_teams(argc, argv, state);
		else
			exit_error("Invalid flag", INVALID_FLAG);
	}
	validate_args(state);
	init_states(state);
}
