/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joserra <joserra@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/09 01:35:32 by joserra           #+#    #+#             */
/*   Updated: 2025/08/09 01:35:35 by joserra          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

int main() {
    try {
        std::string filename;
        std::cout << "Inserte el nombre del archivo *.conf: ";
        std::getline(std::cin, filename);
        if (filename.empty()) {
            throw std::runtime_error("Nombre de archivo vacío");
        }

        ConfigParser parser(filename);
        parser.parse();

        const std::vector<ServerConfig>& servers = parser.getServers();
        for (size_t i = 0; i < servers.size(); ++i) {
            std::cout << "Server #" << i << " listen: ";
            for (size_t j = 0; j < servers[i].listen.size(); ++j)
                std::cout << servers[i].listen[j] << " ";
            std::cout << '\n';

            std::cout << "Server #" << i << " server_names: ";
            for (size_t j = 0; j < servers[i].serverNames.size(); ++j)
                std::cout << servers[i].serverNames[j] << " ";
            std::cout << '\n';

            // Imprimir root como ruta completa (no caracteres)
            std::cout << "Server #" << i << " root: " << servers[i].root << '\n';

            // Imprimir índices (vector de strings)
            std::cout << "Server #" << i << " index: ";
            for (size_t l = 0; l < servers[i].index.size(); ++l)
                std::cout << servers[i].index[l] << " ";
            std::cout << '\n';
        }

        std::cout << "Config OK. Servidores encontrados: " << servers.size() << '\n';
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}