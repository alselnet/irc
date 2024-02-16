// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   channel_command.cpp                                :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ctchen <ctchen@student.42.fr>              +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/02/08 18:18:23 by ctchen            #+#    #+#             //
//   Updated: 2024/02/16 16:01:21 by ctchen           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "irc.hpp"
#include "Reply.hpp"
#include "Notif.hpp"
#include "Error.hpp"

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

	if (channel_name.empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "JOIN needs parameter");
		ERR_NEEDMOREPARAMS.to_client(clientSockFd);
	}
	else if (channel_name == "#0")
	{
		for (std::list<Channel>::iterator it = irc_data->channelList.begin();
				 it != irc_data->channelList.end(); it++)
		{
			if (it->findUserinCh(user->getUsername()) != it->getUsersList().end())
			{
				it->delUser(channel->findUserinCh(
								get_user(clientSockFd, irc_data)->getUsername()));
				Notif	notif(user->getNickname() + "!" + user->getUsername() + "@"
							  + user->getIp(), "PART", it->getChName(), "");
				notif.to_client(clientSockFd);
			}
		}
		return ;
	}
	else if (channel != irc_data->channelList.end())
	{
		if (channel->getKey().empty() == 0 && channel->getKey() != key)
		{
			Error ERR_BADCHANNELKEY(475, user->getNickname(), channel_name,
									"This channel requires a password");
			ERR_BADCHANNELKEY.to_client(clientSockFd);
			return ;
		}
		else if (channel->getInviteMode() == true)
		{
			Error ERR_INVITEONLYCHAN(473, user->getNickname(), channel_name,
									"This channel is in invite only mode");
			ERR_INVITEONLYCHAN.to_client(clientSockFd);
			return ;
		}
		else
			channel->addUser(user);
	}
	else
	{//ERR NOSUCHCHANNEL impossible ?
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
		Reply RPL_TOPIC(332, user->getNickname() + " " + channel_name, channel->getTopic());
		RPL_TOPIC.to_client(clientSockFd);
		//RPL_TOPICTIME:333 pour indiquer l'user et le temps ou le topic est set?
	}
	if (channel->getUsersList().empty() == 0)
	{
		std::string userlistname;
		std::list<User> userlist = channel->getUsersList();
		for (std::list<User>::iterator it = userlist.begin(); it != userlist.end(); it++)
			userlistname += it->getNickname() += " ";
		userlistname.erase(userlistname.size() - 1);
		Reply RPL_NAMREPLY(353, user->getNickname() + " = " + channel_name, userlistname);
		RPL_NAMREPLY.to_client(clientSockFd);
		Reply RPL_ENDOFNAMES(366, user->getNickname() + " " + channel_name, "End of NAMES list");
		RPL_ENDOFNAMES.to_client(clientSockFd);
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
		std::cerr << "loool" << std::endl;
	}
}

void	channel_leave(std::string str, int clientSockFd, irc *irc_data)
{
	std::string 					channel_name = word_picker(str, 2);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user =
		channel->findUserinCh(get_user(clientSockFd, irc_data)->getUsername());

	if (channel_name.empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "JOIN needs parameter");
		ERR_NEEDMOREPARAMS.to_client(clientSockFd);
		return ;
	}
	else if (channel == irc_data->channelList.end())
	{
		Error ERR_NOSUCHCHANNEL(403, user->getNickname(), channel_name,
								"There is no such channel");
		ERR_NOSUCHCHANNEL.to_client(clientSockFd);
		return ;
	}
	else if (user == channel->getUsersList().end())
	{
		Error ERR_NOTONCHANNEL(442, user->getNickname(), channel_name,
							   "You are not on that channel");
		ERR_NOTONCHANNEL.to_client(clientSockFd);
		return ;
	}
	else
		channel->delUser(user);
	Notif	notif(user->getNickname() + "!" + user->getUsername() + "@"
				  + user->getIp(), "PART", channel_name, "");
	notif.to_client(clientSockFd);
}

