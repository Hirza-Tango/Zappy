NAME=zappy
DEPS=libft.a
LIBFT_DIR=mini_libft
INCLUDES=$(LIBFT_DIR)/includes
REL_DEPS=$(DEPS:%=$(LIBFT_DIR)/%)
CC=gcc
CFLAGS=-Wall -Wextra -Werror -I . -I $(INCLUDES) -Ofast
SERVER_FILES=	args.c	execute.c	monitor.c	monitor2.c	monitor4.c		\
	network.c	player1.c	player3.c	ring_buffer.c	util.c	board.c		\
	handle.c	monitor1.c	monitor3.c	monitor5.c	player.c	player2.c	\
	player3.c	player4.c	zappy_server.c
CLIENT_FILES= main.c
SERVER_FILES := $(addprefix server_src/,$(SERVER_FILES))
CLIENT_FILES := $(addprefix client_src/,$(CLIENT_FILES))

SERVER_OBJ:=$(SERVER_FILES:server_src/%.c=build/server/%.o)
CLIENT_OBJ:=$(CLIENT_FILES:client_src/%.c=build/client/%.o)

$(NAME): server client gfx

server: $(SERVER_OBJ) $(REL_DEPS)
	@gcc $(CFLAGS) -o $@ $^

client: $(CLIENT_OBJ) $(REL_DEPS)
	@gcc $(CFLAGS) -o $@ $^

$(REL_DEPS):
	@make -C $(dir $@)

build/server/%.o: server_src/%.c
	@mkdir -p build/server
	@$(CC) $(CFLAGS) -c $< -o $@

build/client/%.o: client_src/%.c
	@mkdir -p build/client
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME);

clean fclean re::
	@for file in $(dir $(REL_DEPS)) ; do $(MAKE) -C $$file $@ ; done

clean::
	@rm -rf build/

fclean:: clean
	@rm -rf $(NAME)

re:: fclean all

.PHONY: clean fclean re all $(REL_DEPS)
