/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_topic.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 09:58:05 by ctchen            #+#    #+#             */
//   Updated: 2024/02/22 17:21:15 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "channel_parse.hpp"
#include "Reply.hpp"
#include "Notif.hpp"
#include "Error.hpp"

void	topic_change(std::string *str, int *clientSockFd, irc *irc_data)
{
	std::string						channel_name = word_picker(str, 2);
	std::list<User>::const_iterator	user = get_user((*clientSockFd), irc_data);

	if (user == irc_data->usersList.end())
	{
		Error ERR_NOSUCHNICK(401, "", "", "No such nick");
		ERR_NOSUCHNICK.to_client(*clientSockFd);
		return ;
	}
	if (channel_name.empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), channel_name, "");
		ERR_NEEDMOREPARAMS.to_client(*clientSockFd);
		return ;
	}
	std::string							arg = word_picker(str, 3);
	std::list<Channel>::iterator		channel = get_channel(channel_name, irc_data);
	if (channel == irc_data->channelList.end())
	{
		Error ERR_NOSUCHCHAN(403, channel_name, "", "No such chan");
		ERR_NOSUCHCHAN.to_client(*clientSockFd);
		return ;
	}
	std::list<std::string>::iterator	ch_user = channel->findUserinCh(user->getNickname());
	if (ch_user == channel->getUsersListEnd())
	{
		Error ERR_NOTONCHANNEL(442, "", channel_name,
							   "You are not on that channel");
		ERR_NOTONCHANNEL.to_client(*clientSockFd);
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
				ERR_CHANOPRIVSNEEDED.to_client(*clientSockFd);
				return ;
			}
			else
				channel->setTopic(arg);
		}
		else
			channel->setTopic(arg);
		Notif	notif(user->getNickname() + "!" + user->getUsername() + "@"
				+ user->getIp(), "TOPIC", channel_name, channel->getTopic());
		notif.to_all_others(*channel, *clientSockFd, irc_data->usersList);
		notif.to_client(*clientSockFd);
	}
	else
	{
		Reply RPL_NOTOPIC(331, user->getNickname() + " " + channel_name, "No topic is set");
		RPL_NOTOPIC.to_client(*clientSockFd);
	}
}
