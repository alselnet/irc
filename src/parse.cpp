/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:22:50 by jthuysba          #+#    #+#             */
//   Updated: 2024/02/19 00:36:11 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/Notif.hpp"
# include "../include/channel_command.hpp"


void	set_user_infos(std::string *str, int *clientSockFd, irc *irc_data)
{
	std::istringstream	iss(*str);
	std::string				username;
	std::string				hostname;
	std::string				realname;
	std::string				dump;

	iss >> dump;
	iss >> username;
	iss >> hostname;
	iss >> realname;

	realname.erase(0, 1);

	std::list<User>::iterator	user = get_user((*clientSockFd), irc_data);

	user->setUsername(username);
	user->setIp(hostname);
	user->setRealname(realname);
	print_user_infos(user);
	return ;
}

// parse la transmission ligne par ligne et execute chaque commande
void	parse_transmission(char *buffer, int clientSockFd, irc *irc_data)
{
//	std::cerr << "DEBUG: parse_transmission starting" << std::endl;
	const std::string		str(buffer);
	std::istringstream		iss(str);
	std::string				line;
	
	while (std::getline(iss, line, '\r'))
	{
		if (!line.empty())
		{
			std::cout << "[" << YELLOW << line << RESET << "]" << std::endl;
			if (get_user(clientSockFd, irc_data) != irc_data->usersList.end())
				std::cout << "Sent by : " << CYAN << get_user(clientSockFd, irc_data)->getNickname() << END << std::endl;
			execute_command(line, clientSockFd, irc_data);
		}
		iss.ignore();
	}
//	std::cerr << "DEBUG: parse_transmission ended successfully" << std::endl;
}
