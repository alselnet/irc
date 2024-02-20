/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/18 13:07:40 by jthuysba          #+#    #+#             */
//   Updated: 2024/02/20 16:21:10 by ctchen           ###   ########.fr       //
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
		std::list< User >	_usersList;
		std::list<std::string>	_operatorsList;
//		std::list< User >	_operatorsList;
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
		
//   	std::list< User >			getOperatorsList( void ) const;
		// std::list<std::string>			&getOperatorsList( void ) const;
		std::string		getTopic( void ) const;
		std::string		getKey( void ) const;
		std::string		getChName( void) const;
		bool			getInviteMode( void ) const;
		bool			getTopicMode( void ) const;
		unsigned long		getUsersLimit( void ) const;
		unsigned long		getUsersCount( void ) const;
		std::list<User>::const_iterator	getUsersListBegin( void ) const;
		std::list<User>::const_iterator	getUsersListEnd( void ) const;
		std::list<std::string>::const_iterator	getOpListBegin( void ) const;
		std::list<std::string>::const_iterator	getOpListEnd( void ) const;
		std::list<std::string>::const_iterator	getInvitedListBegin();
		std::list<std::string>::const_iterator	getInvitedListEnd();
		std::list<std::string>::const_iterator	getInviterListBegin();
		std::list<std::string>::const_iterator	getInviterListEnd();
		// std::list< User >		*getUsersList( void ) const;
		std::list<User>::iterator			findUserinCh(std::string nickname);
		std::list<std::string>::iterator	findOpinCh(std::string nickname);
		std::string			getChanOperatorName(std::string nickname);

		void				addUser( std::list<User>::const_iterator user );
		void				delKey();
		void				delUser( std::list<User>::iterator user );
//		void				addOperator(std::list<User>::const_iterator user);
//		void				delOperator(std::list<User>::const_iterator user);
		void				addOperator(std::string nickname);
		void				delOperator(std::string nickname);
		void				addtoInviteList(std::string invitee, std::string inviter);
		void				changeTopic( std::string nickname, std::string & newTopic, bool is_op );
		void				modeMsg(const char *word, bool set, char flag, std::string username );
		bool				checkInvite( std::string nickname ) const;
		void				deleteInvited( std::string nickname );
		void				eraseFromUserList( std::list<User>::iterator target );
		bool				usersListEmpty( void ) const;
		bool				operatorsListEmpty( void ) const;
};

#endif
