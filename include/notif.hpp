/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   notif.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 03:53:56 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/08 12:53:30 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef NOTIF_HPP
#define NOTIF_HPP

#include <string>
#include <sstream>
#include "../include/irc.hpp"

class notif
{
	public:
			notif(std::string id_string, std::string command, std::string arguments, std::string additional);
			notif(notif &src);
			notif &operator=(notif &src);
			~notif(void);

			std::string	get_notif_message(void);
			const char*	get_cstr(void);	
			int			get_size(void);
			void			to_all(std::list<User> target_fds);
			void			to_client(unsigned int target_fd);

	private:
			std::string _notifMessage;

			notif(void);
};

#endif
