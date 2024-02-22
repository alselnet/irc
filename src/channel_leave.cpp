/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_leave.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:00:15 by ctchen            #+#    #+#             */
//   Updated: 2024/02/22 18:57:29 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "channel_parse.hpp"
#include "Reply.hpp"
#include "Notif.hpp"
#include "Error.hpp"

void	channel_leave(std::string *str, int *clientSockFd, irc *irc_data)
{
	std::list<User>::iterator		user = get_user((*clientSockFd), irc_data);
	if (user == irc_data->usersList.end())
	{
		Error ERR_NOTONCHANNEL(401, "", "", "User not found");
		ERR_NOTONCHANNEL.to_client(*clientSockFd);
		return ;
	}
	std::string 					channel_name = word_picker(str, 2);
	if (channel_name.empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "Leave needs more parameter");
		ERR_NEEDMOREPARAMS.to_client(*clientSockFd);
		return ;
	}
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	if (channel == irc_data->channelList.end())
	{
		Error ERR_NOSUCHCHANNEL(403, channel_name, "",
								"There is no such channel");
		ERR_NOSUCHCHANNEL.to_client(*clientSockFd);
		return ;
	}
	std::list<std::string>::iterator		ch_user =
		channel->findUserinCh(user->getNickname());
	if (ch_user == channel->getUsersListEnd())
	{
		Error ERR_NOTONCHANNEL(442, user->getNickname(), channel_name,
							   "You are not on that channel");
		ERR_NOTONCHANNEL.to_client(*clientSockFd);
		return ;
	}
	Notif	notif(user->getNickname() + "!" + user->getUsername() + "@"
				  + user->getIp(), "PART", channel_name, "");
	notif.to_client(*clientSockFd);
	notif.to_all_others(*channel, *clientSockFd, irc_data->usersList);
	channel->delUser(user->getNickname());
	channel->delOperator(user->getNickname());
	user->deleteChannel(channel_name);
	if (channel->usersListEmpty())
		irc_data->channelList.erase(channel);
	else
	{
		channel->addOperator(findUser((*channel->getUsersListBegin()), irc_data->usersList)->getNickname());
	}
}
