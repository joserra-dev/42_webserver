// Server.hpp

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
