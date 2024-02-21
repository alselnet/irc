/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private_msg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 17:29:46 by jthuysba          #+#    #+#             */
//   Updated: 2024/02/21 17:41:41 by ctchen           ###   ########.fr       //
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

	if (*target.begin() == '#') // Target est un channel
	{	
		std::list<User>::iterator		origin_user = get_user((*clientSockFd), irc_data);
		std::list<Channel>::iterator	target_channel = get_channel(target, irc_data);
		
		if (target_channel == irc_data->channelList.end()) // Si le channel n'existe pas
		{
			Error	ERR_NOSUCHNICK(401, target, "", "No such nick/channel");
			ERR_NOSUCHNICK.to_client(*clientSockFd);
			return ;
		}

		std::string	id_string = target_channel->getChanOperatorName(origin_user->getNickname()) + "!" + origin_user->getUsername() + "@" + origin_user->getIp(); // WIP => Username and hostname to get
		Notif			message_to_send(id_string, "PRIVMSG", target, text);
		
		message_to_send.to_all_others(*target_channel, (*clientSockFd));
	}
	else // Target est un user
	{
		//std::cerr << "Target is a user" << std::endl;
		std::list<User>::iterator	origin_user = get_user((*clientSockFd), irc_data);
		std::list<User>::iterator	target_user = get_user_by_nick(target, irc_data);
		
		if (target_user == irc_data->usersList.end()) // Si le nick n'existe pas
		{
			Error	ERR_NOSUCHNICK(401, target, "", "No such nick/channel");
			ERR_NOSUCHNICK.to_client(*clientSockFd);
			return ;
		}
		
		std::string	id_string = origin_user->getNickname() + "!" + origin_user->getUsername() + "@" + origin_user->getIp(); // WIP => Username and hostname to get
		Notif			message_to_send(id_string, "PRIVMSG", target, text);
		
		message_to_send.to_client(target_user->getSockFd());
	}
}
