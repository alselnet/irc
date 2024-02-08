/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notif.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 03:54:49 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/08 13:57:26 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/notif.hpp"

notif::notif(void) : _notifMessage(NULL)
{
	return ;
}

notif::notif(std::string id_string, std::string command, std::string arguments, std::string additional)
{
	std::string			cmd_id;

	if (id_string.empty() || command.empty() || arguments.empty())
	{
		std::cerr << "incorrect arguments to notif constructor" << std::endl;
		return ;
	}

	this->_notifMessage = ":" + id_string + " " + command + " " + arguments;
	if (!additional.empty())
		this ->_notifMessage += " " + additional;
	this->_notifMessage += "\r\n";
	return ;
}

std::string	notif::get_notif_message(void)
{
	return (this->_notifMessage);
}

const char*	notif::get_cstr(void)
{
	return (this->_notifMessage.c_str());
}

int			notif::get_size(void)
{
	return(this->_notifMessage.size());
}

void		notif::to_all(std::list<User> target_fds)
{
	std::list<User>::iterator it;
	for (it = target_fds.begin(); it != target_fds.end(); it++)
	{
		this->to_client(it->getSockFd());
	}
	return ;
}

void		notif::to_client(unsigned int target_fd)
{
	send(target_fd, this->get_cstr(), this->get_size(), 0);
	return ;
}

notif &notif::operator=(notif &src)
{
	if (this != &src)
	{
		this->_notifMessage = src.get_notif_message();
	}
	return (*this);
}

notif::notif(notif &src)
{
	*(this) = src;
	return ;
}

notif::~notif(void)
{
	return ;
}