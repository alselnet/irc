/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:22:50 by jthuysba          #+#    #+#             */
/*   Updated: 2024/02/06 13:33:54 by aselnet          ###   ########.fr       */
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

std::list<Channel>::iterator getChannel( std::string username, irc * irc_data )
{
	std::list<Channel>::iterator	it = irc_data->channelList.begin();
	std::list<Channel>::iterator	ite = irc_data->channelList.end();

	for (; it != ite; it++)
	{
		if (it->findUserI(username, irc_data->usersList) != irc_data->usersList.end())
			return (it);
	}
	return (ite);
}

// Execute la commande dans str
void	execute_command( std::string str, int clientSockFd, irc * irc_data )
{
	std::string	cmd;
	std::istringstream	iss(str);
	std::string	username = getUser(clientSockFd, irc_data)->getUsername();
	
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
		if (getUser(clientSockFd, irc_data)->getNickname().empty())
		{
			getUser(clientSockFd, irc_data)->setNickname(nick);
			handshake_replies(clientSockFd, getUser(clientSockFd, irc_data)->getNickname());
		}
		else
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
	else if (cmd == "MODE")
	{
		std::string mode_reply = ":" + SERVER_NAME + " MODE abc +i\r\n";
		getChannel(username, irc_data)->findUserinCh(username);
		send(clientSockFd, mode_reply.c_str(), mode_reply.size(), 0);
	}
	else if (cmd == "TOPIC")
	{
		getChannel(username, irc_data)->changeTopic(
			getChannel(username, irc_data)->findUserinCh(username), str);
	}
//	else if (cmd == "KICK")
//	{//Readaptation en cours
//		getChannel(username, irc_data)->kickUser(getUser(clientSockFd, irc_data), target);
//	}
//	else if (cmd == "INVITE")
//	{Readaptation en cours
//		getChannel()->addUser(getUser(clientSockFd, irc_data), target);
//	}
	// WIP => Toutes les autres commandes a ajouterelse if (!cmd.compare("MODE"))
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
