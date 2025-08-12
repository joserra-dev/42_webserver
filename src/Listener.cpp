/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joserra <joserra@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 01:34:50 by joserra           #+#    #+#             */
/*   Updated: 2025/08/12 23:50:00 by joserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "Listener.hpp"
#include <unistd.h>   // close()
#include <iostream>   // std::cerr
#include <sys/socket.h> // accept()
#include <netinet/in.h>
#include <cstdio>


Listener::Listener(const std::vector<Server*>& servers)
	: _servers(servers) {
	setupPollFDs();
}

Listener::~Listener() {
	// No cerramos los sockets aquí porque los Server los gestionan
}

void Listener::setupPollFDs() {
	for (size_t i = 0; i < _servers.size(); ++i) {
		const std::vector<int>& sockets = _servers[i]->getSockets();
		for (size_t j = 0; j < sockets.size(); ++j) {
			pollfd pfd;
			pfd.fd = sockets[j];
			pfd.events = POLLIN;
			pfd.revents = 0;
			_pollfds.push_back(pfd);
		}
	}
}

void Listener::run() {
    std::cout << "🎧 Entrando en el bucle de eventos con poll()...\n";

    while (true) {
        int ret = poll(&_pollfds[0], _pollfds.size(), -1);
        if (ret < 0) {
            perror("poll");
            break;
        }

        for (size_t i = 0; i < _pollfds.size(); ++i) {
            if (_pollfds[i].revents & POLLIN) {
                if (isListeningSocket(_pollfds[i].fd)) {
                    // Nueva conexión entrante
                    int clientFd = accept(_pollfds[i].fd, NULL, NULL);
                    if (clientFd < 0) {
                        perror("accept");
                        continue;
                    }
                    std::cout << "📡 Nueva conexión aceptada (FD: " << clientFd << ")\n";

                    // Añadir cliente a pollfds
                    struct pollfd clientPfd;
                    clientPfd.fd = clientFd;
                    clientPfd.events = POLLIN;
                    clientPfd.revents = 0;
                    _pollfds.push_back(clientPfd);
                } else {
                    // Aquí procesaríamos datos de un cliente ya conectado
                    char buffer[1024];
                    int bytes = recv(_pollfds[i].fd, buffer, sizeof(buffer) - 1, 0);
                    if (bytes <= 0) {
                        close(_pollfds[i].fd);
                        _pollfds.erase(_pollfds.begin() + i);
                        i--;
                    } else {
                        buffer[bytes] = '\0';
                        std::cout << "💬 Mensaje de cliente: " << buffer << "\n";
                    }
                }
				// Respuesta HTTP mínima
		std::string response = "HTTP/1.1 200 OK\r\n"
                       "Content-Type: text/plain\r\n"
                       "Content-Length: 13\r\n"
                       "\r\n"
                       "Hola, mundo!";
		send(_pollfds[i].fd, response.c_str(), response.size(), 0);

		// Cerramos la conexión después de responder
		close(_pollfds[i].fd);
		_pollfds.erase(_pollfds.begin() + i);
		i--;
					}
        }
    }
}

bool Listener::isListeningSocket(int fd) const {
    for (size_t i = 0; i < _servers.size(); ++i) {
        const std::vector<int>& sockets = _servers[i]->getSockets();
        for (size_t j = 0; j < sockets.size(); ++j) {
            if (sockets[j] == fd)
                return true;
        }
    }
    return false;
}


void Listener::handleNewConnection(int fd) {
	sockaddr_in clientAddr;
	socklen_t addrLen = sizeof(clientAddr);
	int clientFd = accept(fd, (sockaddr*)&clientAddr, &addrLen);

	if (clientFd < 0) {
		perror("accept");
		return;
	}

	std::cout << "🆕 Nueva conexión aceptada: FD = " << clientFd << std::endl;

	// Más adelante, pasar este FD a una clase Connection para gestionarlo o
}
