/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_command.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 18:18:23 by ctchen            #+#    #+#             */
/*   Updated: 2024/02/09 14:57:07 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

bool	check_rights(std::list<User>::const_iterator user,
					std::list<Channel>::const_iterator chan)
{
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

std::string	word_picker(const std::string& str, unsigned int nb)
{
	std::istringstream	iss(str);
	std::string			word;
    unsigned int		i = 0;
    
	while (iss >> word)
	{
		i++;
		if (i == nb)
			return (word);
	}
	return ("");
}

void	channel_join(std::string str, int clientSockFd, irc *irc_data)
{
	std::list<User>::iterator		user = get_user(clientSockFd, irc_data);
	std::string 						channel_name = word_picker(str, 2);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);

	if (channel != irc_data->channelList.end()) // Si le channel existe on le rejoint
	{
		if (channel->getKey() != "")
		{
			// WIP => Demander la key, si invalide => Error
		}
		
		if (channel->getInviteMode() == true)
		{
			if (channel->checkInvite(user) == true) // Si user est bien invited
			{
				channel->deleteInvited(user); // On le supprime de la liste des invites
			}
			else
			{	
				return ; // WIP => Error ne peut pas rejoindre le channel 
			}
		}
		
		channel->addUser(user);

		// send notif a ts les users
		// send notif 4 messages
	}
	else // Sinon on le cree
	{
		Channel	new_channel(channel_name);
	
		new_channel.addUser(user);
		new_channel.addOperator(user);
		irc_data->channelList.push_back(new_channel);

		std::cout << "ici" << std::endl;
		printContainer(irc_data->channelList.begin()->getUsersList());
		std::cout << "ici" << std::endl;
		
		// send notif 4 messages
	}
}

void	topic_change(std::string str, int clientSockFd, irc *irc_data)
{
	std::string						channel_name = word_picker(str, 2);
	std::string						arg = word_picker(str, 3);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user = get_user(clientSockFd, irc_data);

	if (channel == irc_data->channelList.end())
	{
		//need error code
		return ;
	}
	if (channel->getTopicMode() == true) // Si seulement ops peuvent modifier topic
	{
		if (check_rights(user, channel) == false)
		{
			std::cerr << user->getNickname() << " cannot change the topic, he needs operator rights\n"; // Message a envoyer au client ? Retourner strings formatees
		}
		else
		{
			channel->setTopic(arg);
			std::cerr << user->getNickname() << " set the channel's topic to \"" << arg << "\"\n"; // Message a envoyer au client ? Retourner strings formatees
		}
	}
	else // Si tout le monde peut modifier le topic
	{
		channel->setTopic(arg);
		std::cerr << user->getNickname() << " set the channel's topic to \"" << arg << "\"\n";
	}
	std::string mode_reply = ":" + SERVER_NAME + " TOPIC abc\r\n";
	send(clientSockFd, mode_reply.c_str(), mode_reply.size(), 0);
}

template < typename T >
void	printContainer( T container )
{
	if (container.size() == 0)
	{
		std::cout << "Empty list !" << std::endl;
		return ;
	}
	
	typename T::const_iterator	it = container.begin();
	typename T::const_iterator	ite = container.end();

	for (; it != ite; it++)
	{
		std::cout << "[" << CYAN << it->getNickname() << RESET << "]" << std::endl;
	}
}

void	kick_user(std::string str, int clientSockFd, irc *irc_data)
{
	std::string 					channel_name = word_picker(str, 3);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user = get_user(clientSockFd, irc_data);
	std::list<User>::iterator 		it = channel->findUserinCh(user->getUsername());
	std::string						target = word_picker(str, 3);

	if (channel == irc_data->channelList.end())
	{
		//need error code
		return ;
	}
	if (it != channel->getUsersList().end())
	{
		if (check_rights(user, channel) == true)
		{
			channel->getUsersList().erase(it);
			std::cout << CYAN << target << RESET << " has been kicked !\n";	
			printContainer(channel->getUsersList());
		}
	}
	else
		std::cout << CYAN << target << RESET << " not in users list !\n";
	std::string mode_reply = ":" + SERVER_NAME + " KICK abc\r\n";
	send(clientSockFd, mode_reply.c_str(), mode_reply.size(), 0);
}

void	invite_user(std::string str, int clientSockFd, irc *irc_data)
{
	std::string 					channel_name = word_picker(str, 3);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user = get_user(clientSockFd, irc_data);
	std::list<User>::iterator 		it = channel->findUserinCh(user->getUsername());
	std::string						target = word_picker(str, 3);

	if (channel == irc_data->channelList.end())
	{
		//need error code
		return ;
	}
	if (it != channel->getUsersList().end())
	{
		if (channel->getInviteMode() == true && check_rights(user, channel) == true)
			channel->addUser(user);
		else
		{
			channel->getUsersList().push_back((*user));
			std::cout << CYAN << user->getNickname() << RESET
					  << " added to the Channel !" << std::endl;
			printContainer(channel->getUsersList());
		}
	}
	std::string mode_reply = ":" + SERVER_NAME + " INVITE abc\r\n";
	send(clientSockFd, mode_reply.c_str(), mode_reply.size(), 0);
}

std::string	word_skip_cut(std::string &str, unsigned long i)
{//Skip 1 mot + les espace qui suit et supprime+retourne le mot qui suit de str
	std::string	word;

	while (i < str.size() && str[i] != ' ')
		i++;
	while (i < str.size() && str[i] == ' ')
		i++;
	while (i < str.size() && str[i] != ' ')
	{
		word += str[i];
		str.erase(i, 1);
	}
	return (word);
}

void	mode_change(std::string str, int clientSockFd, irc *irc_data)
{
	std::string 					channel_name = word_picker(str, 3);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user = get_user(clientSockFd, irc_data);

	if (channel == irc_data->channelList.end())
	{
		//need error code
		return ;
	}
	//////////////////////////////////////////////////Tmp
	unsigned long					i = 0;			//
	while (i < str.size() && str[i] == ' ')			//
		i++;										//
	while (i < str.size() && str[i] != ' ')			//
		i++;										//
	while (i < str.size() && str[i] == ' ')			//
		i++;										//
	//////////////////////////////////////////////////
	if (check_rights(user, channel) == true)
	{
		bool	set = 0;
		for (; i < str.size(); i++)
		{
			switch (str[i])
			{
			case '+':
				set = 1;
				break;
			case '-':
				set = 0;
				break;
			case 'i':
			{
				channel->setInviteMode(set);
				break;
			}
			case 't':
				channel->setTopicMode(set);
				break;
			case 'k':
			{
				std::string word = word_skip_cut(str, i);
				if (set == 1)
					channel->setKey(word);
				else if (set == 0)
					channel->delKey();
				break;
			}
			case 'o':
			{
				std::string word = word_skip_cut(str, i);
				if (set == 1)
					channel->addOperator(user);
				else if (set == 0)
					channel->delOperator(user);
				break;
			}
			case 'l':
			{
				std::string word = word_skip_cut(str, i);
				char	*ptr;
				if (set == 1)
					channel->setUsersLimit(std::strtoul(word.c_str(), &ptr, 10));
				else if (set == 0)
					channel->setUsersLimit(0);
				break;
			}
			}
		}
	}
	std::string mode_reply = ":" + SERVER_NAME + " MODE +ik toto\r\n";
	send(clientSockFd, mode_reply.c_str(), mode_reply.size(), 0);
}
