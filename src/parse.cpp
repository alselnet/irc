/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:22:50 by jthuysba          #+#    #+#             */
/*   Updated: 2024/02/01 14:04:39 by jthuysba         ###   ########.fr       */
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

	// std::cout << "Nick : " << nickname << std::endl;
	// std::cout << "User : " << username << std::endl;
	// std::cout << "Ip : " << ip << std::endl;
	// std::cout << "Real : " << realname << std::endl;

	User	newUser = User(nickname, username, realname, ip);

	// std::cout << "Nick : [" << newUser.getNickname() << "]" << std::endl;
	// std::cout << "User : [" << newUser.getUsername() << "]" << std::endl;
	// std::cout << "Ip : [" << newUser.getIp() << "]" << std::endl;
	// std::cout << "Real : [" << newUser.getRealname() << "]" << std::endl;

	usersList.push_back(newUser);

}