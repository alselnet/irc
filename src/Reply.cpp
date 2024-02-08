/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reply.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 18:29:20 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/08 17:40:31 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Reply.hpp"

Reply::Reply(void) : _replyMessage(NULL)
{
	return ;
}

Reply::Reply(int cmd_nb, std::string target_username, std::string additional)
{
	std::string			cmd_id;
	std::stringstream	ss;

	if (cmd_nb < 1 || cmd_nb > 999 || target_username.empty())
	{
		std::cerr << "incorrect arguments to Reply constructor" << std::endl;
		return ;
	}
	ss << cmd_nb;
	if (cmd_nb < 10)
		cmd_id = "00" + ss.str();
	else if (cmd_nb < 100)
		cmd_id = "0" + ss.str();
	else
		cmd_id = ss.str();

	this->_replyMessage = ":" + SERVER_NAME + " " + cmd_id + " " + target_username;
	if (!additional.empty())
		this ->_replyMessage += " :" + additional;
	this->_replyMessage += "\r\n";
	return ;
}

std::string	Reply::get_reply_message(void)
{
	return(this->_replyMessage);
}

const char*	Reply::get_cstr(void)
{
	return (this->_replyMessage.c_str());
}

int			Reply::get_size(void)
{
	return(this->_replyMessage.size());
}

void		Reply::to_client(unsigned int target_fd)
{
	send(target_fd, this->get_cstr(), this->get_size(), 0);
	return ;
}

Reply &Reply::operator=(Reply &src)
{
	if (this != &src)
	{
		this->_replyMessage = src.get_reply_message();
	}
	return (*this);
}

Reply::Reply(Reply &src)
{
	*(this) = src;
	return ;
}

Reply::~Reply(void)
{
	return ;
}