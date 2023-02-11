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
		while (s1[i] && (s1[i] == '_' || s1[i] == '.' || s1[i] == '@'))
			i++;
		while (s2[j] && (s2[j] == '_' || s2[j] == '.' || s2[j] == '@'))
			j++;
		if (!s1[i] || ft_tolower(s1[i]) != ft_tolower(s2[j]))
			break ;
		i++;
		j++;
	}
	ret = ft_tolower(s1[i]) - ft_tolower(s2[j]);
	if (ret == 0)
	{
		return (ft_strlen(s2) - ft_strlen(s1));
	}
	return (ret);
}

static t_list	*merge(t_list *left, t_list *right, void const *str)
{
	t_list *result = NULL;

	if (left == NULL)
		return (right);
	else if (right == NULL)
		return (left);

	if (is_sorted(right, left, str) >= 0)
	{
		result = left;
		result->next = merge(left->next, right, str);
	}
	else
	{
		result = right;
		result->next = merge(left, right->next, str);
	}
	return (result);
}

t_list *merge_sort(t_list *head, void const *str)
{
	t_list	*left = NULL;
	t_list 	*right = NULL;
	t_list 	*current = head;
	t_list 	*tail = NULL;
	t_list 	*middle = NULL;
	size_t	count = 0;

	if (head == NULL || head->next == NULL)
		return (head);

	while (current != NULL)
	{
		count++;
		current = current->next;
	}

	current = head;
	middle = head;
	for (size_t i = 0; i < count / 2; i++)
	{
		tail = middle;
		middle = middle->next;
	}

	tail->next = NULL;
	left = merge_sort(head, str);
	right = merge_sort(middle, str);

	return (merge(left, right, str));
}
