/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:16:41 by jthuysba          #+#    #+#             */
//   Updated: 2024/02/16 23:09:23 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"

/* Functions */

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

void	Channel::addtoInviteList(std::string nickname)
{
	this->_invitedList.push_back(nickname);
}

void	Channel::delKey()
{
	this->_key.clear();
}

void	Channel::addOperator(std::list<User>::const_iterator user)
{
	std::list<User>::iterator	it;

	for (it = this->_operatorsList.begin(); it != this->_operatorsList.end(); it++)
	{//op already exists, no need to add
		if (it->getUsername() == user->getUsername())
			return ;
	}
	this->_operatorsList.push_back(*user);
}

void	Channel::delOperator(std::list<User>::const_iterator user)
{
	std::list<User>::iterator	it;

	for (it = this->_operatorsList.begin(); it != this->_operatorsList.end(); it++)
	{
		if (it->getUsername() == user->getUsername())
		{
			this->_operatorsList.erase(it);
			return ;
		}
	}
}

/*
void	Channel::addUser( std::list<User>::const_iterator user )
{//invitedlist not needed?
=======
	if (this->_inviteMode == true)
		deleteUserFromList(this->_invitedList, (*user)); //WIP
	else
	{
		this->_usersList.push_back((*user));
		std::cout << CYAN << user->getNickname() << RESET
				  << " added to the Channel !" << std::endl;
		printContainer(this->_usersList);
	}
}
*/

void	Channel::addUser( std::list<User>::const_iterator user )
{
	this->_usersList.push_back((*user));
	std::cout << CYAN << user->getNickname() << RESET
			  << " added to the Channel !" << std::endl;
	printContainer(this->_usersList);
}

void	Channel::delUser( std::list<User>::iterator user )
{
	this->_usersList.erase(user);
}

std::list<User>::iterator	Channel::findUserinCh(std::string nickname)
{
	std::list<User>::iterator it = this->_usersList.begin();
	while (it != this->_usersList.end())
	{
		if (it->getNickname() == nickname)
			return (it);
		it++;
	}
	return (it);
}

/* Members Functions  */

// Getters

std::list< User >	Channel::getUsersList( void ) const
{
	return (_usersList);
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

unsigned int	Channel::getUsersLimit( void ) const
{
	return (_usersLimit);
}

std::list< User >	Channel::getOperatorsList() const
{
	return (_operatorsList);
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

Channel::Channel( const std::string name ) : _name(name), _topic(""), _key(""), _inviteMode(false), _topicMode(false), _usersLimit(0)
{
	std::cout << DARK_WHITE << "Channel : Name Constructor" << END;
}

Channel::~Channel( void )
{
	std::cout << DARK_WHITE << "Channel : Destructor" << END;
}

Channel::Channel( void ) : _topic(""), _key(""), _inviteMode(false), _topicMode(false), _usersLimit(0)
{
	std::cout << DARK_WHITE << "Channel : Constructor" << END;
}
