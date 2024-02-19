/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 18:29:20 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/19 13:38:34 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Error.hpp"

Error::Error(void) : _replyMessage(NULL)
{
	return ;
}

Error::Error(int error_nb, std::string client_nick, std::string erroneous_parameter, std::string error_description)
{
	std::string			error_id;
	std::stringstream	ss;

	if (error_nb < 1 || error_nb > 999 || error_description.empty())
	{
		std::cerr << "incorrect arguments to Error constructor" << std::endl;
		std::cerr << "error_nb was: " << error_nb << std::endl;
		std::cerr << "client_nick was: " << client_nick << std::endl;
		std::cerr << "erroneus_parameter was: " << erroneous_parameter << std::endl;
		std::cerr << "error_description was: " << error_description << std::endl;
		return ;
	}
	ss << error_nb;
	error_id = ss.str();
	if (erroneous_parameter.empty())
		this->_replyMessage = ":" + SERVER_NAME + " " + error_id + " " + client_nick + " :" + error_description + "\r\n";
	else
		this->_replyMessage = ":" + SERVER_NAME + " " + error_id + " " + client_nick + " " + erroneous_parameter + " :" + error_description + "\r\n";
	return ;
}

std::string	Error::get_reply_message(void)
{
	return(this->_replyMessage);
}

const char*	Error::get_cstr(void)
{
	return (this->_replyMessage.c_str());
}

int			Error::get_size(void)
{
	return(this->_replyMessage.size());
}

void		Error::to_client(unsigned int target_fd)
{
	send(target_fd, this->get_cstr(), this->get_size(), 0);
	return ;
}

Error &Error::operator=(Error &src)
{
	if (this != &src)
	{
		this->_replyMessage = src.get_reply_message();
	}
	return (*this);
}

Error::Error(Error &src)
{
	*(this) = src;
	return ;
}

Error::~Error(void)
{
	return ;
}