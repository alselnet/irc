/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:16:41 by jthuysba          #+#    #+#             */
/*   Updated: 2024/02/02 12:14:34 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"
#include "../include/Channel.hpp"

/* Functions */

template < typename T >
void	printContainer( T container )
{
	typename T::const_iterator	it = container.begin();
	typename T::const_iterator	ite = container.end();

	for (; it != ite; it++)
	{
		std::cout << "[" << it->getNickname() << "]" << std::endl;
	}
}

bool	deleteUserFromList( std::list<User> & list, const User & user )
{
	std::cout << user.getNickname() << std::endl;
	typename	std::list<User>::iterator	it = std::find(list.begin(), list.end(), user);
	
	std::cout << it->getNickname() << std::endl;

	if (it != list.end())
	{
		list.erase(it);
		return (true);
	}
	return (false);

}

/* Members Functions  */

void	Channel::changeTopic( User & user, std::string & newTopic )
{
	if (_topicMode == true) // Si seulement ops peuvent modifier topic
	{
		if (std::find(_operatorsList.begin(), _operatorsList.end(), user) == _operatorsList.end())
		{
			std::cerr << user.getNickname() << " cannot change the topic, he needs operator rights\n"; // Message a envoyer au client ? Retourner strings formatees
		}
		else
		{
			this->setTopic(newTopic);
			std::cerr << user.getNickname() << " set the channel's topic to \"" << newTopic << "\"\n"; // Message a envoyer au client ? Retourner strings formatees
		}
	}
	else // Si tout le monde peut modifier le topic
	{
		this->setTopic(newTopic);
	}
}

// Getters

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

// Setters

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

// Functions

void	Channel::kickUser( User & user )
{
	if (deleteUserFromList(_usersList, user) == true)
		std::cout << CYAN << user.getNickname() << RESET << " has been kicked !\n";
	else
		std::cout << CYAN << user.getNickname() << RESET << " not in users list !\n";
	printContainer(_usersList);
}

void	Channel::addUser( User & user )
{
	if (_inviteMode == true)
	{
		// check si invited
		deleteUserFromList(_invitedList, user);
	}
	else
	{
		_usersList.push_back(user);
		std::cout << CYAN << user.getNickname() << RESET
			<< " added to the Channel !" << std::endl;
		printContainer(_usersList);
	}
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