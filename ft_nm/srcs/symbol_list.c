#include "ft_nm.h"
#include <elf.h>

static int		is_sorted(t_list *a, t_list *b, void const *str)
{
	size_t	i = 0, j = 0;
	int		ret = 0;
	char	*s1;
	char	*s2;

	s1 = (char*)(str + ((Elf64_Sym*)a->content)->st_name);
	s2 = (char*)(str + ((Elf64_Sym*)b->content)->st_name);
	while (s1[i] && s2[j])
	{
		while (s1[i] && s1[i] == '_')
			i++;
		while (s2[j] && s2[j] == '_')
			j++;
		if (!s1[i] || ft_tolower(s1[i]) != ft_tolower(s2[j]))
			break ;
		i++;
		j++;
	}
	ret = ft_tolower(s1[i]) - ft_tolower(s2[j]);
	if (ret == 0)
	{
		return (ELF64_ST_BIND(((Elf64_Sym*)a->content)->st_info)
			- ELF64_ST_BIND(((Elf64_Sym*)b->content)->st_info));
	}
	return (ret);
}

static t_list *swap(t_list *ptr1, t_list *ptr2)
{
    t_list *tmp = ptr2->next;
    ptr2->next = ptr1;
    ptr1->next = tmp;
    return ptr2;
}

int sort_list(t_list **head, void const *str)
{
    t_list	**h;
	int		count = ft_lstlen(*head);
    int		i, j, swapped;

    for (i = 0; i <= count; i++)
    {
        h = head;
        swapped = 0;
        for (j = 0; j < count - i - 1; j++)
        {
            t_list *p1 = *h;
            t_list *p2 = p1->next;
            if (is_sorted(p1, p2, str) > 0)
            {
                *h = swap(p1, p2);
                swapped = 1;
            }
            h = &(*h)->next;
        }
        if (swapped == 0)
            break;
    }
	return (0);
}
