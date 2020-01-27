NAME=libft.a
CC=gcc
CFLAGS=-c -Wall -Wextra -Werror -I. -Ofast
CFILES=	ft_lstdelone.c	ft_lstdel.c		ft_lstadd.c		ft_lstiter.c	\
		ft_lstmap.c		ft_lstadd_sorted.c								\
		ft_lstswap.c	ft_lstsort.c	ft_lstappend.c	ft_lstrev.c		\
		ft_lstlen.c		ft_lstnew.c										\
		ft_lstrm.c		ft_dlstadd.c	ft_dlstdel.c	ft_dlstdelone.c	\
		ft_dlstnew.c	ft_dlstmap.c	ft_strsplit.c	ft_strndup.c

OBJ=$(CFILES:%.c=build/%.o)

$(NAME): $(OBJ)
	@ar rcs $(NAME) $(OBJ)

build/%.o: %.c libft.h
	@mkdir -p build
	@$(CC) $(CFLAGS) $< -o $@

all: $(NAME);

clean:
	@rm -rf build/

fclean: clean
	@rm -rf $(NAME)

re: fclean all

.PHONY: clean fclean re all
