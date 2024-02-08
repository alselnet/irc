/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   message.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/02 18:29:48 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/06 16:34:30 by jthuysba         ###   ########.fr       */
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