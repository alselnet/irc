/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   welcome.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 04:46:33 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/06 13:25:01 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/reply.hpp"

void	handshake_replies(int clientSockFd, std::string target_nickname)
{
	reply RPL_WELCOME(001, target_nickname, "Welcome to the new whatsapp " + target_nickname);
	reply RPL_YOURHOST(002, target_nickname, "Your host is " + SERVER_NAME+ ", running version " + SERVER_VERS);
	reply RPL_CREATED(003, target_nickname, "This server was created on " + SERVER_BIRTH);
	reply RPL_MYINFO(004, target_nickname, SERVER_NAME + " " +  SERVER_BIRTH + " " + SERVER_UMODES + " " + SERVER_CMODES);

	RPL_WELCOME.to_client(clientSockFd);
	RPL_YOURHOST.to_client(clientSockFd);	
	RPL_CREATED.to_client(clientSockFd);
	RPL_MYINFO.to_client(clientSockFd);

	return ;
}