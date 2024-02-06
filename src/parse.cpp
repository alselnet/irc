/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:22:50 by jthuysba          #+#    #+#             */
/*   Updated: 2024/02/06 12:43:59 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/Channel.hpp"
# include "../include/User.hpp"

// Return un iterator sur le user correspondant a clientSockFd
std::list<User>::iterator getUser( int clientSockFd, irc * irc_data )
{
	std::list<User>::iterator	it = irc_data->usersList.begin();
	std::list<User>::iterator	ite = irc_data->usersList.end();

	for (; it != ite; it++)
	{
		if (it->getSockFd() == clientSockFd)
			return (it);
	}
	// WIP => Gerer erreurs si fd non present
	return (ite);
}

// Execute la commande dans str
void	execute_command( std::string str, int clientSockFd, irc * irc_data )
{
	std::string	cmd;
	std::istringstream	iss(str);
	
	iss >> cmd;

	if (cmd == "CAP")
	{
		// WIP => Gerer capacite ?
	}
	else if (cmd == "NICK")
	{
		// WIP => Check si nick deja utilise
		std::string	nick;
		
		iss >> nick;
		getUser(clientSockFd, irc_data)->setNickname(nick);
	}
	else if (cmd == "USER")
	{
		// WIP => Coder une fonction qui gere "USER <username> <username> <ip> :<realname>" & "userhost <username>" (a voir)
	}
	else if (cmd == "PING")
	{
		pong(clientSockFd);
	}
	else if (!cmd.compare("MODE"))
	{
		std::string mode_reply = ":" + SERVER_NAME + " MODE abc +i\r\n";
		send(clientSockFd, mode_reply.c_str(), mode_reply.size(), 0);
	}
//	else if (cmd == "TOPIC")
//	{
//		getChannel()->changeTopic(getUser(clientSockFd, irc_data), str);
//	}
//	else if (cmd == "KICK")
//	{Readaptation en cours
//		getChannel()->kickUser(getUser(clientSockFd, irc_data), target);
//	}
//	else if (cmd == "INVITE")
//	{Readaptation en cours
//		getChannel()->addUser(getUser(clientSockFd, irc_data), target);
//	}
	// WIP => Toutes les autres commandes a ajouter
}

// parse la transmission ligne par ligne et execute chaque commande
void	parse_transmission( char * buffer, int clientSockFd, irc * irc_data )
{
	(void) irc_data;
	(void) clientSockFd;
	
	const std::string		str(buffer);
	std::istringstream	iss(str);
	std::string				line;
	
	while (std::getline(iss, line, '\r'))
	{
		if (!line.empty())
		{
			std::cout << "[" << YELLOW << line << RESET << "]" << std::endl;
			std::cout << "Sent by : " << CYAN << getUser(clientSockFd, irc_data)->getNickname() << END << std::endl;
			execute_command(line, clientSockFd, irc_data);
		}
		iss.ignore();
	}
}
