/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private_msg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 17:29:46 by jthuysba          #+#    #+#             */
/*   Updated: 2024/02/08 18:08:53 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/Notif.hpp" 

void	private_msg( std::string str, irc * irc_data, int clientSockFd)
{
	std::istringstream	iss(str);
	std::string				target;
	std::string				dump;
	std::string				text;
	
	iss >> target;
	std::getline(iss, text);

	if (*target.begin() == '#') // Target est un channel
	{
		std::string	channel_name = target;
		
		channel_name.erase(0);
		
		std::list<User>::iterator	origin_user = get_user(clientSockFd, irc_data);
		std::list<Channel>::iterator	target_channel = get_channel(channel_name, irc_data);
		
		std::string	id_string = origin_user->getNickname() + "!" + "user" + "@" + origin_user->getIp(); // WIP => Username and hostname to get
		Notif			message_to_send(id_string, "PRIVMSG", target, text);
		
		message_to_send.to_all(target_channel->getUsersList());
	}
	else // Target est un user
	{
		std::list<User>::iterator	origin_user = get_user(clientSockFd, irc_data);
		std::list<User>::iterator	targetUser = get_user_by_nick(target, irc_data);
		
		std::string	id_string = origin_user->getNickname() + "!" + "user" + "@" + origin_user->getIp(); // WIP => Username and hostname to get
		Notif			message_to_send(id_string, "PRIVMSG", target, text);
		
		std::cout << "string = " << message_to_send.get_cstr() << std::endl;
		
		message_to_send.to_client(targetUser->getSockFd());
	}
}