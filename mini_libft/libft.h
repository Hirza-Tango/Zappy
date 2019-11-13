/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dslogrov <dslogrove@gmail.com>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/19 12:02:56 by dslogrov          #+#    #+#             */
/*   Updated: 2019/11/13 17:23:23 by dslogrov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <string.h>
# include <stdlib.h>
# include <unistd.h>
# ifdef __linux__
#  include <inttypes.h>
# endif

typedef struct	s_list
{
	void			*content;
	size_t			content_size;
	struct s_list	*next;
}				t_list;

t_list			*ft_lstnew(void const *content, size_t content_size);
void			ft_lstdelone(t_list **alst, void (*del)(void*, size_t));
void			ft_lstdel(t_list **alst, void (*del)(void *, size_t));
void			ft_lstadd(t_list **alst, t_list *elem);
void			ft_lstiter(t_list *lst, void (*f)(t_list *elem));
t_list			*ft_lstmap(t_list *lst, t_list *(*f)(t_list *elem));
void			ft_lstswap(t_list *a, t_list *b);
t_list			**ft_lstsort(t_list **lst,
	long (*f)(const void *, const void *));
void			ft_lstappend(t_list **alst, t_list *elem);
void			ft_lstrev(t_list **lst);
size_t			ft_lstlen(t_list *list);
void			ft_lstrm(t_list **list, size_t index,
	void (*del)(void *, size_t));

typedef struct	s_d_list
{
	void			*content;
	size_t			content_size;
	struct s_d_list	*next;
	struct s_d_list *prev;
}				t_d_list;

t_d_list		*ft_dlstnew(void const *content, size_t content_size);
void			ft_dlstdelone(t_d_list **alst, void (*del)(void*, size_t));
void			ft_dlstdel(t_d_list **alst, void (*del)(void*, size_t));
void			ft_dlstadd(t_d_list **alst, t_d_list *elem);
void			ft_dlstiter(t_d_list *lst, void (*f)(t_d_list *elem));
t_d_list		*ft_dlstmap(t_d_list *lst, t_d_list *(*f)(t_d_list *elem));

#endif
