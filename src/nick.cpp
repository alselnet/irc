/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/12 16:47:53 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/14 12:43:48 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"

// void	set_nick(std::string nick, int clientSockFd, irc *irc_data)
// {
// 	if (nick.empty()) //ERR_NONICKNAMEGIVEN
// 	{

// 	}
// 	else if() //ERR_NICKNAMEINUSE
// }

void nick(std::string arg, int clientSockFd, irc *irc_data)
{
		if (get_user(clientSockFd, irc_data)->getNickname().empty())
		{
			get_user(clientSockFd, irc_data)->setNickname(arg);
			handshake_replies(clientSockFd, get_user(clientSockFd, irc_data)->getNickname());
		}
		else
			get_user(clientSockFd, irc_data)->setNickname(arg);
		return ;
}
