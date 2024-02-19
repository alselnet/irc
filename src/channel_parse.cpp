/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_parse.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:16:05 by ctchen            #+#    #+#             */
//   Updated: 2024/02/19 20:13:52 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Reply.hpp"
#include "Notif.hpp"
#include "Error.hpp"

unsigned long   word_comma_replace(std::string *str)
{
	unsigned long	count = 0;

	for (unsigned int i = 0; i < (*str).size(); i++)
	{
		if ((*str)[i] == ',')
		{
			(*str)[i] = ' ';
			count++;
		}
	}
	return (count);
}

std::string	word_picker(const std::string *str, unsigned int nb)
{
	std::istringstream	iss(*str);
	std::string			word;
    unsigned int		i = 0;

//	if ((*str).empty())
//		return ("");
	while (iss >> word)
	{
		i++;
		if (i == nb)
			return (word);
	}
	return ("");
}

bool	check_rights(std::list<User>::const_iterator user,
					std::list<Channel>::const_iterator chan)
{
	if (user->getOperator() == true)
		return true;
	else
	{
		for (std::list<std::string>::const_iterator it = chan->getOpListBegin();
			 it != chan->getOpListEnd(); ++it)
		{
			if ((*it) == user->getNickname())
				return true;
		}
	}
	return false;
}
