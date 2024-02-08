/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:16:41 by jthuysba          #+#    #+#             */
//   Updated: 2024/02/08 21:14:32 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"

/* Functions */

bool	deleteUserFromList( std::list<User> & list, const User & user )
{
	typename	std::list<User>::iterator	it = std::find(list.begin(), list.end(), user);

	if (it != list.end())
	{
		list.erase(it);
		return (true);
	}
	return (false);

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
	if (this->_inviteMode == true)
		deleteUserFromList(this->_invitedList, (*user));
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
	if (this->_inviteMode == true)
		this->_usersList.push_back((*user));
	else
	{
		this->_usersList.push_back((*user));
		std::cout << CYAN << user->getNickname() << RESET
				  << " added to the Channel !" << std::endl;
		printContainer(this->_usersList);
	}
}

std::list<User>::iterator	Channel::findUserinCh(std::string username)
{
	std::list<User>::iterator it = this->_usersList.begin();
	while (it != this->_usersList.end())
	{
		if (it->getUsername() == username)
			return (it);
		it++;
	}
	return (it);
}

/* Members Functions  */

// Getters

std::list<User>	Channel::getUsersList( void ) const
{
	return (_usersList);
}

std::list<User>	Channel::getInvitedList( void ) const
{
	return (_invitedList);
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

Channel::Channel( const std::string name ) : _name(name)
{
	std::cout << DARK_WHITE << "Channel : Name Constructor" << END;
}

Channel::~Channel( void )
{
	std::cout << DARK_WHITE << "Channel : Destructor" << END;
}

Channel::Channel( void ) : _topic(""), _key(""), _inviteMode(false), _topicMode(false), _usersLimit(-1)
{
	std::cout << DARK_WHITE << "Channel : Constructor" << END;
}
