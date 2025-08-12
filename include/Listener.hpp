/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Listener.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joserra <joserra@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 01:36:58 by joserra           #+#    #+#             */
/*   Updated: 2025/08/09 01:37:01 by joserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LISTENER_HPP
#define LISTENER_HPP

#include "Server.hpp"
#include <vector>
#include <poll.h>

class Listener {
	public:
		Listener(const std::vector<Server*>& servers);
		~Listener();

		void run(); //Bucle principal de eventos

		bool isListeningSocket(int fd) const;

	private:
		std::vector<pollfd> _pollfds;
		std::vector<Server*> _servers;

		void setupPollFDs();					//Inincializa pollfd con los sockets de escucha
		void handleNewConnection(int fd); 	// MAneja nuevas conexiones entrantes
};

#endif