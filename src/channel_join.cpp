/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_join.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 09:56:07 by ctchen            #+#    #+#             */
//   Updated: 2024/02/21 17:39:32 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "channel_parse.hpp"
#include "Reply.hpp"
#include "Notif.hpp"
#include "Error.hpp"

void	channel_pick(int *clientSockFd, irc *irc_data, std::string *channel_name, std::string *key, std::list<User>::iterator user)
{
	std::list<Channel>::iterator	channel = get_channel((*channel_name), irc_data);

	/*
	if (channel_name == "#0")
	{
		for (std::list<Channel>::iterator it = irc_data->channelList.begin();
				 it != irc_data->channelList.end(); it++)
		{
			if (it->findUserinCh(user->getNickname()) != it->getUsersListEnd())
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
	if (channel != irc_data->channelList.end())
	{
		if (channel->getKey().empty() == 0 && channel->getKey() != (*key)
			&& channel->checkInvite(user->getNickname()) == 0)
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
		else if (channel->getUsersLimit() > 0 && channel->getUsersCount()
				 >= channel->getUsersLimit())
		{
			Error ERR_CHANNELISFULL(471, user->getNickname(), (*channel_name),
				"");
			ERR_CHANNELISFULL.to_client(*clientSockFd);
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
	
	user->addChannel(*channel_name);
	
	Notif notif(user->getNickname() + "!" + user->getUsername() + "@"
				+ user->getIp(), "JOIN", (*channel_name), "");
	notif.to_client(*clientSockFd); // WIP a send to all
	notif.to_all_others(*channel, *clientSockFd);
	if (!(channel->usersListEmpty()))
	{
		std::string userlistname;
		// std::list<User> userlist = channel->getUsersList();
		for (std::list<User>::const_iterator it = channel->getUsersListBegin(); it != channel->getUsersListEnd(); it++)
			userlistname += channel->getChanOperatorName(it->getNickname()) + " ";
		userlistname.erase(userlistname.size() - 1);
		Reply RPL_NAMREPLY(353, channel->getChanOperatorName(user->getNickname())
						   + " = " + (*channel_name), userlistname);
		RPL_NAMREPLY.to_client(*clientSockFd);
		Reply RPL_ENDOFNAMES(366, channel->getChanOperatorName(user->getNickname())
							 + " " + (*channel_name), "End of NAMES list");
		RPL_ENDOFNAMES.to_client(*clientSockFd);
	}
	if (channel->getTopic().empty() == 0)
	{
		Reply RPL_TOPIC(332, user->getNickname() + " " + (*channel_name) + " " + channel->getTopic(), "");
		RPL_TOPIC.to_client(*clientSockFd);
		//RPL_TOPICTIME:333 pour indiquer l'user et le temps ou le topic est set?
	}
//	std::cerr << "DEBUG: channel_pick ended successfully" << std::endl;
}

void	channel_join(std::string *str, int *clientSockFd, irc *irc_data)
{
	std::string	channels = word_picker(str, 2);
	std::string	keylist = word_picker(str, 3);
	std::list<User>::iterator		user = get_user((*clientSockFd), irc_data);

	if (user == irc_data->usersList.end())
	{
		std::cerr << "Error: no user is impossible" << std::endl;
		return ;
	}
	if (channels.empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "JOIN needs parameter");
		ERR_NEEDMOREPARAMS.to_client(*clientSockFd);
		return ;
	}	
	int			ch_count = word_comma_replace(&channels);
	word_comma_replace(&keylist);
	for (int i = 0; i <= ch_count; i++)
	{
		std::string channel_name = word_picker(&channels, i + 1);
		std::string	key = word_picker(&keylist, i + 1);
		channel_pick(clientSockFd, irc_data, &channel_name, &key, user);
	}
}
