/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:16:41 by jthuysba          #+#    #+#             */
/*   Updated: 2024/02/06 15:19:51 by jthuysba         ###   ########.fr       */
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
		std::cout << "[" << CYAN << it->getNickname() << RESET << "]" << std::endl;
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

// Functions

void	Channel::delKey()
{
	this->_key.clear();
}

void	Channel::addOperator(std::string username)
{
	this->_operatorsList.push_back(username);
}

void	Channel::delOperator(std::string username)
{
	std::list< User >::iterator	it;

	for (it = this->_operatorsList.begin(); it != this->_operatorsList.end(); it++)
	{
		if ((*it) == username)
		{
			this->_operatorsList.erase(it);
			return ;
		}
	}
}

void	Channel::kickUser( User & user, User & init)
{
//	std::list<User>::iterator it = findUserI(str, usersList);

//	if (it != usersList.end())
//	{
		if (this->checkRights(init) == true)
		{
			if (deleteUserFromList(_usersList, user) == true)
				std::cout << CYAN << user.getNickname() << RESET << " has been kicked !\n";
			else
				std::cout << CYAN << user.getNickname() << RESET << " not in users list !\n";
			printContainer(_usersList);
		}
//	}
//	else
//		std::cout << "User not found" << std::endl;
}

void	Channel::addUser( User & user )
{
	if (this->_inviteMode == true)
	{
			// check si invited
			deleteUserFromList(this->_invitedList, user);
	}
	else
	{
		this->_usersList.push_back(user);
		std::cout << CYAN << user.getNickname() << RESET
				  << " added to the Channel !" << std::endl;
		printContainer(this->_usersList);
	}
}

void	Channel::addUser( User & user, User & init )
{
	if (this->_inviteMode == true && this->checkRights(init) == true)
	{
			// check si invited
			deleteUserFromList(this->_invitedList, user);
	}
	else
	{
		this->_usersList.push_back(user);
		std::cout << CYAN << user.getNickname() << RESET
				  << " added to the Channel !" << std::endl;
		printContainer(this->_usersList);
	}
}

/*
void	Channel::inviteUser(User &init, std::string username, Server &serv)
{//l'user init invite un utilisateur (receiv)
	int	user_index = serv.findUsername(username);

	if (user_index == -1)
	{
		std::cout << "No User with this username" << std::endl;
		return ;
	}
	if ((this->getInvRight() == 1 && this->checkRights(init) == true)
		|| this->getInvRight() == 0)
	{
		;//Invite receiv if succeeded then addUser
		addUser(receiv);
	}
}

*/

void	Channel::changeTopic( User user, std::string & newTopic )
{
	newTopic.erase(0, 6);
	if (_topicMode == true) // Si seulement ops peuvent modifier topic
	{
		if (checkRights(user) == false)
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
		std::cerr << user.getNickname() << " set the channel's topic to \"" << newTopic << "\"\n";
	}
}

void	Channel::modeChange(User init, std::string str)//Call this when /MODE
{
	str.erase(0, 5);//only one space allowed after command
	std::string chan_name = firstWord(str);
	str.erase(0, chan_name.size());
      
	unsigned long   i = 0;
	while (i < str.size() && str[i] == ' ')
		i++;
	str.erase(0, i);
	if (this->checkRights(init) == true)
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
				this->setInviteMode(set);
				break;
			case 't':
				this->setTopicMode(set);
				break;
			case 'k':
			{
				std::string word = this->wordRemoveExtract(str, i);
				std::cout << "k: " << word << std::endl;
				if (set == 1)
					this->setKey(word);
				else if (set == 0)
					this->delKey();
				break;
			}
			case 'o':
			{
				std::string word = this->wordRemoveExtract(str, i);
				std::cout << "o: " << word << std::endl;
				if (set == 1)
					this->addOperator(word);
				else if (set == 0)
					this->delOperator(word);
				break;
			}
			case 'l':
			{
				std::string word = this->wordRemoveExtract(str, i);
				std::cout << "l: " << word << std::endl;
				char	*ptr;
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

bool	Channel::checkRights(User init) const
{//verifie si l'user est un op server puis op channel
	if (init.getOperator() == true)
		return true;
	else
	{
		for (std::list<User>::const_iterator it = this->_operatorsList.begin();
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

User	Channel::findUserinCh(std::string username)
{
	for (std::list<User>::iterator it = this->_usersList.begin();
		 it != this->_usersList.end(); it++)
	{
		if (it->getUsername() == username)
			return (*it);
	}
	return User();
}

std::list<User>::iterator	Channel::findUserI(std::string username, std::list<User> usersList)
{
	std::list<User>::iterator	it = usersList.begin();
	while (it != usersList.end())
	{
		if (it->getUsername() == username)
			return (it);
		it++;
	}
	return (it);
}
/*
bool	Channel::commandHandler(User &init, std::list<User> usersList, unsigned long code)
{
	switch (code)
	{
	case 421:
		std::cout << "Unknown command" << std::endl;//placeholder
		break;
	}
}

bool	Channel::commandHandler(User &init, std::list<User> usersList, std::string &str)
{//prend une str en param: "<command> <arg>"
//	if (str[0] != '/')
//		return (0);//need to display as a message if returning 0
//	str.erase(0, 1);
//	if (str[0] == ' ')
//		return (0);//need to display as a message if returning 0
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
		this->changeTopic(init, str);
		std::cout << init.getNickname() << "changed the topic of " << this->getChName()
				  << "to: " << this->getTopic() << std::endl;
		//doit afficher le nom du topic si success et si non changer
	}
	else if (firstWord(str) == "INVITE")
	{
		std::list<User>::iterator it = findUserI(wordRemoveExtract(str, 0), usersList);
		if (!(it == usersList.end()))
		{
			this->addUser(init, (*it));
//		std::cout << "check = " << str << std::endl;//debug
			str.erase(0, 7);
		}
		else
			std::cout << "User not found" << std::endl;
	}
	else if (firstWord(str) == "KICK")
	{
		std::list<User>::iterator it = findUserI(str, usersList);
		if (!(it == usersList.end()))
			this->kickUser((*it), init);
		else
			std::cout << "User not found" << std::endl;
	}
	return (1);
}// "/MODE  +l 10" ne doit pas marcher et pas d'undefined behavior
*/


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
