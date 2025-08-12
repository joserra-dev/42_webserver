/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joserra <joserra@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 01:36:38 by joserra           #+#    #+#             */
/*   Updated: 2025/08/09 01:36:43 by joserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
#define SERVER_HPP

#include "ServerConfig.hpp"
#include <map>
#include <string>
#include <vector>

class Server {
public:
	Server(const ServerConfig& config);
	~Server();

	static int createSocket(const std::string& ipPort);
	const std::vector<int>& getSockets() const;

private:
	ServerConfig _config;
	std::vector<int> _listenSockets;

	static std::map<std::string, int> _globalSocketMap;  // 🧠 Sockets compartidos
};

#endif
