/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joserra <joserra@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 01:35:42 by joserra           #+#    #+#             */
/*   Updated: 2025/08/09 01:35:44 by joserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>


std::map<std::string, int> Server::_globalSocketMap;  // Definición

Server::Server(const ServerConfig& config) : _config(config) {
	for (size_t i = 0; i < _config.listen.size(); i++) {
		const std::string& ipPort = _config.listen[i];
		if (_globalSocketMap.find(ipPort) == _globalSocketMap.end()) {
			int sock = createSocket(ipPort);
			if (sock != -1) {
				_globalSocketMap[ipPort] = sock;
				std::cout << "Socket creado en: " << ipPort << " -> FD: " << sock << std::endl;
			}
		} else {
			std::cout << "⚠️  Socket ya existente reutilizado: " << ipPort
			          << " -> FD: " << _globalSocketMap[ipPort] << std::endl;
		}
		_listenSockets.push_back(_globalSocketMap[ipPort]);
	}
}

Server::~Server() {
	// ⚠️ No cerramos los sockets aquí, porque son compartidos por múltiples servidores.
}

int Server::createSocket(const std::string& ipPort) {
	std::string ip = "0.0.0.0";
	int port = 0;

	size_t colon = ipPort.find(':');
	if (colon != std::string::npos) {
		ip = ipPort.substr(0, colon);
		port = std::atoi(ipPort.substr(colon + 1).c_str());
	} else {
		port = std::atoi(ipPort.c_str());
	}

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return -1;
	}

	int opt = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &(addr.sin_addr));

	if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		perror("bind");
		close(sockfd);
		return -1;
	}

	if (listen(sockfd, 128) < 0) {
		perror("listen");
		close(sockfd);
		return -1;
	}

	return sockfd;
}

const std::vector<int>& Server::getSockets() const {
	return _listenSockets;
}
