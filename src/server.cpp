#include "Server.hpp"         // Incluye la definición de la clase Server.
#include <iostream>           // Para usar std::cout, std::cerr.
#include <cstring>            // Para usar std::memset.
#include <arpa/inet.h>        // Para funciones como inet_pton, sockaddr_in.
#include <sstream>            // Para manipulación de strings (aunque aquí no se usa directamente).

// Constructor: inicializa la clase con una configuración de servidor.
Server::Server(const ServerConfig& config) : _config(config) {}

// Destructor: cierra todos los sockets que fueron abiertos.
Server::~Server() {
    for (size_t i = 0; i < _listenSockets.size(); ++i)
        close(_listenSockets[i]); // Cierra cada socket abierto.
}

// Método que inicializa todos los sockets definidos en la configuración.
void Server::initSockets() {
    for (size_t i = 0; i < _config.listen.size(); ++i) {
        int sock = createSocket(_config.listen[i]); // Crea un socket por cada entrada listen.
        if (sock != -1) {
            _listenSockets.push_back(sock); // Si se crea correctamente, lo guarda.
            std::cout << "Socket creado en: " << _config.listen[i] << std::endl;
        }
    }
}

// Método auxiliar para crear y configurar un socket desde un string "IP:PORT" o "PORT".
int Server::createSocket(const std::string& ipPort) {
    std::string ip = "0.0.0.0"; // IP por defecto si no se especifica.
    int port = 0;

    size_t colon = ipPort.find(':'); // Busca si el string tiene ':' (IP:PORT).
    if (colon != std::string::npos) {
        ip = ipPort.substr(0, colon);                           // Extrae la IP.
        port = std::atoi(ipPort.substr(colon + 1).c_str());     // Extrae el puerto como entero.
    } else {
        port = std::atoi(ipPort.c_str()); // Si no hay IP, toma solo el puerto.
    }

    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // Crea el socket TCP.
    if (sockfd < 0) {
        perror("socket"); // Imprime error si falla.
        return -1;
    }

    int opt = 1;
    // Permite reusar la dirección (útil al reiniciar servidor).
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr;
    std::memset(&addr, 0, sizeof(addr)); // Limpia la estructura.
    addr.sin_family = AF_INET;          // IPv4.
    addr.sin_port = htons(port);        // Convierte puerto al formato de red.
    inet_pton(AF_INET, ip.c_str(), &(addr.sin_addr)); // Convierte IP de string a binario.

    if (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind");     // Error al enlazar el socket a la dirección.
        close(sockfd);      // Cierra el socket si falla.
        return -1;
    }

    if (listen(sockfd, 128) < 0) {
        perror("listen");   // Error al poner el socket en escucha.
        close(sockfd);      // Cierra el socket si falla.
        return -1;
    }

    return sockfd; // Devuelve el descriptor del socket creado correctamente.
}

// Devuelve la lista de descriptores de socket en escucha.
const std::vector<int>& Server::getSockets() const {
    return _listenSockets;
}
