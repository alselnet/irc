/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_topic.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 09:58:05 by ctchen            #+#    #+#             */
/*   Updated: 2024/02/22 16:45:33 by jthuysba         ###   ########.fr       */
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
	std::string						nickname = get_user((*clientSockFd), irc_data)->getNickname();
	
	if (channel_name.empty())
	{
		Error ERR_NEEDMOREPARAMS(461, nickname, channel_name, "");
		ERR_NEEDMOREPARAMS.to_client(*clientSockFd);
		return ;
	}
	
	std::string						arg = word_picker(str, 3);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user = channel->findUserinCh(nickname);

	if (user == channel->getUsersListEnd())
	{
		Error ERR_NOTONCHANNEL(442, user->getNickname(), channel_name,
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
		notif.to_all_others(*channel, *clientSockFd);
		notif.to_client(*clientSockFd);
	}
	else
	{
		Reply RPL_NOTOPIC(331, user->getNickname() + " " + channel_name, "No topic is set");
		RPL_NOTOPIC.to_client(*clientSockFd);
	}
}
