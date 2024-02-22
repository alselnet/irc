/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:22:50 by jthuysba          #+#    #+#             */
/*   Updated: 2024/02/22 16:33:53 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/Notif.hpp"
# include "../include/channel_command.hpp"

void	set_user_infos(std::string *str, int *clientSockFd, irc *irc_data)
{
	std::istringstream		iss(*str);
	std::string				username;
	std::string				hostname;
	std::string				realname;
	std::string				dump;

		
	iss >> dump;
	iss >> hostname;
	iss >> realname;

	realname.erase(0, 1);

	std::list<User>::iterator	user = get_user((*clientSockFd), irc_data);
	if (user->getNickname().empty())
	{
		std::cout << "Closing  the connection..." << std::endl;
		delete_user(*clientSockFd, irc_data);
		*clientSockFd = -1;
		return ;
	}
	user->setIp(hostname);
	user->setRealname(realname);
	print_user_infos(user);
	return ;
}

void	parse_transmission(char *buffer, int *clientSockFd, irc *irc_data)
{
	const std::string		str(buffer);
	std::istringstream		iss(str);
	std::string				line;
	char					end_char = 4;
	
	while (std::getline(iss, line, end_char) && *clientSockFd > -1)
	{
			if (line.find('\n') == std::string::npos)
			{
				irc_data->stock += line;
				return ;
			}
			
			line.erase(line.size() - 1);

			std::string	transmission = irc_data->stock + line;
			irc_data->stock.clear();
			irc_data->stock = "";
			
			std::istringstream	iss1(transmission);

			std::string	command;
			while (std::getline(iss1, command, '\r'))
			{
				std::cout << "[" << YELLOW << command << RESET << "]" << std::endl;
				if (get_user(*clientSockFd, irc_data) != irc_data->usersList.end())
				{
					std::cout << "Sent by : " << CYAN << get_user(*clientSockFd, irc_data)->getNickname() << END << std::endl;
				}
				execute_command(command, clientSockFd, irc_data);
				iss1.ignore();

			if  (*clientSockFd < 0)
				return ;
		}
	}
}
