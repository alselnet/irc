// ************************************************************************** //
//                                                                            //
//                                                        :::      ::::::::   //
//   channel_command.hpp                                :+:      :+:    :+:   //
//                                                    +:+ +:+         +:+     //
//   By: ctchen <ctchen@student.42.fr>              +#+  +:+       +#+        //
//                                                +#+#+#+#+#+   +#+           //
//   Created: 2024/02/08 18:22:12 by ctchen            #+#    #+#             //
//   Updated: 2024/02/19 10:19:03 by ctchen           ###   ########.fr       //
//                                                                            //
// ************************************************************************** //

#ifndef CHANNEL_COMMAND_HPP
# define CHANNEL_COMMAND_HPP

void		kick_user(std::string *str, int *clientSockFd, irc *irc_data);
void		invite_user(std::string *str, int *clientSockFd, irc *irc_data);
void		mode_change(std::string *str, int *clientSockFd, irc *irc_data);
void		topic_change(std::string *str, int *clientSockFd, irc *irc_data);
void		channel_join(std::string *str, int *clientSockFd, irc *irc_data);
void		channel_leave(std::string *str, int *clientSockFd, irc *irc_data);

#endif
