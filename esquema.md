webserv/
├── src/
│   ├── main.cpp
│   ├── config/
│   │   ├── ConfigParser.hpp / .cpp
│   │   ├── Tokenizer.hpp / .cpp
│   │   ├── Config.hpp / .cpp       # estructura del config final
│   ├── server/
│   │   ├── Server.hpp / .cpp
│   │   ├── Connection.hpp / .cpp
│   │   ├── Request.hpp / .cpp
│   │   ├── Response.hpp / .cpp
│   ├── utils/
│   │   ├── Logger.hpp / .cpp
│   │   ├── Utils.hpp / .cpp
│
├── www/                   # root por defecto
│   ├── index.html
│   └── 404.html
├── conf/
│   └── default.conf
├── Makefile
└── README.md


🧠 COMPONENTES PRINCIPALES
1. 🧾 Parser de configuración (ConfigParser)
Lee y tokeniza el archivo .conf.

Verifica estructura (server, location, {}, ;...).

Valida cada directiva (listen, server_name, etc.).

Crea una estructura tipo ServerConfig con toda la información.

2. 🌐 Servidor (Server)
Crea sockets con socket(), bind(), listen().

Usa poll() para manejar múltiples conexiones (evita select() por límite de FDs).

Acepta conexiones con accept(), y las distribuye a Connection.

3. 🤝 Conexiones (Connection)
Recibe datos (requests) de clientes.

Llama al parser de Request.

Genera una Response adecuada.

Envía la respuesta al cliente.

4. 📬 Request + Response
Request parsea el método, headers, body.

Response genera respuesta con código HTTP, headers, body.

Manejo de errores (404, 500, 405, etc.).

Servir archivos estáticos (read()).

Ejecutar CGI si aplica.

5. ⚙️ Módulos extra
Logger: log de eventos, errores y peticiones.

Utils: conversión de unidades, validación de paths, etc.

🔁 FLUJO DE FUNCIONAMIENTO
1. Leer archivo de configuración
2. Crear sockets según los bloques `listen`
3. Esperar conexiones (poll)
4. Leer petición → parsear
5. Buscar el bloque `server` / `location` que le corresponde
6. Generar respuesta:
    - Archivos estáticos
    - CGI
    - Error
    - Redirección
7. Enviar respuesta
8. Cerrar o mantener conexión


🪜 ETAPAS DE IMPLEMENTACIÓN
Etapa	Objetivo	Estado
✅ 1	Crear estructura de carpetas y Makefile	
✅ 2	Diseñar clases ServerConfig, LocationConfig	
⏳ 3	Implementar ConfigParser básico y validar .conf	
⏳ 4	Crear clase Server, inicializar sockets	
⏳ 5	Añadir poll() y bucle de espera	
⏳ 6	Parsear y responder peticiones básicas (GET)	
⏳ 7	Soporte de POST, manejo de body y límite de tamaño	
⏳ 8	Manejo de errores (404, 500, etc.)	
⏳ 9	Implementar autoindex	
⏳10	Implementar CGI (Python, PHP)	
⏳11	Implementar redirecciones	
⏳12	Validaciones y limpieza de código	
⏳13	Testing, benchmarking, checker


ChatGPT me propone lo siguiente:

✍️ ¿Cómo seguimos?
Te propongo que empecemos por aquí:

Diseñar la estructura de clases del parser:

ServerConfig

LocationConfig

ConfigParser

Y luego implementamos paso a paso:

Empezar leyendo el archivo .conf

Tokenizar líneas

Validar estructura y directivas