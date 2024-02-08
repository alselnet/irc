/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notif.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 03:54:49 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/08 17:37:57 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Notif.hpp"

Notif::Notif(void) : _notifMessage(NULL)
{
	return ;
}

Notif::Notif(std::string id_string, std::string command, std::string arguments, std::string additional)
{
	std::string			cmd_id;

	if (id_string.empty() || command.empty() || arguments.empty())
	{
		std::cerr << "incorrect arguments to Notif constructor" << std::endl;
		return ;
	}

	this->_notifMessage = ":" + id_string + " " + command + " " + arguments;
	if (!additional.empty())
		this ->_notifMessage += " " + additional;
	this->_notifMessage += "\r\n";
	return ;
}

std::string	Notif::get_notif_message(void)
{
	return (this->_notifMessage);
}

const char*	Notif::get_cstr(void)
{
	return (this->_notifMessage.c_str());
}

int			Notif::get_size(void)
{
	return(this->_notifMessage.size());
}

void		Notif::to_all(std::list<User> target_fds)
{
	std::list<User>::iterator it;
	for (it = target_fds.begin(); it != target_fds.end(); it++)
	{
		this->to_client(it->getSockFd());
	}
	return ;
}

void		Notif::to_client(unsigned int target_fd)
{
	send(target_fd, this->get_cstr(), this->get_size(), 0);
	return ;
}

Notif &Notif::operator=(Notif &src)
{
	if (this != &src)
	{
		this->_notifMessage = src.get_notif_message();
	}
	return (*this);
}

Notif::Notif(Notif &src)
{
	*(this) = src;
	return ;
}

Notif::~Notif(void)
{
	return ;
}