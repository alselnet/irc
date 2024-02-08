/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reply.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 18:29:48 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/08 17:40:31 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REPLY_HPP
#define REPLY_HPP

#include <string>
#include <sstream>
#include "../include/irc.hpp"

class Reply
{
	public:
			Reply(int cmd_nb, std::string target_username, std::string additional);
			Reply(Reply &src);
			Reply &operator=(Reply &src);
			~Reply(void);

			std::string	get_reply_message(void);
			const char*	get_cstr(void);	
			int			get_size(void);
			void			to_client(unsigned int target_fd);

	private:
			std::string _replyMessage;
			Reply(void);
};

#endif