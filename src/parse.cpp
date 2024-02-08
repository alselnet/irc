/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:22:50 by jthuysba          #+#    #+#             */
/*   Updated: 2024/02/08 18:08:53 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/Channel.hpp"
# include "../include/User.hpp"
# include "../include/Notif.hpp"

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
	std::string			username = get_user(clientSockFd, irc_data)->getUsername();
	bool	ch_found;
	bool	is_op;

	iss >> cmd;//tmp
	iss >> second;//tmp
	iss >> third;//tmp
//	std::cout << "cmd = [" << cmd << "]" << std::endl;
//	std::cout << "sec = [" << second << "]" << std::endl;
//	std::cout << "thi = [" << third << "]" << std::endl;
	if (get_channel(second, irc_data) != irc_data->channelList.end())
	{
		ch_found = 1;
		is_op = checkRights(get_user(clientSockFd, irc_data), get_channel(second, irc_data));
	}
	else
	{
		ch_found = 0;
		is_op = get_user(clientSockFd, irc_data)->getOperator();
	}
	if (cmd == "MODE")
	{
		std::string mode_reply = ":" + SERVER_NAME + " MODE +ik toto\r\n";
		if (ch_found == 1)
			get_channel(second, irc_data)->modeChange(get_user(clientSockFd, irc_data), cmd, is_op);
		send(clientSockFd, mode_reply.c_str(), mode_reply.size(), 0);
	}
	else if (cmd == "TOPIC")
	{
		std::string mode_reply = ":" + SERVER_NAME + " TOPIC abc\r\n";
		if (ch_found == 1)
			get_channel(second, irc_data)->changeTopic(
				get_user(clientSockFd, irc_data)->getNickname(), third, is_op);
		send(clientSockFd, mode_reply.c_str(), mode_reply.size(), 0);
	}
	else if (cmd == "KICK")
	{
		std::string mode_reply = ":" + SERVER_NAME + " KICK abc\r\n";
		if (ch_found == 1)
			get_channel(second, irc_data)->kickUser(third, is_op);
		send(clientSockFd, mode_reply.c_str(), mode_reply.size(), 0);
	}
	else if (cmd == "INVITE")
	{
		std::string mode_reply = ":" + SERVER_NAME + " INVITE abc\r\n";
		if (ch_found == 1)
			get_channel(second, irc_data)->inviteUser
				(get_user(clientSockFd, irc_data), third, is_op);
		send(clientSockFd, mode_reply.c_str(), mode_reply.size(), 0);
	}
	else if (cmd == "join")
		join_channel(third, clientSockFd, irc_data);
}

// Execute la commande dans str
void	execute_command( std::string str, int clientSockFd, irc * irc_data )
{
	std::string			cmd;
	std::istringstream	iss(str);

	iss >> cmd;
	//std::cerr << "str:cmd = [" << str << "] [" << cmd << "]" << std::endl;
	// if (cmd == "CAP")
	// {
		// WIP => Gerer capacite ?
	// }
	if (cmd == "NICK")
	{
		// WIP => Check si nick deja utilise
		std::string	nick;
		
		iss >> nick;
		if (get_user(clientSockFd, irc_data)->getNickname().empty())
		{
			get_user(clientSockFd, irc_data)->setNickname(nick);
			handshake_replies(clientSockFd, get_user(clientSockFd, irc_data)->getNickname());
		}
		else
			get_user(clientSockFd, irc_data)->setNickname(nick);
	}
	else if (cmd == "PRIVMSG")
	{
		std::string	args;
		std::getline(iss, args);

		private_msg(args, irc_data, clientSockFd);
	}
	else if (cmd == "PING")
	{
		pong(clientSockFd);
	}
	else
		execute_channel_cmd(str, clientSockFd, irc_data);
//	else if (cmd == "USER")
//	{
		// WIP => Coder une fonction qui gere "USER <username> <username> <ip> :<realname>" & "userhost <username>" (a voir)
//	}
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
			std::cout << "Sent by : " << CYAN << get_user(clientSockFd, irc_data)->getNickname() << END << std::endl;
			execute_command(line, clientSockFd, irc_data);
		}
		iss.ignore();
	}
}
