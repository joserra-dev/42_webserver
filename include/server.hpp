/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joscastr <joscastr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/02 23:04:00 by joscastr          #+#    #+#             */
/*   Updated: 2025/08/02 23:17:06 by joscastr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "ServerConfig.hpp"
#include <vector>
#include <string>
#include <netinet/in.h> // sockaddr_in
#include <sys/socket.h> // socket functions
#include <unistd.h>     // close()
#include <map>

class Server {
	public:
		Server(const ServerConfig& config);
		~Server();

		void initSockets(); // Crea y configura los Sockets
		void start(); // Llama a bind(), listen(), etc.

		const std::vector<int>& getSockets() const;

	private:
		ServerConfig _config;
		std::vector<int> _listenSockets;
		std::vector<struct sockaddr_in> _addrs;
	
		int createSocket(const std::string& ipPort);
	};


#endif