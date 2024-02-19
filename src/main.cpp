/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aselnet <aselnet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:10:24 by aselnet           #+#    #+#             */
/*   Updated: 2024/02/19 18:18:50 by aselnet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/Channel.hpp"
# include "../include/User.hpp"
# include "../include/Reply.hpp"

int	argcheck(char **argv)
{
	std::string port_arg;
	std::stringstream iss(argv[1]);
	iss >> port_arg;

	for (unsigned int i = 0; i < port_arg.size(); ++i)
	{
		if (!isdigit(port_arg[i]))
			return (-1);
	}
	return (0);
}

int main( int argc, char **argv)
{
	irc	irc_data;

	if (argc != 2)
	{
		std::cerr << "./bin/ircserv <listening port>" << std::endl;
		return (0);
	}
	if (argcheck(argv) < 0)
		return (-1);
	std::stringstream iss(argv[1]);
	iss >> irc_data.port;
	if (irc_data.port < 0 || irc_data.port > 65535)
	{
		std::cerr << "Invalid port." << std::endl;
		return (-1);
	}
	if (server_loop(&irc_data) < 0)
		return (-1);
	return (0);
}