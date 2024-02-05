/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 18:29:48 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/02 18:58:55 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

class message
{
	private:

	public:
			message(void);
			message(const std::string tmp);
			message(const message &src);

			message	&operator=(const message &src);
			~message(void);
};



#endif