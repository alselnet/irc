// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   channel_topic.cpp                                  :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ctchen <ctchen@student.42.fr>              +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/02/19 09:58:05 by ctchen            #+#    #+#             //
//   Updated: 2024/02/19 13:33:25 by ctchen           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#include "irc.hpp"
#include "channel_parse.hpp"
#include "Reply.hpp"
#include "Notif.hpp"
#include "Error.hpp"

void	topic_change(std::string *str, int *clientSockFd, irc *irc_data)
{//ERR_NEEDMOREPARAM impossible?
	std::string						channel_name = word_picker(str, 2);
	std::string						arg = word_picker(str, 3);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user =
		channel->findUserinCh(get_user((*clientSockFd), irc_data)->getNickname());
//	std::list<User>::iterator		user = get_user(clientSockFd, irc_data);

	if (user == channel->getUsersList().end())
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
		Reply RPL_TOPIC(332, user->getNickname() + " " + channel_name + " " + channel->getTopic(), "");
		RPL_TOPIC.to_client(*clientSockFd);
	}
	else
	{
		Reply RPL_NOTOPIC(331, user->getNickname() + " " + channel_name, "No topic is set");
		RPL_NOTOPIC.to_client(*clientSockFd);
	}
}
