/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joscastr <joscastr@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 17:26:58 by joscastr          #+#    #+#             */
/*   Updated: 2025/08/02 00:38:13 by joscastr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"    // Incluye la clase que parsea el archivo de configuración
#include <iostream>            // Para imprimir en consola con std::cout y std::cerr
#include <stdexcept>           // Para manejar excepciones std::runtime_error

int main(){
	try{ // 🔐 Se inicia un bloque try para capturar errores durante el parsing.
		std::string filename;
		std::cout << "inserte el nombre del archivo *.conf:" << std::endl;
		std::cin >> filename;
		
		ConfigParser parser(filename); // 🧠 Se crea un objeto ConfigParser, pasándole el archivo de configuración mac.conf.
		
		parser.parse();
		/*⚙️ Se llama a la función parse(), que:

		Lee el archivo
		
		Elimina los comentarios
		
		Separa los bloques server
		
		Extrae la directiva listen y server_name*/

		const std::vector<ServerConfig>& servers = parser.getServers(); // 📦 Recupera la lista de servidores que se generaron al parsear el archivo.
		for (size_t i = 0; i < servers.size(); ++i) { //🔁 Este bucle imprime los puertos o direcciones indicadas en la directiva listen para cada servidor encontrado.
			std::cout << "Server #" << i << " listen: ";
			for (size_t j = 0; j < servers[i].listen.size(); ++j)
				std::cout << servers[i].listen[j] << " ";
			std::cout << std::endl;
			
			std::cout << "Server #" << i << " server_names: ";
			for (size_t j = 0; j < servers[i].serverNames.size(); ++j)
				std::cout << servers[i].serverNames[j] << " ";
			std::cout << std::endl;

			}	
			
		
		std::cout << "Config OK. Servidores encontrados: " // ✅ Muestra un mensaje de éxito y cuántos bloques server fueron parseados correctamente.
			<< parser.getServers().size() << std::endl;
	} catch (std::exception& e) { //🚨 Si ocurre un error (como archivo no encontrado o error de sintaxis), se captura y se imprime el mensaje correspondiente.
		std::cerr << e.what() << std::endl;
	}
	

	return 0;
}
/* "RESULTADO SI TODO ESTA BIEN" 
Server #0 listen: 8080 127.0.0.1:8080
Config OK. Servidores encontrados: 1
 */