/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 14:59:35 by jthuysba          #+#    #+#             */
/*   Updated: 2024/03/05 16:22:19 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"
#include "../include/User.hpp"

/* Members Functions */

void	User::clearStock( void )
{
	_stock.clear();
}

void	User::addStock( std::string & add )
{
	_stock += add;
}

void	User::deleteChannel( std::string & channel )
{
	std::list<std::string>::iterator	it = _channelList.begin();
	std::list<std::string>::iterator	ite = _channelList.end();
	
	for (; it != ite; it++)
	{
		if (*it == channel)
		{
			_channelList.erase(it);
			return ;
		}
	}
}

void	User::addChannel( std::string & channel )
{
	_channelList.push_back(channel);
}

const std::string	& User::getStock( void ) const
{
	return (_stock);
}

std::list<std::string>	&User::getChannelList( void )
{
	return (_channelList);
}

int	User::getSockFd( void ) const
{
	return (_sockFd);
}

std::string User::getIp( void ) const
{
	return (_ip);
}

std::string User::getRealname( void ) const
{
	return (_realname);
}

std::string User::getUsername( void ) const
{
	return (_username);
}

std::string User::getNickname( void ) const
{
	return (_nickname);
}

bool		User::getOperator() const
{
	return (this->_irc_op);
}

bool	User::getPass(void) const
{
	return (this->_pass);
}

void	User::setIp( const std::string & ip )
{
	_ip = ip;
}

void	User::setRealname( const std::string & realname )
{
	_realname = realname;
}

void	User::setUsername( const std::string & username )
{
	_username = username;
}

void	User::setNickname( const std::string & nickname )
{
	_nickname = nickname;
}

void	User::setOperator(bool is_op)
{
	this->_irc_op = is_op;
}

void	User::setPass(const bool pass)
{
	this->_pass = pass;
}

/* Operators Overload */

bool User::operator==( const User & rhs ) const
{
	return (this->_serverAddr.sin_addr.s_addr == rhs._serverAddr.sin_addr.s_addr);
}

/* Constr & Destr */

User::User( int sockFd ) : _nickname(""), _username(""), _realname(""), _ip(""), _irc_op(false), _pass(false), _sockFd(sockFd), _stock("")
{
	// std::cout << DARK_WHITE << "User : Socket FD Constructor" << END;
}

User::~User( void )
{
	// std::cout << DARK_WHITE << "User : Destructor" << END;
}

User::User( const std::string & ip ) : _nickname(""), _username(""), _realname(""), _ip(ip), _irc_op(false), _stock("")
{
	// std::cout << DARK_WHITE << "User : IP Constructor" << END;
}

User::User( void ) : _nickname(""), _username(""), _realname(""), _ip(""), _irc_op(false), _stock("")
{
	// std::cout << DARK_WHITE << "User : Void Constructor" << END;
}
