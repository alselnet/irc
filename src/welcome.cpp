/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   welcome.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 04:46:33 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/06 04:46:53 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/reply.hpp"

void	handshake_replies(int clientSockFd, std::string target_username)
{
	reply RPL_WELCOME(001, target_username, "Welcome to the new whatsapp " + target_username);
	reply RPL_YOURHOST(002, target_username, "Your host is " + target_username + ", running version " + SERVER_VERS);
	reply RPL_CREATED(003, target_username, "This server was created on " + SERVER_BIRTH);
	reply RPL_MYINFO(004, target_username, SERVER_NAME + " " +  SERVER_BIRTH + " " + SERVER_UMODES + " " + SERVER_CMODES);

	RPL_WELCOME.to_client(clientSockFd);
	RPL_YOURHOST.to_client(clientSockFd);	
	RPL_CREATED.to_client(clientSockFd);
	RPL_MYINFO.to_client(clientSockFd);

	return ;
}