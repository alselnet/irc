/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private_msg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 17:29:46 by jthuysba          #+#    #+#             */
//   Updated: 2024/02/22 20:03:59 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/Notif.hpp"
# include "../include/Error.hpp"

void	private_msg(std::string *str, int *clientSockFd, irc *irc_data)
{
	std::istringstream	iss(*str);
	std::string				target;
	std::string				dump;
	std::string				text;
	
	iss >> dump;
	iss >> target;
	iss.ignore();
	std::getline(iss, text);

	if (target.empty())
	{
		Error	ERR_NORECIPIENT(411, target, "", "No recipient given (/msg)");
		ERR_NORECIPIENT.to_client(*clientSockFd);
		return ;
	}

	if (text.empty())
	{
		Error	ERR_NOTEXTTOSEND(412, target, "", "No text to send");
		ERR_NOTEXTTOSEND.to_client(*clientSockFd);
		return ;
	}
	if (*target.begin() == '#')
	{	
		std::list<User>::iterator		origin_user = get_user((*clientSockFd), irc_data);
		std::list<Channel>::iterator	target_channel = get_channel(target, irc_data);
		
		if (target_channel == irc_data->channelList.end())
		{
			Error	ERR_NOSUCHCHANNEL(403, origin_user->getNickname(), target, "No such nick/channel");
			ERR_NOSUCHCHANNEL.to_client(*clientSockFd);
			return ;
		}
		else if (target_channel->findUserinCh(origin_user->getNickname()) == target_channel->getUsersListEnd())
		{
			Error	ERR_NOTINCHANNEL(442, origin_user->getNickname(), target, "Sending extern msg to channel prohibited: it will not be received");
			ERR_NOTINCHANNEL.to_client(*clientSockFd);
			return ;
		}
		std::string	id_string = target_channel->getChanOperatorName(origin_user->getNickname()) + "!" + origin_user->getUsername() + "@" + origin_user->getIp();
		Notif			message_to_send(id_string, "PRIVMSG", target, text);		
		message_to_send.to_all_others(*target_channel, (*clientSockFd), irc_data->usersList);
	}
	else
	{
		std::list<User>::iterator	origin_user = get_user((*clientSockFd), irc_data);
		std::list<User>::iterator	target_user = get_user_by_nick(target, irc_data);
		
		if (target_user == irc_data->usersList.end())
		{
			Error	ERR_NOSUCHNICK(401, origin_user->getNickname(), target, "");
			ERR_NOSUCHNICK.to_client(*clientSockFd);
			return ;
		}
		
		std::string	id_string = origin_user->getNickname() + "!" + origin_user->getUsername() + "@" + origin_user->getIp();
		Notif			message_to_send(id_string, "PRIVMSG", target, text);		
		message_to_send.to_client(target_user->getSockFd());
	}
}