void	topic_change(std::string str, int clientSockFd, irc *irc_data)
{//ERR_NEEDMOREPARAM impossible?
	std::string						channel_name = word_picker(str, 2);
	std::string						arg = word_picker(str, 3);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user =
		channel->findUserinCh(get_user(clientSockFd, irc_data)->getUsername());
//	std::list<User>::iterator		user = get_user(clientSockFd, irc_data);

	if (user == channel->getUsersList().end())
	{
		Error ERR_NOTONCHANNEL(442, user->getNickname(), channel_name,
							   "You are not on that channel");
		ERR_NOTONCHANNEL.to_client(clientSockFd);
		return ;
	}
	if (!arg.empty())
	{
		if (channel->getTopicMode() == true)
		{
			if (check_rights(user, channel) == false)
			{
				Error ERR_CHANOPRIVSNEEDED(482, user->getNickname(), channel_name,
										   "You are not an operator of this channel");
				ERR_CHANOPRIVSNEEDED.to_client(clientSockFd);
				return ;
			}
			else
				channel->setTopic(arg);
		}
		else
			channel->setTopic(arg);
		Reply RPL_TOPIC(332, user->getNickname() + " " + channel_name, channel->getTopic());
		RPL_TOPIC.to_client(clientSockFd);
	}
	else
	{
		Reply RPL_NOTOPIC(331, user->getNickname() + " " + channel_name, "No topic is set");
		RPL_NOTOPIC.to_client(clientSockFd);
	}
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
	std::list<User>::iterator		user =
		channel->findUserinCh(get_user(clientSockFd, irc_data)->getUsername());
//	std::list<User>::iterator		user = get_user(clientSockFd, irc_data);
	std::list<User>::iterator		target = channel->findUserinCh(word_picker(str, 3));
//	std::string						target = word_picker(str, 3);

	if (channel_name.empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "Kick needs parameter");
		ERR_NEEDMOREPARAMS.to_client(clientSockFd);
		return ;
	}
	else if (channel == irc_data->channelList.end())
	{
		Error ERR_NOSUCHCHANNEL(403, user->getNickname(), channel_name,
								"There is no such channel");
		ERR_NOSUCHCHANNEL.to_client(clientSockFd);
		return ;
	}
	else if (target == channel->getUsersList().end())
	{
		Error ERR_USERNOTINCHANNEL(441, user->getNickname(), channel_name,
							   "The user you are trying to kick is not in the channel");
		ERR_USERNOTINCHANNEL.to_client(clientSockFd);
		return ;
	}
	else if (user == channel->getUsersList().end())
	{
		Error ERR_NOTONCHANNEL(442, user->getNickname(), channel_name,
							   "You are not on that channel");
		ERR_NOTONCHANNEL.to_client(clientSockFd);
		return ;
	}
	else if (target != channel->getUsersList().end())
	{
		if (check_rights(user, channel) == true)
		{
			channel->getUsersList().erase(target);
			printContainer(channel->getUsersList());//check
		}
		else
		{
			Error ERR_NOTONCHANNEL(482, user->getNickname(), channel_name,
								   "You do not have operator privileges");
			ERR_NOTONCHANNEL.to_client(clientSockFd);
		}
	}
	Notif	notif(user->getNickname() + "!" + user->getUsername() + "@"
				  + user->getIp(), "KICK", channel_name, "");
	notif.to_client(clientSockFd);
}

void	invite_user(std::string str, int clientSockFd, irc *irc_data)
{
	std::string 					channel_name = word_picker(str, 2);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user = get_user(clientSockFd, irc_data);
	std::list<User>::iterator 		target = get_user_by_nick(word_picker(str, 3), irc_data);

	if (channel_name.empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "Invite needs parameter");
		ERR_NEEDMOREPARAMS.to_client(clientSockFd);
	}
	else if (user == channel->getUsersList().end())
	{
		Error ERR_NOTONCHANNEL(442, user->getNickname(), channel_name,
							   "You are not on that channel");
		ERR_NOTONCHANNEL.to_client(clientSockFd);
	}
