// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   channel_command.cpp                                :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ctchen <ctchen@student.42.fr>              +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/02/08 18:18:23 by ctchen            #+#    #+#             //
//   Updated: 2024/02/14 00:40:39 by ctchen           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "irc.hpp"
#include "Reply.hpp"
#include "Notif.hpp"

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

void	channel_pick(int clientSockFd, irc *irc_data, std::string channel_name, std::string key)
{//chanfull + banned + too much chan a check/faire
	std::list<User>::iterator		user = get_user(clientSockFd, irc_data);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);

//	std::cerr << "DEBUG: channel_pick start" << std::endl;
	if (channel != irc_data->channelList.end())
	{
		if (channel->getKey().empty() == 0 && channel->getKey() != key)
		{
			Reply reply(475, user->getNickname() + " " + channel_name,
						"This channel requires a password");
			reply.to_client(clientSockFd);
			return ;
		}
		else if (channel->getInviteMode() == true)
		{
			Reply reply(473, user->getNickname() + " " + channel_name,
						"This channel is in invite only mode");
			reply.to_client(clientSockFd);
			return ;
		}
		else
			channel->addUser(user);
	}
	else
	{
		Channel newchannel(channel_name);
		if (key != "")
			newchannel.setKey(key);
		newchannel.addUser(user);
		newchannel.addOperator(user);
		irc_data->channelList.push_back(newchannel);
		channel = get_channel(channel_name, irc_data);
	}
	Notif notif(user->getNickname() + "!" + user->getUsername() + "@"
				  + user->getIp(), "JOIN", channel_name, "");
	notif.to_client(clientSockFd);
	if (channel->getTopic().empty() == 0)
	{
		Notif topic("localhost", "332", user->getNickname() + " #" + channel_name
					, ":" + channel->getTopic());
		topic.to_client(clientSockFd);
		//RPL_TOPICTIME:333 pour indiquer l'user et le temps ou le topic est set?
	}
	if (channel->getUsersList().empty() == 0)
	{
		std::string userlistname;
		std::list<User> userlist = channel->getUsersList();
		for (std::list<User>::iterator it = userlist.begin(); it != userlist.end(); it++)
			userlistname += it->getNickname() += " ";
		userlistname.erase(userlistname.size() - 1);
		Notif topic("localhost", "353", user->getNickname() + "= #" + channel_name,
					":" +  userlistname);
		topic.to_client(clientSockFd);
	}
//	std::cerr << "DEBUG: channel_pick ended successfully" << std::endl;
}

void	channel_join(std::string str, int clientSockFd, irc *irc_data)
{
	std::string	channels = word_picker(str, 2);
	std::string	keylist = word_picker(str, 3);
	int			ch_count = 0;
	int			key_count = 0;

	for (unsigned long i = 0; i < channels.size(); i++)
	{
		if (channels[i] == ',')
		{
			channels[i] = ' ';
			ch_count++;
		}
	}
	for (unsigned long i = 0; i < keylist.size(); i++)
	{
		if (keylist[i] == ',')
		{
			keylist[i] = ' ';
			key_count++;
		}
	}
	for (int i = 0; i <= ch_count; i++)
	{
		std::string channel_name = word_picker(channels, i + 1);
		std::string	key = word_picker(keylist, i + 1);
		channel_pick(clientSockFd, irc_data, channel_name, key);
	}
}

void	channel_leave(std::string str, int clientSockFd, irc *irc_data)
{
	std::string 					channel_name = word_picker(str, 2);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user =
		channel->findUserinCh(get_user(clientSockFd, irc_data)->getUsername());

//	std::cerr << "DEBUG: channel_leave start" << std::endl;
	if (channel == irc_data->channelList.end())
	{
		Reply	reply(403, user->getNickname(), channel_name);
		reply.to_client(clientSockFd);
		return ;
	}
	else
		channel->delUser(user);
	Notif	notif(user->getNickname() + "!" + user->getUsername() + "@"
				  + user->getIp(), "PART", channel_name, "");
	notif.to_client(clientSockFd);
//	std::cerr << "DEBUG: channel_leave ended successfully" << std::endl;
}

