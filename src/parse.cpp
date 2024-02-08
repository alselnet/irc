/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/29 17:22:50 by jthuysba          #+#    #+#             */
/*   Updated: 2024/02/08 16:41:19 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/Channel.hpp"
# include "../include/User.hpp"
# include "../include/notif.hpp"

// Return le sockFd du user correspondant au username
std::list<User>::iterator get_user_by_nick( std::string nickname, irc * irc_data )
{
	std::list<User>::iterator	it = irc_data->usersList.begin();
	std::list<User>::iterator	ite = irc_data->usersList.end();

	for (; it != ite; it++)
	{
		if (it->getNickname() == nickname)
			return (it);
	}
	// WIP => Gerer erreurs si fd non present
	return (ite);
}

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
	// WIP => Gerer erreurs si fd non present
	return (ite);
}

void	private_msg( std::string str, irc * irc_data, int clientSockFd)
{
	std::istringstream	iss(str);
	std::string				target;
	std::string				dump;
	std::string				text;
	
	iss >> target;
	std::getline(iss, text);

	if (*target.begin() == '#') // Target est un channel
	{
		std::string	channel_name = target;
		
		channel_name.erase(0);
		
		std::list<User>::iterator	origin_user = getUser(clientSockFd, irc_data);
		std::list<Channel>::iterator	target_channel = getChannel(channel_name, irc_data);
		
		std::string	id_string = origin_user->getNickname() + "!" + "user" + "@" + origin_user->getIp(); // WIP => Username and hostname to get
		notif			message_to_send(id_string, "PRIVMSG", target, text);
		
		message_to_send.to_all(target_channel->getUsersList());
	}
	else // Target est un user
	{
		std::list<User>::iterator	origin_user = getUser(clientSockFd, irc_data);
		std::list<User>::iterator	targetUser = get_user_by_nick(target, irc_data);
		
		std::string	id_string = origin_user->getNickname() + "!" + "user" + "@" + origin_user->getIp(); // WIP => Username and hostname to get
		notif			message_to_send(id_string, "PRIVMSG", target, text);
		
		message_to_send.to_client(targetUser->getSockFd());
	}
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
	bool	ch_found;
	bool	is_op;

	iss >> cmd;//tmp
	iss >> second;//tmp
	iss >> third;//tmp
//	std::cout << "cmd = [" << cmd << "]" << std::endl;
//	std::cout << "sec = [" << second << "]" << std::endl;
//	std::cout << "thi = [" << third << "]" << std::endl;
	if (getChannel(second, irc_data) != irc_data->channelList.end())
	{
		ch_found = 1;
		is_op = checkRights(getUser(clientSockFd, irc_data), getChannel(second, irc_data));
	}
	else
	{
		ch_found = 0;
		is_op = getUser(clientSockFd, irc_data)->getOperator();
	}
	if (cmd == "MODE")
	{
		std::string mode_reply = ":" + SERVER_NAME + " MODE +ik toto\r\n";
		if (ch_found == 1)
			getChannel(second, irc_data)->modeChange(getUser(clientSockFd, irc_data), cmd, is_op);
		send(clientSockFd, mode_reply.c_str(), mode_reply.size(), 0);
	}
	else if (cmd == "TOPIC")
	{
		std::string mode_reply = ":" + SERVER_NAME + " TOPIC abc\r\n";
		if (ch_found == 1)
			getChannel(second, irc_data)->changeTopic(
				getUser(clientSockFd, irc_data)->getNickname(), third, is_op);
		send(clientSockFd, mode_reply.c_str(), mode_reply.size(), 0);
	}
	else if (cmd == "KICK")
	{
		std::string mode_reply = ":" + SERVER_NAME + " KICK abc\r\n";
		if (ch_found == 1)
			getChannel(second, irc_data)->kickUser(third, is_op);
		send(clientSockFd, mode_reply.c_str(), mode_reply.size(), 0);
	}
	else if (cmd == "INVITE")
	{
		std::string mode_reply = ":" + SERVER_NAME + " INVITE abc\r\n";
		if (ch_found == 1)
			getChannel(second, irc_data)->inviteUser
				(getUser(clientSockFd, irc_data), third, is_op);
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
		if (getUser(clientSockFd, irc_data)->getNickname().empty())
		{
			getUser(clientSockFd, irc_data)->setNickname(nick);
			handshake_replies(clientSockFd, getUser(clientSockFd, irc_data)->getNickname());
		}
		else
			getUser(clientSockFd, irc_data)->setNickname(nick);

	}
//	else if (cmd == "USER")
//	{
		// WIP => Coder une fonction qui gere "USER <username> <username> <ip> :<realname>" & "userhost <username>" (a voir)
//	}
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
