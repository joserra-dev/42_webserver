#include "ConfigParser.hpp"         // Incluye el header de la clase ConfigParser
#include <fstream>                  // Para trabajar con archivos (ifstream)
#include <sstream>                  // Para manejar buffers de texto
#include <iostream>                 // Para imprimir errores o debug
#include <stdexcept>   
#include "ServerConfig.hpp"            // Para lanzar excepciones como runtime_error

// Constructor que recibe la ruta del archivo .conf
ConfigParser::ConfigParser(const std::string& filepath)
	: _filepath(filepath) {}       // Guarda la ruta del archivo como atributo

// Función principal para parsear la configuración
void ConfigParser::parse() {
	readFile();                    // Leer el archivo a string (_fileContent)
	removeComments();              // Eliminar los comentarios (líneas con #)
	splitServerBlocks();           // Separar bloques server {...}
}

// Lee el contenido completo del archivo de configuración
void ConfigParser::readFile() {
	std::ifstream file(_filepath.c_str());         // Abrir archivo
	if (!file.is_open())                           // Verificar si se abrió correctamente
		throw std::runtime_error("Error: Cannot open config file."); // Lanzar error si falla

	std::stringstream buffer;
	buffer << file.rdbuf();                         // Leer todo el contenido en buffer
	_fileContent = buffer.str();                    // Guardar como string en _fileContent
	file.close();                                    // Cerrar el archivo
}

// Elimina comentarios de tipo '#' en cada línea
void ConfigParser::removeComments() {
	std::string cleaned;
	std::istringstream stream(_fileContent);       // Convertir string a stream línea por línea
	std::string line;

	while (std::getline(stream, line)) {           // Leer cada línea
		size_t commentPos = line.find('#');        // Buscar la posición del '#'
		if (commentPos != std::string::npos)
			line = line.substr(0, commentPos);     // Cortar el comentario
		cleaned += line + '\n';                    // Agregar la línea limpia al string final
	}

	_fileContent = cleaned;                        // Reemplazar el contenido original sin comentarios
}

// Divide el contenido en bloques de configuración de servidor
void ConfigParser::splitServerBlocks() {
	size_t pos = 0;                                // Posición para recorrer el string

	while ((pos = _fileContent.find("server", pos)) != std::string::npos) {
		size_t braceStart = _fileContent.find("{", pos);          // Buscar la llave de apertura
		size_t braceEnd = _fileContent.find("}", braceStart);     // Buscar la llave de cierre

		if (braceStart == std::string::npos || braceEnd == std::string::npos)
			throw std::runtime_error("Error: Invalid server block."); // Error si no hay llaves

		// Extraer contenido entre las llaves
		std::string blockContent = _fileContent.substr(braceStart + 1, braceEnd - braceStart - 1);
		std::istringstream blockStream(blockContent);     // Stream para leer línea por línea
		std::string line;

		ServerConfig server;      // Crear instancia de ServerConfig (vacía)

		while (std::getline(blockStream, line)) {
			std::istringstream lineStream(line);           // Stream para separar palabras
			std::string directive;
			lineStream >> directive;                       // Leer la primera palabra (la directiva)

			if (directive == "listen") {                   // Si la directiva es listen
				std::string value;
				while (lineStream >> value) {              // Leer todos los valores siguientes
					if (value.back() == ';')               // Quitar punto y coma si está al final
						value.pop_back();
					server.addListen(value);               // Guardar valor en el objeto server
				}
			}
			else if (directive == "server_name") {
				std::string value;
				while (lineStream >> value) {
					if (value.back() == ';')
						value.pop_back();
					server.addServerName(value);
				}
			}
			else if (directive == "root") {
				std::string value;
				lineStream >> value;
				if (!value.empty() && value.back() == ';')
					value.pop_back();
				server.index = value;
			}
			else if (directive == "index") {
				std::string value;
				lineStream >> value;
				if(!value.empty() && value.back() == ';' )
					value.pop_back();
				server.index = value;
			}
			

			// Aquí luego vendrán otros if para otras directivas
		}

		_servers.push_back(server);                        // Guardar el servidor en la lista
		pos = braceEnd + 1;                                // Mover posición para buscar el siguiente bloque
	}
}

// Función para acceder a la lista de servidores parseados
const std::vector<ServerConfig>& ConfigParser::getServers() const {
	return _servers;
}
