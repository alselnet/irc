/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:16:41 by jthuysba          #+#    #+#             */
//   Updated: 2024/02/08 14:27:12 by ctchen           ###   ########.fr       //
/*                                                                            */
/* ************************************************************************** */

#include "../include/irc.hpp"

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

/* fonctionnel mais non utile(?): droits de l'user1 = is_op, besoin de la cible
void	Channel::kickUser( std::list<User>::const_iterator user, bool is_op)
{
	if (is_op == true)
	{
		if (deleteUserFromList(_usersList, (*user)) == true)
			std::cout << CYAN << user->getNickname() << RESET << " has been kicked !\n";
		else
			std::cout << CYAN << user->getNickname() << RESET << " not in users list !\n";
		printContainer(_usersList);
	}
}
*/

void	Channel::kickUser( std::string target, bool is_op)
{
	std::list<User>::iterator it = findUserinCh(target);

	if (it != this->_usersList.end())
	{
		if (is_op == true)
		{
			this->_usersList.erase(it);
			std::cout << CYAN << target << RESET << " has been kicked !\n";	
			printContainer(_usersList);
		}
	}
	else
		std::cout << CYAN << target << RESET << " not in users list !\n";
}


void	Channel::addUser( User & user )
{
	if (this->_inviteMode == true)
		deleteUserFromList(this->_invitedList, (user));
	else
	{
		this->_usersList.push_back(user);
		std::cout << CYAN << user.getNickname() << RESET
				  << " added to the Channel !" << std::endl;
		printContainer(this->_usersList);
	}
}

void	Channel::addUser( std::list<User>::const_iterator user )
{//invitedlist not needed
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

void	Channel::inviteUser( std::list<User>::const_iterator user,
							 std::string target, bool is_op )
{
	(void)target;
	std::list<User>::iterator it = findUserinCh(user->getUsername());

	if (it != this->_usersList.end())
	{
		if (this->_inviteMode == true && is_op == true)
			deleteUserFromList(this->_invitedList, (*user));
		else
		{
			this->_usersList.push_back((*user));
			std::cout << CYAN << user->getNickname() << RESET
					  << " added to the Channel !" << std::endl;
			printContainer(this->_usersList);
		}
	}
}

std::string	Channel::wordSkipExtractRemove(std::string &str, unsigned long i)
{//Skip 1 mot + les espace qui suit et supprime+retourne le mot qui suit de str
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

void	Channel::changeTopic( std::string nickname, std::string & newTopic, bool is_op )
{
	newTopic.erase(0, 6);
	if (_topicMode == true) // Si seulement ops peuvent modifier topic
	{
		if (is_op == false)
		{
			std::cerr << nickname << " cannot change the topic, he needs operator rights\n"; // Message a envoyer au client ? Retourner strings formatees
		}
		else
		{
			this->setTopic(newTopic);
			std::cerr << nickname << " set the channel's topic to \"" << newTopic << "\"\n"; // Message a envoyer au client ? Retourner strings formatees
		}
	}
	else // Si tout le monde peut modifier le topic
	{
		this->setTopic(newTopic);
		std::cerr << nickname << " set the channel's topic to \"" << newTopic << "\"\n";
	}
}

void	Channel::modeMsg(const char *word, bool set, char flag, std::string username )
{
	std::cout << "mode/#" << this->getChName() << " [";
	if (set == 0)
		std::cout << '-';
	else if (set == 1)
		std::cout << '+';
	std::cout << flag;
	if (word != NULL)
		std::cout << ' ' << word;
	std:: cout << "] by " << username <<  std::endl;
}

void	Channel::modeChange(std::list<User>::const_iterator user, std::string str, bool is_op)
{
	str.erase(0, 5);//only one space allowed after command
	std::string chan_name = firstWord(str);
	str.erase(0, chan_name.size());
      
	unsigned long   i = 0;
	while (i < str.size() && str[i] == ' ')
		i++;
	str.erase(0, i);
	if (is_op == true)
	{
		bool	set = 0;
		
		for (unsigned long i = 0; i < str.size(); i++)
		{
			switch (str[i])
			{
			case '+':
				set = 1;
				break;
			case '-':
				set = 0;
				break;
			case 'i':
			{
//				this->modeMsg(NULL, set, 'i', user->getUsername());
				this->setInviteMode(set);
				break;
			}
			case 't':
//				this->modeMsg(NULL, set, 't', user->getUsername());
				this->setTopicMode(set);
				break;
			case 'k':
			{
				std::string word = this->wordSkipExtractRemove(str, i);
//				this->modeMsg(word.c_str(), set, 'k', user->getUsername());
				if (set == 1)
					this->setKey(word);
				else if (set == 0)
					this->delKey();
				break;
			}
			case 'o':
			{
				std::string word = this->wordSkipExtractRemove(str, i);
//				this->modeMsg(word.c_str(), set, 'o', user->getUsername());
				if (set == 1)
					this->addOperator(user);
				else if (set == 0)
					this->delOperator(user);
				break;
			}
			case 'l':
			{
				std::string word = this->wordSkipExtractRemove(str, i);
				char	*ptr;
//				this->modeMsg(word.c_str(), set, 'l', user->getUsername());
				if (set == 1)
					this->setUsersLimit(std::strtoul(word.c_str(), &ptr, 10));
				else if (set == 0)
					this->setUsersLimit(0);
				break;
			}
			}
		}
	}
}

/* Members Functions  */

// Getters

std::string	Channel::getChName( void ) const
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
