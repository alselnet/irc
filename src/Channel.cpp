/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:16:41 by jthuysba          #+#    #+#             */
//   Updated: 2024/01/31 11:01:11 by ctchen           ###   ########.fr       //
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





bool	Channel::checkRights(User &init) const
{//verifie le droits de l'user init
	if (init.getOperator() == true)
		return true;
	else
	{
		for (std::list<User>::iterator it = this->_operatorsList.begin();
			 it != this->_operatorsList.end(); ++it)
		{
			if (it->getUsername() == init.getUsername())
				return true;
		}
	}
	return false;
}

std::string	Channel::wordRemoveExtract(std::string &str, unsigned long i)
{//Skip 1 mot les espace qui suit et supprime+retourne le mot qui suit de str
	std::string	word;

	while (i < str.size() && str[i] != ' ')
		i++;
	while (i < str.size() && str[i] == ' ')
		i++;
	while (i < str.size() && str[i] != ' ')
	{
		word += str[i];
		str.erase(i, 1);
	}
	return (word);
}

std::string	Channel::firstWord(std::string str)
{//retourne le premier mot
	std::string		temp;
	unsigned long 	i = 0;

	while (i < str.size() && str[i] != ' ')
	{
		temp += str[i];
		i++;
	}
	return (temp);
}

bool	Channel::commandHandler(User &init, std::string &str)//, Server &serv)
{//need to display as a message if returning 0
	if (str[0] != '/')
		return (0);
	str.erase(0, 1);
	if (str[0] == ' ')
		return (0);
	if (firstWord(str) == "MODE")
	{
		str.erase(0, 5);//only one space allowed after command
		std::string	chan_name = firstWord(str);
		str.erase(0, chan_name.size());
		//skip all space
		unsigned long	i = 0;
		while (i < str.size() && str[i] == ' ')
			i++;
		str.erase(0, i);
//		this->modeChange(init, str);
	}
	else if (firstWord(str) == "TOPIC")
	{
		str.erase(0, 6);
//		this->topicChange(init, str);
	}
	else if (firstWord(str) == "INVITE")
	{
//		this->inviteUser(init, wordRemoveExtract(str, 0), serv);
//		std::cout << "check = " << str << std::endl;//debug
		str.erase(0, 7);
	}
//	else if (firstword(str) == "KICK")
//		this->kickUser(init, nameofkicked);
	return (1);
}// "/MODE  +l 10" ne doit pas marcher et pas d'undefined behavior
