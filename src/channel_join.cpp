// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   channel_join.cpp                                   :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ctchen <ctchen@student.42.fr>              +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/02/19 09:56:07 by ctchen            #+#    #+#             //
//   Updated: 2024/02/19 10:24:50 by ctchen           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "irc.hpp"
#include "channel_parse.hpp"
#include "Reply.hpp"
#include "Notif.hpp"
#include "Error.hpp"

void	channel_pick(int *clientSockFd, irc *irc_data, std::string *channel_name, std::string *key)
{//chanfull + banned + too much chan a check/faire
	std::list<User>::iterator		user = get_user((*clientSockFd), irc_data);
	std::list<Channel>::iterator	channel = get_channel((*channel_name), irc_data);

	if (channel_name->empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "JOIN needs parameter");
		ERR_NEEDMOREPARAMS.to_client(*clientSockFd);
	}
	/*
	else if (channel_name == "#0")
	{
		for (std::list<Channel>::iterator it = irc_data->channelList.begin();
				 it != irc_data->channelList.end(); it++)
		{
			if (it->findUserinCh(user->getNickname()) != it->getUsersList().end())
			{
				it->delUser(channel->findUserinCh(
								get_user(clientSockFd, irc_data)->getNickname()));
				Notif	notif(user->getNickname() + "!" + user->getUsername() + "@"
							  + user->getIp(), "PART", it->getChName(), "");
				notif.to_client(clientSockFd);
			}
		}
		return ;
	}
	*/
	else if (channel != irc_data->channelList.end())
	{
		if (channel->getKey().empty() == 0 && channel->getKey() != (*key))
		{
			Error ERR_BADCHANNELKEY(475, user->getNickname(), (*channel_name),
									"This channel requires a password");
			ERR_BADCHANNELKEY.to_client(*clientSockFd);
			return ;
		}
		else if (channel->getInviteMode() == true
				 && channel->checkInvite(user->getNickname()) == 0)
		{
			Error ERR_INVITEONLYCHAN(473, user->getNickname(), (*channel_name),
									"This channel is in invite only mode");
			ERR_INVITEONLYCHAN.to_client(*clientSockFd);
			return ;
		}
		else
		{
			if (channel->getInviteMode() == true
				&& channel->checkInvite(user->getNickname()) == 1)
				channel->deleteInvited(user->getNickname());
			channel->addUser(user);
		}
	}
	else
	{//ERR NOSUCHCHANNEL impossible ?
		Channel newchannel((*channel_name));
		if ((*key) != "")
			newchannel.setKey(*key);
		newchannel.addUser(user);
		newchannel.addOperator(user->getNickname());
		irc_data->channelList.push_back(newchannel);
		channel = get_channel((*channel_name), irc_data);
	}
	Notif notif(user->getNickname() + "!" + user->getUsername() + "@"
				+ user->getIp(), "JOIN", (*channel_name), "");
	notif.to_client(*clientSockFd);
	if (channel->getTopic().empty() == 0)
	{
		Reply RPL_TOPIC(332, user->getNickname() + " " + (*channel_name), channel->getTopic());
		RPL_TOPIC.to_client(*clientSockFd);
		//RPL_TOPICTIME:333 pour indiquer l'user et le temps ou le topic est set?
	}
	if (channel->getUsersList().empty() == 0)
	{
		std::string userlistname;
		std::list<User> userlist = channel->getUsersList();
		for (std::list<User>::iterator it = userlist.begin(); it != userlist.end(); it++)
			userlistname += channel->getChanOperatorName(it->getNickname()) + " ";
		userlistname.erase(userlistname.size() - 1);
		Reply RPL_NAMREPLY(353, channel->getChanOperatorName(user->getNickname())
						   + " = " + (*channel_name), userlistname);
		RPL_NAMREPLY.to_client(*clientSockFd);
		Reply RPL_ENDOFNAMES(366, channel->getChanOperatorName(user->getNickname())
							 + " " + (*channel_name), "End of NAMES list");
		RPL_ENDOFNAMES.to_client(*clientSockFd);
	}
//	std::cerr << "DEBUG: channel_pick ended successfully" << std::endl;
}

void	channel_join(std::string *str, int *clientSockFd, irc *irc_data)
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
		std::string channel_name = word_picker(&channels, i + 1);
		std::string	key = word_picker(&keylist, i + 1);
		channel_pick(clientSockFd, irc_data, &channel_name, &key);
	}
}
