/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jthuysba <jthuysba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/06 14:10:24 by aselnet           #+#    #+#             */
/*   Updated: 2024/03/05 16:20:37 by jthuysba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../include/irc.hpp"
# include "../include/Channel.hpp"
# include "../include/User.hpp"
# include "../include/Reply.hpp"

int	argcheck(char **argv)
{
	std::string port_arg;
	std::stringstream port_stream(argv[1]);
	port_stream >> port_arg;

	if (port_arg.size() > 5)
		return (-1);

	for (unsigned int i = 0; i != port_arg.size(); ++i)
	{
		if (!isdigit(port_arg[i]))
			return (-1);
	}

	std::stringstream 	pass_stream(argv[2]);
	std::string			pass_arg;
	pass_stream >> pass_arg;

	if (pass_arg.size() > 20)
		return (-1);

	for (unsigned int i = 0; i != pass_arg.size(); i++)
	{
		if (pass_arg[i] < 33 || pass_arg[i] > 126)
			return (-1);
	}
	return (0);
}

int main( int argc, char **argv)
{
	irc	irc_data;

	if (argc != 3)
	{
		std::cerr << "./bin/ircserv <listening port> <password>" << std::endl;
		return (0);
	}
	if (argcheck(argv) < 0)
	{
		std::cerr << "invalid port/password" << std::endl;
		std::cerr << "port : any number from 0 to 65535" << std::endl;
		std::cerr << "password : any  combination of 20 characters (excepted whitespace or control characters)" << std::endl;
		return (-1);
	}
	std::stringstream port_stream(argv[1]);
	port_stream >> irc_data.port;
	std::stringstream pass_stream(argv[2]);
	pass_stream >> irc_data.password;
	if (irc_data.port < 0 || irc_data.port > 65535)
	{
		std::cerr << "Invalid port." << std::endl;
		return (-1);
	}
	if (server_loop(&irc_data) < 0)
		return (-1);
	return (0);
}
