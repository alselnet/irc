/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:07:40 by jthuysba          #+#    #+#             */
/*   Updated: 2024/02/08 15:14:14 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
	#define CHANNEL_HPP

#include "irc.hpp"

class Channel
{
	private:

		std::string	_name;
		std::string	_topic;
		std::string	_key;
		bool			_inviteMode;
		bool			_topicMode;
		unsigned int			_usersLimit;
		std::list< User >	_usersList;
		std::list< User >	_operatorsList;
		std::list< User >	_invitedList;

	public:

		Channel( void );
		~Channel( void );
		Channel( const std::string name );

		void	setChName( std::string name );
		void	setTopic( std::string topic );
		void	setKey( std::string key );
		void	setInviteMode( bool status );
		void	setTopicMode( bool status );
		void	setUsersLimit( unsigned int limit );
		
		std::string		getTopic( void ) const;
		std::string		getKey( void ) const;
		std::string		getChName( void) const;
		bool			getInviteMode( void ) const;
		bool			getTopicMode( void ) const;
		unsigned int	getUsersLimit( void ) const;
		std::list<User>	getUsersList( void ) const;

		void		addUser( User & user);
		void		addUser( User & user, User & init );
		void		kickUser( User & user, User & init );
		void		delKey();
		void		addOperator(std::string username);
		void		delOperator(std::string username);
		void		modeChange(User init, std::string str);
		bool		checkRights(User init) const;
		std::string wordRemoveExtract(std::string &str, unsigned long i);
		std::string	firstWord(std::string str);
		User    	findUserinCh(std::string username);
//		bool    	commandHandler(User &init, std::list<User> usersList, std::string &str);
		void		changeTopic( User user, std::string & newTopic );

	std::list<User>::iterator	findUserI(std::string username, std::list<User> usersList);//temp, c'est pas pertinent de le mettre ici findUser cherche dans la liste d'user du server
};

#endif
