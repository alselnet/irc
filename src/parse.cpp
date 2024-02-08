/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:22:50 by jthuysba          #+#    #+#             */
//   Updated: 2024/02/08 12:16:42 by ctchen           ###   ########.fr       //
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

//Pt besoin d'une fonc pour recuperer tout les channels ou l'user est present

std::list<Channel>::iterator getChannel(std::string chan_name, irc *irc_data )
{
	if (chan_name[0] != '#')
		return (irc_data->channelList.end());
	std::list<Channel>::iterator	it = irc_data->channelList.begin();
	std::list<Channel>::iterator	ite = irc_data->channelList.end();

	for (; it != ite; it++)
	{
		if (it->getChName() == chan_name)
			return (it);
	}
	return (ite);
}

bool	checkRights(std::list<User>::const_iterator user,
					std::list<Channel>::const_iterator chan)
{//verifie si l'user est un op server puis op channel
	if (user->getOperator() == true)
		return true;
	else
	{
		for (std::list<User>::const_iterator it = chan->getOperatorsList().begin();
			 it != chan->getOperatorsList().end(); ++it)
		{
			if (it->getUsername() == user->getUsername())
				return true;
		}
	}
	return false;
}

void	execute_channel_cmd(std::string str, int clientSockFd, irc *irc_data)
{
	std::string			cmd;
	std::string			second;
	std::string			third;
	std::istringstream	iss(str);
	std::string			username = getUser(clientSockFd, irc_data)->getUsername();

	iss >> cmd;//tmp
	iss >> second;//tmp
	iss >> third;//tmp
	if (getChannel(second, irc_data) != irc_data->channelList.end())
{
		bool	is_op = checkRights(getUser(clientSockFd, irc_data),
									getChannel(second, irc_data));
		if (cmd == "MODE")
		{
			std::string mode_reply = ":" + SERVER_NAME + " MODE abc +i\r\n";
			getChannel(second, irc_data)->modeChange(getUser(clientSockFd, irc_data),
														 cmd, is_op);
			send(clientSockFd, mode_reply.c_str(), mode_reply.size(), 0);
		}
		else if (cmd == "TOPIC")
		{
		 	getChannel(second, irc_data)->changeTopic(
				getUser(clientSockFd, irc_data)->getNickname(), third, is_op);
		}
		else if (cmd == "KICK")
		{
			getChannel(second, irc_data)->kickUser(third, is_op);
		}
		else if (cmd == "INVITE")
		{
			getChannel(second, irc_data)->inviteUser
				(getUser(clientSockFd, irc_data), third, is_op);
		}
	}
}

// Execute la commande dans str
void	execute_command( std::string str, int clientSockFd, irc * irc_data )
{
	std::string			cmd;
	std::string			second;
	std::istringstream	iss(str);

	iss >> cmd;
	iss >> second;
	//std::cerr << "str:cmd = [" << str << "] [" << cmd << "]" << std::endl;
	// if (cmd == "CAP")
	// {
		// WIP => Gerer capacite ?
	// }
	if (cmd == "join")
		join_channel(second, clientSockFd, irc_data);
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
	else
		execute_channel_cmd(str, clientSockFd, irc_data);
	// WIP => Toutes les autres commandes a ajoute
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
