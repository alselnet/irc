/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:22:50 by jthuysba          #+#    #+#             */
/*   Updated: 2024/02/06 12:30:43 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/Channel.hpp"
# include "../include/User.hpp"

void	parse_transmission( char * buffer, std::list< User > usersList)
{
	(void) usersList;

	std::string	str(buffer);

	std::istringstream	iss(str);
	std::string				token;
	std::string				nickname;
	std::string				username;
	std::string				ip;
	std::string				realname;

	while (iss >> token && token != "NICK")
	{
	}

	iss >> nickname;
	iss >> token;
	iss >> username;
	iss >> token;
	iss >> ip;
	iss >> realname;
	realname.erase(realname.begin());

	User	newUser = User(nickname, username, realname, ip);
	usersList.push_back(newUser);
	return ;
}