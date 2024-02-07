/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pong.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 04:46:02 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/06 04:46:12 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"

void	pong(int target_fd)
{
	std::string pong_reply;

	pong_reply = ":" + SERVER_NAME + " PONG " + SERVER_NAME + "\r\n";
	send(target_fd, pong_reply.c_str(), pong_reply.size(), 0);
	return ;
}