void	topic_change(std::string str, int clientSockFd, irc *irc_data)
{
	std::string						channel_name = word_picker(str, 2);
	std::string						arg = word_picker(str, 3);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user = get_user(clientSockFd, irc_data);

	if (channel == irc_data->channelList.end())
	{
		Reply reply(442, user->getNickname() + " " + channel_name,
					"This channel does not exists");
		reply.to_client(clientSockFd);
		return ;
	}
	if (channel->getTopicMode() == true)
	{
		if (check_rights(user, channel) == false)
		{
			Reply reply(482, user->getNickname() + " " + channel_name,
						"You are not an operator of this channel");
			reply.to_client(clientSockFd);
			return ;
		}
		else
			channel->setTopic(arg);
	}
	else
		channel->setTopic(arg);
	Reply reply(332, user->getNickname() + " " + channel_name, channel->getTopic());
	reply.to_client(clientSockFd);
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
	std::string 					channel_name = word_picker(str, 2);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user = get_user(clientSockFd, irc_data);
	std::list<User>::iterator 		it = channel->findUserinCh(user->getUsername());
	std::string						target = word_picker(str, 3);

	if (channel == irc_data->channelList.end())
	{
		Reply reply(442, user->getNickname() + " " + channel_name,
					"This channel does not exists");
		reply.to_client(clientSockFd);
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
		else
		{
			Reply reply(482, user->getNickname() + " " + channel_name,
						"You do not have operator privileges");
			reply.to_client(clientSockFd);
		}
	}
	else
		std::cout << CYAN << target << RESET << " not in users list !\n";
	Notif	notif(user->getNickname() + "!" + user->getUsername() + "@"
				  + user->getIp(), "KICK", channel_name, "");
	notif.to_client(clientSockFd);
}

void	invite_user(std::string str, int clientSockFd, irc *irc_data)
{
	std::string 					channel_name = word_picker(str, 2);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user = get_user(clientSockFd, irc_data);
	std::list<User>::iterator 		it = channel->findUserinCh(user->getUsername());
	std::string						target = word_picker(str, 3);

	if (channel == irc_data->channelList.end())
	{
		Reply reply(442, user->getNickname() + " " + channel_name,
					"This channel does not exists");
		reply.to_client(clientSockFd);
		return ;
	}
	else if (it != channel->getUsersList().end())
	{
		if (channel->getInviteMode() == true && check_rights(user, channel) == true)
			channel->addUser(user);
		else
		{
			Reply reply(482, user->getNickname() + " " + channel_name,
						"You do not have operator privileges");
			reply.to_client(clientSockFd);
		}
	}
	Notif	notif(user->getNickname() + "!" + user->getUsername() + "@"
				  + user->getIp(), "INVITE", channel_name, "");
	notif.to_client(clientSockFd);
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
	std::string 					channel_name = word_picker(str, 2);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user = get_user(clientSockFd, irc_data);
	std::string						flags = word_picker(str, 3);
	unsigned long					i = 0;

	if (channel == irc_data->channelList.end())
	{
		Reply reply(442, user->getNickname() + " " + channel_name,
					"This channel does not exists");
		reply.to_client(clientSockFd);
		return ;
	}
	else if (check_rights(user, channel) == true)
	{
		bool		set = 0;
		std::string	option = "";
		for (; i < flags.size(); i++)
		{
			switch (flags[i])
			{
			case '+':
			{
				option += '+';
				set = 1;
				break;
			}
			case '-':
			{
				option += '-';
				set = 0;
				break;
			}
			case 'i':
			{
				option += 'i';
				channel->setInviteMode(set);
				break;
			}
			case 't':
			{
				option += 't';
				channel->setTopicMode(set);
				break;
			}
			case 'k':
			{
				option += 'k';
				std::string word = word_skip_cut(str, i);
				if (set == 1)
					channel->setKey(word);
				else if (set == 0)
					channel->delKey();
				break;
			}
			case 'o':
			{
				option += 'o';
				std::string word = word_skip_cut(str, i);
				if (set == 1)
					channel->addOperator(user);
				else if (set == 0)
					channel->delOperator(user);
				break;
			}
			case 'l':
			{
				option += 'l';
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
		Notif	notif(user->getNickname() + "!" + user->getUsername() + "@"
					  + user->getIp(), "MODE", channel_name, option);
		notif.to_client(clientSockFd);
	}
	else
	{
		Reply reply(482, user->getNickname() + " " + channel_name,
					"You do not have operator privileges");
		reply.to_client(clientSockFd);
	}
}
