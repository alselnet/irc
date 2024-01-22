/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:16:41 by jthuysba          #+#    #+#             */
/*   Updated: 2024/01/22 20:02:25 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"
#include "../include/Channel.hpp"

/* Functions */

template < typename T, typename V >
void	deleteUserFromList( T container, const V & value )
{
	typename T::const_iterator	it = container.begin();
	typename T::const_iterator	ite = container.end();

	for (; it != ite; it++)
	{
		if ((*it) == value)
		{
			container.erase(it);
			return ;
		}
	}

	std::cout << "Not in the list !" << std::endl;
}

/* Members Functions  */

void	Channel::addUser( const User & user )
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
	}
}

/* Constr & Destr */

Channel::Channel( std::string name ) : _name(name)
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