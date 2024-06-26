/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel_invite.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 13:15:05 by ctchen            #+#    #+#             */
//   Updated: 2024/02/22 20:02:17 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "channel_parse.hpp"
#include "Reply.hpp"
#include "Notif.hpp"
#include "Error.hpp"

void	addtoinv_user(std::string *str, int *clientSockFd, irc *irc_data, 
std::string target_name, std::list<User>::iterator user)
{
	std::string 					channel_name = word_picker(str, 3);
	std::list<Channel>::iterator	channel = get_channel(channel_name, irc_data);
	std::list<User>::iterator 		target = get_user_by_nick(target_name, irc_data);

	if (channel == irc_data->channelList.end())
	{
		Error ERR_NOSUCHCHAN(403, user->getNickname(), channel_name + " :"
							 , "There is no such channel");
		ERR_NOSUCHCHAN.to_client(*clientSockFd);
		return ;
	}
	else if (target_name.empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "Invite needs parameter");
		ERR_NEEDMOREPARAMS.to_client(*clientSockFd);
		return ;
	}
	else if (target == irc_data->usersList.end())
	{
		Error ERR_NOSUCHNICK(401, user->getNickname(), word_picker(str, 2) + " :"
							 , "There is no such nickname");
		ERR_NOSUCHNICK.to_client(*clientSockFd);
		return ;
	}
	else if (channel->findUserinCh(target_name) != channel->getUsersListEnd())
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
			channel->addtoInviteList(target->getNickname(), user->getNickname());
			Notif	RPL_INVITING(user->getNickname() + "!" + user->getUsername() + "@"
								 + user->getIp(), "RPL_INVITING", target->getNickname(), channel_name);
			RPL_INVITING.to_client(*clientSockFd);
			Notif	you_are_invited(user->getNickname() + "!" + user->getUsername()
									+ "@" + user->getIp(), user->getNickname(), "invites you to", channel_name);
			you_are_invited.to_client(target->getSockFd());
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
	std::list<User>::iterator		user = get_user((*clientSockFd), irc_data);
	std::string		userlist = word_picker(str, 2);


	if (user == irc_data->usersList.end())
	{
		Error ERR_NOSUCHNICK(401, "", "", "User doesn't exist");
		ERR_NOSUCHNICK.to_client(*clientSockFd);
		return ;
	}
	if (word_picker(str, 3).empty() || userlist.empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "Invite needs parameter");
		ERR_NEEDMOREPARAMS.to_client(*clientSockFd);
		return ;
	}
	unsigned long	count_comma = word_comma_replace(&userlist);
	for (unsigned long i = 0; i <= count_comma; i++)
	{
		std::string select_user = word_picker(&userlist, i + 1);
		addtoinv_user(str, clientSockFd, irc_data, select_user, user);
	}
}
