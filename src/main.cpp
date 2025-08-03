#include "ConfigParser.hpp"
#include "Server.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

int main() {
	try {
		std::string filename;
		std::cout << "Inserte el nombre del archivo *.conf:" << std::endl;
		std::cin >> filename;

		ConfigParser parser(filename);
		parser.parse();

		const std::vector<ServerConfig>& serverConfigs = parser.getServers();
		std::vector<Server*> servers;

		for (size_t i = 0; i < serverConfigs.size(); ++i) {
			Server* server = new Server(serverConfigs[i]);
			servers.push_back(server);
		}

		for (size_t i = 0; i < servers.size(); ++i) {
			const std::vector<int>& sockets = servers[i]->getSockets();
			std::cout << "Server #" << i << " socket abierto con el numero: ";
			for (size_t j = 0; j < sockets.size(); ++j) {
				std::cout << sockets[j] << " ";
			}
			std::cout << std::endl;
		}

		std::cout << "Sockets inicializados correctamente. Servidores activos: "
		          << servers.size() << std::endl;

		for (size_t i = 0; i < servers.size(); ++i) {
			delete servers[i];
		}
	} catch (const std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
