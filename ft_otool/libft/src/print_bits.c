/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_bits.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpivet-p <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/01/13 16:07:14 by mpivet-p          #+#    #+#             */
/*   Updated: 2021/05/29 20:00:03 by mpivet-p         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_print_bits(long long unsigned int nbr)
{
	char	tab[64];
	int		i;

	i = 0;
	while (i < 64)
	{
		if (nbr & 1ULL << i)
			tab[i] = '1';
		else
			tab[i] = '0';
		i++;
	}
	while (i >= 0)
	{
		ft_putchar(tab[i]);
		if (i != 64 && i % 8 == 0 && i != 0)
			ft_putchar(' ');
		i--;
	}
	ft_putchar('\n');
}
