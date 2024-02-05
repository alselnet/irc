/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:22:50 by jthuysba          #+#    #+#             */
/*   Updated: 2024/02/05 15:02:40 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/Channel.hpp"
# include "../include/User.hpp"

// void	create_new_user( std::string str, std::list< User > * usersList )
// {
// 	std::istringstream	iss(str);
// 	std::string				token;
// 	std::string				nickname;
// 	std::string				username;
// 	std::string				ip;
// 	std::string				realname;

// 	while (iss >> token && token != "NICK")
// 	{
// 	}

// 	iss >> nickname;
// 	iss >> token;
// 	iss >> username;
// 	iss >> token;
// 	iss >> ip;
// 	iss >> realname;
// 	realname.erase(realname.begin());
 
// 	User	newUser = User(nickname, username, realname, ip);

// 	usersList->push_back(newUser);

// 	std::cout << "[" << usersList->begin()->getNickname() << "] added !" << std::endl;
// }


void	parse_transmission( char * buffer, std::list< User > * usersList)
{
	(void) usersList;
	
	const std::string		str(buffer);
	std::istringstream	iss(str);
	std::string				line;
	
	while (std::getline(iss, line, '\r'))
	{
		if (!line.empty())
		{
			std::cout << "[" << line << "]" << std::endl;
		}
		iss.ignore();
	}
}