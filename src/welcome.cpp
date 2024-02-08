/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   welcome.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 04:46:33 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/08 17:40:31 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/Reply.hpp"

void	handshake_replies(int clientSockFd, std::string target_nickname)
{
	Reply RPL_WELCOME(001, target_nickname, "Welcome to the new whatsapp " + target_nickname);
	Reply RPL_YOURHOST(002, target_nickname, "Your host is " + SERVER_NAME+ ", running version " + SERVER_VERS);
	Reply RPL_CREATED(003, target_nickname, "This server was created on " + SERVER_BIRTH);
	Reply RPL_MYINFO(004, target_nickname, SERVER_NAME + " " +  SERVER_BIRTH + " " + SERVER_UMODES + " " + SERVER_CMODES);

	RPL_WELCOME.to_client(clientSockFd);
	RPL_YOURHOST.to_client(clientSockFd);	
	RPL_CREATED.to_client(clientSockFd);
	RPL_MYINFO.to_client(clientSockFd);

	return ;
}