//	if (channel == irc_data->channelList.end())
//	{//Numeric reply pas present sur la liste possible pour Invite
//		Reply reply(442, user->getNickname() + " " + channel_name,
//					"This channel does not exists");
//		reply.to_client(clientSockFd);
//		return ;
//	}
	else if (channel->findUserinCh(word_picker(str, 3)) != channel->getUsersList().end())
	{
		Error ERR_USERONCHANNEL(443, user->getNickname(), channel_name,
								"Target already in channel");
		ERR_USERONCHANNEL.to_client(clientSockFd);
	}
	else if (target == irc_data->usersList.end())
	{
		Error ERR_NOSUCHNICK(406, user->getNickname(), channel_name, "There is no such nickname");
		ERR_NOSUCHNICK.to_client(clientSockFd);
	}
	else
	{
		if (channel->getInviteMode() == true && check_rights(user, channel) == true)
		{
			channel->addUser(user);
			Notif	RPL_INVITING(user->getNickname() + "!" + user->getUsername() + "@"
								 + user->getIp(), "RPL_INVITING", word_picker(str, 3)
								 , "#" + channel_name);
			RPL_INVITING.to_client(clientSockFd);
//			Reply reply(341, user->getNickname() + " " + channel_name,
//						"User invited successfully");
//			reply.to_client(clientSockFd);
		}
		else
		{
			Error ERR_NOTONCHANNEL(482, user->getNickname(), channel_name,
								   "You do not have operator privileges");
			ERR_NOTONCHANNEL.to_client(clientSockFd);
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

std::string	active_mode(std::list<Channel>::iterator channel)
{
	std::string	active = "";

	if (channel->getInviteMode())
		active += "i";
	if (channel->getTopicMode())
		active += "t";
	if (!channel->getKey().empty())
		active += "k";
	if (!channel->getOperatorsList().empty())
		active += "o";
	if (channel->getUsersLimit() > 0)
		active += "l";
	if (active.size() > 0)
		return ("+" + active);
	return (active);
}

void	mode_channel(std::string str, int clientSockFd, irc *irc_data, std::string channel_name)
{
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user = get_user(clientSockFd, irc_data);
	std::string						flags = word_picker(str, 3);
	unsigned long					i = 0;

//	Notif notif(SERVER_NAME, "324", user->getNickname()  + " #" + channel_name,
//				active_mode(channel));
//	notif.to_client(clientSockFd);
	if (channel == irc_data->channelList.end())
	{
		Error ERR_NOTONCHANNEL(442, user->getNickname(), channel_name,
					"This channel does not exists");
		ERR_NOTONCHANNEL.to_client(clientSockFd);
		return ;
	}
	else if (channel_name.empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), channel_name, "Not enough parameters");
		ERR_NEEDMOREPARAMS.to_client(clientSockFd);
		return ;
	}
	else if (user->getOperator() == 0 &&
			 channel->findUserinCh(user->getUsername()) == channel->getUsersList().end())
	{
		Error ERR_USERNOTINCHANNEL(441, user->getNickname(), channel_name,
								   "You are not in the channel");
		ERR_USERNOTINCHANNEL.to_client(clientSockFd);
		return ;
	}
	//ERR_NOCHANMODES = 477 if channel don't support modes
	if (check_rights(user, channel) == true || user->getOperator() == 1)
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
				std::string key = word_skip_cut(str, i);
				if (!channel->getKey().empty()
					&& channel->getKey() != key)
				{
					Error ERR_KEYSET(467, user->getNickname(), channel_name,
									 "The key is already set");
					ERR_KEYSET.to_client(clientSockFd);
					return;
				}
				option += 'k';
				if (set == 1)
					channel->setKey(key);
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
			default :
			{
				Error ERR_UNKNOWNMODE(472, user->getNickname(), channel_name, "Unknown mode");
				ERR_UNKNOWNMODE.to_client(clientSockFd);
			}
			}
		}
		Notif	notif(user->getNickname() + "!" + user->getUsername() + "@"
					  + user->getIp(), "MODE", channel_name, option);
		notif.to_client(clientSockFd);
	}
	else
	{
		Error ERR_CHANOPRIVSNEEDED(482, user->getNickname(), channel_name,
					"You do not have operator privileges");
		ERR_CHANOPRIVSNEEDED.to_client(clientSockFd);
	}
}

void	mode_user(std::string str, int clientSockFd, irc *irc_data, std::string nickname)
{
	std::list<User>::iterator		user = get_user(clientSockFd, irc_data);
	std::string						active_umode = "+";
	std::string						modes = word_picker(str, 3);

//	active_umode += "w";
	if (nickname.empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "MODE needs parameter");
		ERR_NEEDMOREPARAMS.to_client(clientSockFd);
		return;
	}
	else if (nickname != user->getNickname())
	{//can change if serv op ?
		Error ERR_USERSDONTMATCH(502, user->getNickname(), "",
								 "Can't change mode for other users");
		ERR_USERSDONTMATCH.to_client(clientSockFd);
		return ;
	}
	//ERR_UMODEUNKNOWNFLAG = 501
	if (user->getOperator())
		active_umode += "o";
	Reply RPL_UMODEIS(221, user->getNickname(), active_umode);
	RPL_UMODEIS.to_client(clientSockFd);
}

void	mode_change(std::string str, int clientSockFd, irc *irc_data)
{
	std::string	second = word_picker(str, 2);

	if (second[0] == '#')
		mode_channel(str, clientSockFd, irc_data, second);
	else
		mode_user(str, clientSockFd, irc_data, second);
}
