/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_mode.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 10:01:19 by ctchen            #+#    #+#             */
/*   Updated: 2024/02/20 09:15:38 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "channel_parse.hpp"
#include "Reply.hpp"
#include "Notif.hpp"
#include "Error.hpp"

std::string	word_skip_cut(std::string *str, unsigned long i)
{//Skip 1 mot + les espace qui suit et supprime+retourne le mot qui suit de str
	std::string	word = "";

	while (i < (*str).size() && (*str)[i] != ' ')
		i++;
	while (i < (*str).size() && (*str)[i] == ' ')
		i++;
	while (i < (*str).size() && (*str)[i] != ' ')
	{
		word += (*str)[i];
		(*str).erase(i, 1);
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
	if (!channel->operatorsListEmpty())
		active += "o";
	if (channel->getUsersLimit() > 0)
		active += "l";
	if (active.size() > 0)
		return ("+" + active);
	return (active);
}

/*
void	invite_list(std::list<User>::iterator user, int * clientSockFd,
					std::string *channel_name,
					std::list<Channel>::iterator channel)
{
	std::list<std::string>::const_iterator invited = channel->getInvitedListBegin();
	std::list<std::string>::const_iterator inviter = channel->getInviterListBegin();

	while (invited != channel->getInvitedListEnd())
	{
		Notif RPL_INVITELIST(user->getNickname() + "!" + user->getUsername() + "@" + user->getIp(), "346", (*invited) + " " + (*channel_name), (*inviter));
		RPL_INVITELIST.to_client(*clientSockFd);
		invited++;
		inviter++;
	}
	Notif RPL_ENDOFINVITELIST(user->getNickname() + "!" + user->getUsername() + "@" + user->getIp(), "347", (*invited) + " " + (*channel_name), "");
	RPL_ENDOFINVITELIST.to_client(*clientSockFd);
}
*/

void	mode_channel(std::string str_local, int *clientSockFd, irc *irc_data,
					 std::string *channel_name)
{
	std::list<Channel>::iterator	channel = get_channel((*channel_name), irc_data);
	std::list<User>::iterator		user = get_user((*clientSockFd), irc_data);
	std::string						flags = word_picker(&str_local, 3);
	unsigned long					i = 0;

	if (channel == irc_data->channelList.end())//pas dans la liste de numreplies pour mode channel
	{
		Error ERR_NOTONCHANNEL(442, user->getNickname(), (*channel_name),
					"This channel does not exists");
		ERR_NOTONCHANNEL.to_client((*clientSockFd));
		return ;
	}
	else if (channel_name->empty())//if param don't match options
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), (*channel_name),
								 "Not enough parameters");
		ERR_NEEDMOREPARAMS.to_client(*clientSockFd);
		return ;
	}
	else if (user->getOperator() == 0 &&
			 channel->findUserinCh(user->getNickname())
			 == channel->getUsersListEnd())
	{
		Error ERR_USERNOTINCHANNEL(441, user->getNickname(), (*channel_name),
								   "You are not in the channel");
		ERR_USERNOTINCHANNEL.to_client(*clientSockFd);
		return ;
	}
	if (flags.empty())
	{
		Notif notif(SERVER_NAME, "324", user->getNickname()  + " #" +
					(*channel_name),	active_mode(channel));
		notif.to_client(*clientSockFd);
		return ;
	}
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
				//invite_list(user, clientSockFd, channel_name, channel);
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
				// std::string key = word_skip_cut(&str_local, i + 2);
				
				std::string	key = word_picker(&str_local, 4);

				if (key.empty())
					return ;
				
				std::cout << "STR LOCAL = " << str_local << std::endl;
				
				if (!channel->getKey().empty()
					&& channel->getKey() != key)
				{
					Error ERR_KEYSET(467, user->getNickname(), (*channel_name),
									 "The key is already set");
					ERR_KEYSET.to_client(*clientSockFd);
					return;
				}
				option += "k " + key;
				std::cout << "OPTION = " << option << std::endl;
				if (set == 1)
					channel->setKey(key);
				else if (set == 0)
					channel->delKey();
				break;
			}
			case 'o':
			{
				option += 'o';
				std::string word = word_skip_cut(&str_local, i);
				if (set == 1)
					channel->addOperator(user->getNickname());
				else if (set == 0)
					channel->delOperator(user->getNickname());
				break;
			}
			case 'l':
			{
				std::string word = word_skip_cut(&str_local, i);
				option += 'l' + word;
				char	*ptr;
				if (set == 1)
					channel->setUsersLimit(std::strtoul(word.c_str(), &ptr, 10));
				else if (set == 0)
					channel->setUsersLimit(0);
				break;
			}
			default :
			{
				Error ERR_UNKNOWNMODE(472, user->getNickname(), (*channel_name), "Unknown mode");
				ERR_UNKNOWNMODE.to_client(*clientSockFd);
			}
			}
		}
		Notif	notif(user->getNickname() + "!" + user->getUsername() + "@"
					  + user->getIp(), "MODE", (*channel_name), option);
		notif.to_client(*clientSockFd);
	}
	else if (check_rights(user, channel) == false)
	{
		Error ERR_CHANOPRIVSNEEDED(482, user->getNickname(), (*channel_name),
					"You do not have operator privileges");
		ERR_CHANOPRIVSNEEDED.to_client(*clientSockFd);
	}
}

void	mode_user(std::string *str, int *clientSockFd, irc *irc_data, std::string *nickname)
{
	std::list<User>::iterator		user = get_user((*clientSockFd), irc_data);
	std::string						active_umode = "+";
	std::string						modes = word_picker(str, 3);

//	active_umode += "w";
	if (nickname->empty())
	{
		Error ERR_NEEDMOREPARAMS(461, user->getNickname(), "", "MODE needs parameter");
		ERR_NEEDMOREPARAMS.to_client(*clientSockFd);
		return;
	}
	else if ((*nickname) != user->getNickname())
	{//can change if serv op ?
		Error ERR_USERSDONTMATCH(502, user->getNickname(), "",
								 "Can't change mode for other users");
		ERR_USERSDONTMATCH.to_client(*clientSockFd);
		return ;
	}
	//ERR_UMODEUNKNOWNFLAG = 501
	if (user->getOperator())
		active_umode += "o";
	Reply RPL_UMODEIS(221, user->getNickname(), active_umode);
	RPL_UMODEIS.to_client(*clientSockFd);
}

void	mode_change(std::string *str, int *clientSockFd, irc *irc_data)
{
	std::string	second = word_picker(str, 2);

	if (second[0] == '#')
		mode_channel((*str), clientSockFd, irc_data, &second);
	else
		mode_user(str, clientSockFd, irc_data, &second);
}
