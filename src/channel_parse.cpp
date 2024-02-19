// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   channel_parse.cpp                                  :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ctchen <ctchen@student.42.fr>              +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/02/19 10:16:05 by ctchen            #+#    #+#             //
//   Updated: 2024/02/19 10:16:57 by ctchen           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "irc.hpp"
#include "Reply.hpp"
#include "Notif.hpp"
#include "Error.hpp"

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
		std::list<std::string> oplist = chan->getOperatorsList();
		for (std::list<std::string>::const_iterator it = oplist.begin();
			 it != oplist.end(); ++it)
		{
			if ((*it) == user->getNickname())
				return true;
		}
	}
	return false;
}
