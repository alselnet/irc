/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_invite.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 13:15:05 by ctchen            #+#    #+#             */
//   Updated: 2024/02/19 19:57:34 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "channel_parse.hpp"
#include "Reply.hpp"
#include "Notif.hpp"
#include "Error.hpp"

void	addtoinv_user(std::string *str, int *clientSockFd, irc *irc_data, std::string target_name)
{
	std::string 					channel_name = word_picker(str, 3);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator		user = get_user((*clientSockFd), irc_data);
	std::list<User>::iterator 		target = get_user_by_nick(target_name, irc_data);

	if (target == irc_data->usersList.end())
	{
		Error ERR_NOSUCHNICK(401, user->getNickname(), word_picker(str, 2) + " :"
							 , "There is no such nickname");
		ERR_NOSUCHNICK.to_client(*clientSockFd);
		return ;
	}
	else if (target == irc_data->usersList.end())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "Invite needs parameter");
		ERR_NEEDMOREPARAMS.to_client(*clientSockFd);
		return ;
	}
	else if (user == channel->getUsersListEnd())
	{
		Error ERR_NOTONCHANNEL(442, user->getNickname(), channel_name,
							   "You are not on that channel");
		ERR_NOTONCHANNEL.to_client(*clientSockFd);
		return ;
	}
	else if (channel->findUserinCh(word_picker(str, 2)) != channel->getUsersListEnd())
	{
		Error ERR_USERONCHANNEL(443, user->getNickname(), word_picker(str, 2)
								+ " " + channel_name, "Target already in channel");
		ERR_USERONCHANNEL.to_client(*clientSockFd);
		return ;
	}
	else
	{
		if ((channel->getInviteMode() == true && check_rights(user, channel) == true)
			|| channel->getInviteMode() == false)
		{
			channel->addtoInviteList(target->getNickname());
			Notif	RPL_INVITING(user->getNickname() + "!" + user->getUsername() + "@"
								 + user->getIp(), "RPL_INVITING", target->getNickname(),
								 channel_name);
			RPL_INVITING.to_client(*clientSockFd);
		}
		else
		{
			Error ERR_CHANOPRIVSNEEDED(482, user->getNickname(), channel_name,
								   "You do not have operator privileges");
			ERR_CHANOPRIVSNEEDED.to_client(*clientSockFd);
		}
	}
	Notif	notif(user->getNickname() + "!" + user->getUsername() + "@"
				  + user->getIp(), "INVITE", channel_name, "");
	notif.to_client(*clientSockFd);
}

void	invite_user(std::string *str, int *clientSockFd, irc *irc_data)
{
	std::string		userlist = word_picker(str, 2);
	unsigned long	count_user = word_comma_replace(&userlist);

	for (unsigned long i = 0; i <= count_user; i++)
	{
		std::string select_user = word_picker(&userlist, i + 1);
		addtoinv_user(str, clientSockFd, irc_data, select_user);
	}
}
