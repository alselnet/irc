/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:07:40 by jthuysba          #+#    #+#             */
//   Updated: 2024/02/22 18:13:42 by ctchen           ###   ########.fr       //
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
		unsigned long			_usersLimit;
		unsigned long			_usersCount;
		std::list< std::string >	_usersList;
		std::list< std::string >	_operatorsList;
		std::list< std::string >	_invitedList;
		std::list< std::string >	_inviterList;

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
		unsigned long		getUsersLimit( void ) const;
		unsigned long		getUsersCount( void ) const;
	std::list<std::string>::const_iterator	getUsersListBegin( void ) const;
	std::list<std::string>::const_iterator	getUsersListEnd( void ) const;
		std::list<std::string>::const_iterator	getOpListBegin( void ) const;
		std::list<std::string>::const_iterator	getOpListEnd( void ) const;
		std::list<std::string>::const_iterator	getInvitedListBegin();
		std::list<std::string>::const_iterator	getInvitedListEnd();
		std::list<std::string>::const_iterator	getInviterListBegin();
		std::list<std::string>::const_iterator	getInviterListEnd();
	std::list<std::string>::iterator			findUserinCh(std::string nickname);
		std::list<std::string>::iterator	findOpinCh(std::string nickname);
	std::string			getChanOperatorName(std::string nickname);

		void				addUser( std::string nickname );
		void				delKey();
		void				delUser( std::string nickname );
		void				addOperator(std::string nickname);
		void				delOperator(std::string nickname);
		void				addtoInviteList(std::string invitee, std::string inviter);
		void				changeTopic( std::string nickname, std::string & newTopic, bool is_op );
		void				modeMsg(const char *word, bool set, char flag, std::string username );
		bool				checkInvite( std::string nickname ) const;
		void				deleteInvited( std::string nickname );
		bool				usersListEmpty( void ) const;
		bool				operatorsListEmpty( void ) const;
		unsigned long	usersListSize( void) const;
};

#endif
