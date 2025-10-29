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
#include <cerrno>
#include <fcntl.h>   // ✅ fcntl(), F_GETFL, F_SETFL, O_NONBLOCK



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

	// Validar puerto
	if (port <= 0 || port > 65535) {
		std::cerr << "Error: Puerto inválido: " << port << std::endl;
		return -1;
	}

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		perror("socket");
		return -1;
	}

	int opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
		perror("setsockopt");

	// 🔹 Hacer socket no bloqueante
	int flags = fcntl(sockfd, F_GETFL, 0);
	fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

	sockaddr_in addr;
	std::memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (inet_pton(AF_INET, ip.c_str(), &(addr.sin_addr)) <= 0) {
		std::cerr << "Error: dirección IP inválida: " << ip << std::endl;
		close(sockfd);
		return -1;
	}

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

	std::cout << "✅ Escuchando en " << ip << ":" << port << " (FD: " << sockfd << ")\n";
	return sockfd;
}

const std::vector<int>& Server::getSockets() const {
	return _listenSockets;
}
