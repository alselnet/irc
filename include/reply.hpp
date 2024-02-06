/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 18:29:48 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/06 11:38:35 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_HPP
#define REPLY_HPP

#include <string>
#include <sstream>
#include "../include/irc.hpp"

class reply
{
	public:
			reply(int cmd_nb, std::string target_username, std::string additional);
			reply(reply &src);
			reply &operator=(reply &src);
			~reply(void);

			std::string	get_reply_message(void);
			const char*	get_cstr(void);	
			int			get_size(void);
			void		to_client(unsigned int target_fd);

	private:
			std::string _replyMessage;
			reply(void);
};

#endif