#include "libft.h"

t_list	*ft_lstappend(t_list **alst, void const *content, size_t content_size)
{
	t_list *new = ft_lstnew(content, content_size);
	t_list *ptr = *alst;

	
	if (*alst == NULL)
		*alst = new;
	else
	{
		while (ptr->next != NULL)
			ptr = ptr->next;
		ptr->next = new;
			
	}
	return (new);
}
