/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:16:41 by jthuysba          #+#    #+#             */
//   Updated: 2024/02/22 21:14:34 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"

/* Members Functions */

bool	Channel::operatorsListEmpty( void ) const
{
	return (_operatorsList.empty());
}

bool	Channel::usersListEmpty( void ) const
{
	return (_usersList.empty());
}

unsigned long	Channel::usersListSize( void) const
{
	return (_usersList.size());
}

void	Channel::deleteInvited( std::string nickname )
{
	std::list<std::string>::iterator	it = _invitedList.begin();
	std::list<std::string>::const_iterator	ite = _invitedList.end();

	for (; it != ite; it++)
	{
		if ((*it) == nickname)
		{
			_invitedList.erase(it);
			return ;
		}
	}
}

bool	Channel::checkInvite( std::string nickname ) const
{
	std::list<std::string>::const_iterator	it = _invitedList.begin();
	std::list<std::string>::const_iterator	ite = _invitedList.end();

	for (; it != ite; it++)
	{
		if ((*it) == nickname)
			return (true);
	}
	return (false);
}

void	Channel::addtoInviteList(std::string invitee, std::string inviter)
{
	for (std::list<std::string>::iterator it = this->_invitedList.begin();
		 it != this->_invitedList.end(); it++)
	{
		if (invitee == (*it))
			return;
	}
	this->_invitedList.push_back(invitee);
	this->_inviterList.push_back(inviter);
}

void	Channel::delKey()
{
	this->_key.clear();
}

std::list<std::string>::iterator	Channel::findUserinCh(std::string nickname)
{
	std::list<std::string>::iterator it = this->_usersList.begin();

	while (it != this->_usersList.end())
	{
		if ((*it) == nickname)
			return (it);
		it++;
	}
	return (it);
}

std::list<std::string>::iterator	Channel::findOpinCh(std::string nickname)
{
	std::list<std::string>::iterator it = this->_operatorsList.begin();

	while (it != this->_operatorsList.end())
	{
		if ((*it) == nickname)
			return (it);
		it++;
	}
	return (it);
}

void	Channel::addOperator(std::string nickname)
{
	std::list<std::string>::iterator	it;

	for (it = this->_operatorsList.begin(); it != this->_operatorsList.end(); it++)
	{
		if ((*it) == nickname)
			return ;
	}
	this->_operatorsList.push_back(nickname);
}

void	Channel::delOperator(std::string nickname)
{
	if (this->findOpinCh(nickname) != this->getOpListEnd())
		this->_operatorsList.erase(this->findOpinCh(nickname));
}

void	Channel::addUser( std::string nickname )
{
	this->_usersList.push_back(nickname);
	this->_usersCount++;
	std::cout << CYAN << nickname << RESET
			  << " added to the Channel !" << std::endl;
}

void	Channel::delUser( std::string nickname )
{
	std::list<std::string>::iterator it = this->_usersList.begin();

	for (; it != this->_usersList.end(); it++)
	{
		if ((*it) == nickname)
		{
			this->_usersList.erase(it);
			this->_usersCount--;
			return ;
		}
	}
}

/* Members Functions  */

// Getters

std::string	Channel::getChanOperatorName(std::string nickname)
{
	std::list<std::string>::iterator	it;

	for (it = this->_operatorsList.begin(); it != this->_operatorsList.end(); it++)
	{
		if ((*it) == nickname)
			return ("@" + nickname);
	}
	return (nickname);
}

std::list<std::string>::const_iterator	Channel::getInvitedListBegin()
{
	return (_invitedList.begin());
}

std::list<std::string>::const_iterator	Channel::getInvitedListEnd()
{
	return (_invitedList.end());
}

std::list<std::string>::const_iterator	Channel::getInviterListBegin()
{
	return (_inviterList.begin());
}

std::list<std::string>::const_iterator	Channel::getInviterListEnd()
{
	return (_inviterList.end());
}

std::string	Channel::getChName( void) const
{
	return (_name);
}

std::string	Channel::getTopic( void ) const
{
	return (_topic);
}

std::string	Channel::getKey( void ) const
{
	return (_key);
}

bool	Channel::getInviteMode( void ) const
{
	return (_inviteMode);
}

bool	Channel::getTopicMode( void ) const
{
	return (_topicMode);
}

unsigned long	Channel::getUsersCount( void ) const
{
	return (_usersCount);
}

unsigned long	Channel::getUsersLimit( void ) const
{
	return (_usersLimit);
}

std::list<std::string>::const_iterator	Channel::getUsersListBegin( void ) const
{
	return (_usersList.begin());
}

std::list<std::string>::const_iterator	Channel::getUsersListEnd( void ) const
{
	return (_usersList.end());
}

std::list<std::string>::const_iterator	Channel::getOpListBegin( void ) const
{
	return (_operatorsList.begin());
}

std::list<std::string>::const_iterator	Channel::getOpListEnd( void ) const
{
	return (_operatorsList.end());
}

// Setters

void	Channel::setChName( std::string name )
{
	_name = name;
}

void	Channel::setTopic( std::string topic )
{
	_topic = topic;
}

void	Channel::setKey( std::string key )
{
	_key = key;
}

void	Channel::setInviteMode( bool status )
{
	_inviteMode = status;
}

void	Channel::setTopicMode( bool status )
{
	_topicMode = status;
}

void	Channel::setUsersLimit( unsigned int limit )
{
	_usersLimit = limit;
}

/* Constr & Destr */

Channel::Channel( const std::string name ) : _name(name), _topic(""), _key(""), _inviteMode(false), _topicMode(false), _usersLimit(0), _usersCount(0)
{
	// std::cout << DARK_WHITE << "Channel : Name Constructor" << END;
}

Channel::~Channel( void )
{
	// std::cout << DARK_WHITE << "Channel : Destructor" << END;
}

Channel::Channel( void ) : _topic(""), _key(""), _inviteMode(false), _topicMode(false), _usersLimit(0), _usersCount(0)
{
	// std::cout << DARK_WHITE << "Channel : Constructor" << END;
}
