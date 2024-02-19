/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private_msg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 17:29:46 by jthuysba          #+#    #+#             */
//   Updated: 2024/02/19 00:37:04 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/Notif.hpp" 

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

	if (*target.begin() == '#') // Target est un channel
	{	
		std::list<User>::iterator		origin_user = get_user((*clientSockFd), irc_data);
		std::list<Channel>::iterator	target_channel = get_channel(target, irc_data);
		
		// printContainer(target_channel->getUsersList());
		
		std::string	id_string = origin_user->getNickname() + "!" + origin_user->getUsername() + "@" + origin_user->getIp(); // WIP => Username and hostname to get
		Notif			message_to_send(id_string, "PRIVMSG", target, text);
		
		message_to_send.to_all_others(target_channel->getUsersList(), (*clientSockFd));
	}
	else // Target est un user
	{
		std::list<User>::iterator	origin_user = get_user((*clientSockFd), irc_data);
		std::list<User>::iterator	targetUser = get_user_by_nick(target, irc_data);
		
		std::string	id_string = origin_user->getNickname() + "!" + origin_user->getUsername() + "@" + origin_user->getIp(); // WIP => Username and hostname to get
		Notif			message_to_send(id_string, "PRIVMSG", target, text);
		
		message_to_send.to_client(targetUser->getSockFd());
	}
}
