# 42_webserver
🧠 Aprendizaje previo:
Cómo NGINX usa los bloques server {} y location {}

Análisis de configuración basado en recursión

Tokenización, coincidencia de corchetes

🔧 Acciones:
Crear un analizador léxico/tokenizador

Crear un analizador recursivo que genere un árbol de configuraciones

Validar la herencia de las configuraciones (p. ej., raíz, índice del servidor a la ubicación)

✅ Paso 4: Configurar los sockets del servidor
Objetivo: Vincular los sockets según el archivo de configuración.

🧠 Aprendizaje previo:
socket(), bind(), listen(), accept()

poll() vs. select() vs. epoll() (seleccione poll() para portabilidad POSIX)

Gestión de múltiples puertos (p. ej., múltiples bloques de escucha)

🔧 Acciones:
Recorrer las configuraciones del servidor → crear sockets de escucha

Usar poll() para monitorizar las conexiones entrantes

✅ Paso 5: Implementar la gestión de conexiones
Objetivo: Aceptar clientes y gestionar solicitudes sin bloqueos.

🧠 Aprendizaje previo:
Sockets sin bloqueos

Reutilizar descriptores de archivo y búferes

Gestionar múltiples clientes eficientemente

🔧 Acciones:
Aceptar nuevas conexiones

Leer desde sockets, analizar solicitudes

Almacenar lecturas parciales (fragmentadas o de gran tamaño)

Detectar desconexiones de clientes

✅ Paso 6: Analizar solicitudes HTTP
Objetivo: Desarrollar un analizador de solicitudes robusto.

🧠 Aprendizaje previo:
Formato de solicitud HTTP: Línea de solicitud, encabezados, cuerpo opcional

Codificación de transferencia fragmentada

Decodificación de URL y validación de encabezados

🔧 Acciones:
Usar una máquina de estados para analizar las líneas, encabezados y cuerpo de la solicitud

Gestionar casos extremos: solicitudes mal formadas, métodos no compatibles

✅ Paso 7: Enrutar la solicitud
Objetivo: Asociar la solicitud al servidor y bloque de ubicación correctos.

🧠 Aprendizaje previo:
Lógica de enrutamiento de NGINX: listen, server_name, location /foo

Cómo funciona la coincidencia con comodines (location /path/ {} vs. coincidencia exacta)

🔧 Acciones:
Implementar la lógica para asociar las solicitudes con el bloque de configuración más adecuado

Aplicar valores heredados (p. ej., raíz, métodos, índice)

✅ Paso 8: Generar respuestas
Objetivo: Generar respuestas para GET, POST y DELETE.

🧠 Aprenda antes:
Cómo funciona el servicio de archivos (abrir/leer/enviar)

Comportamiento de autoindexación (cuando falta el archivo de índice)

Códigos de estado HTTP comunes

🔧 Acciones:
Para GET: servir un archivo estático o autoindexar un directorio

Para POST: gestionar los datos del formulario y almacenarlos si es necesario

Para DELETE: eliminar el archivo y devolver el código correcto

Añadir encabezados (Tipo de contenido, Longitud del contenido, Fecha, etc.)

✅ Paso 9: Implementar el manejo de CGI
Objetivo: Admitir la ejecución de scripts CGI.

🧠 Aprendizaje previo:
Variables de entorno CGI (REQUEST_METHOD, SCRIPT_FILENAME, etc.)

Cómo bifurcar(), ejecutar() y configurar tuberías

Tiempos de espera y almacenamiento en búfer de entrada

🔧 Acciones:
Al coincidir la ubicación, bifurcar un proceso para ejecutar la CGI

Enrutar el cuerpo de la solicitud a la entrada estándar (stdin) y leer desde la salida estándar (stdout)

Analizar y reenviar encabezados en la salida CGI

✅ Paso 10: Gestionar errores y códigos de estado
Objetivo: Mostrar páginas de estado y errores personalizadas correctas.

🧠 Aprendizaje previo:
Comportamiento de la directiva error_page de NGINX

Qué significan los códigos 404, 405, 500, 413, etc.

🔧 Acciones:
Devolver códigos de error correctos

Si error_page 404 /404.html; Si está definido, mostrar esa página.

Si no está definido, se mostrará un mensaje de error genérico.

✅ Paso 11: Pruebas exhaustivas
Objetivo: Garantizar la robustez y un comportamiento similar al de NGINX.

🧪 Acciones:
Usar curl, ab o Postman para probar:

Subidas fragmentadas

Ejecución de CGI

Solicitudes no válidas

Múltiples conexiones simultáneas

Escribir pruebas para casos extremos de análisis de configuración.

✅ Paso 12: Optimizar y pulir
Objetivo: Gestionar casos extremos y escalar con fluidez.

⚙️ Consejos:
Implementar la gestión de tiempos de espera (p. ej., 408)

Limita el tamaño máximo del cuerpo (413)

Eliminar procesos CGI no deseados

Registrar solicitudes y errores como access.log/error.log

✅ Paso 13: Bonus / Avanzado
Si quieres ir más allá:

Admitir redirecciones (devolver 301 ...)

Usar epoll (Linux) o kqueue (BSD/Mac)

Compatibilidad con IPv6

Añadir funciones de proxy inverso (simular proxy_pass)

🚀 Consejo final
Enfoque de la fase
Semana 1: HTTP, análisis de configuración, sockets
Semana 2: análisis de solicitudes, respuesta, CGI
Semana 3: gestión de errores, casos extremos
Semana 4: optimización, pruebas, perfeccionamiento

Inspírate en NGINX, pero no sobredimensiones la ingeniería. Céntrate primero en cumplir con los requisitos del sujeto y luego añade complejidad.

¿Te gustaría un diagrama o una estructura de código inicial para este plan?


Paso 1: Comprender los requisitos del proyecto


Objetivo: Servidor web HTTP 1.1 en C++98.

Admite múltiples clientes.

Analiza un archivo de configuración.

Sirve archivos estáticos y ejecuta CGI.

Maneja métodos HTTP comunes (GET, POST, DELETE).

Gestiona errores correctamente.
1. NÚCLEO DE RED: Crea sockets de servidor, usa poll() y acepta conexiones sin bloqueo.
2. ANALIZADOR DE CONFIGURACIÓN: Analiza la configuración al estilo NGINX. Soporte de servidor, ubicación, escucha, etc., y almacenamiento de relaciones de herencia.
3. MANEJO DE SOLICITUDES -> Analiza solicitudes HTTP sin procesar (Línea de solicitud + Encabezados + Cuerpo). Gestiona GET, POST, DELETE. Almacena solicitudes recibidas parcialmente si es necesario.
4. MANEJADOR CGI -> Usa fork()/execve() para ejecutar scripts. Usa pipe() y dup2() para capturar la entrada/salida. Establece variables de entorno a partir de la solicitud.
5. GENERADOR DE RESPUESTAS -> Genera una respuesta HTTP/1.1 válida con estado, encabezados y cuerpo. Gestiona codificación fragmentada si es necesario. Admite conexión: keep-alive o close.
6. MANEJO DE ERRORES -> Detecta y gestiona
Solicitudes malformadas, compatibilidad con error_page personalizado por código de estado, retorno de respuestas de error HTTP válidas.
7. LÓGICA DE ENRUTAMIENTO -> Emparejar la solicitud con el servidor y la ubicación correctos según el puerto, el host y la ruta de la solicitud. Aplicar valores de configuración heredados.

QUÉ APRENDER:
Buscar:
- Características requeridas: Servir archivos estáticos (GET), gestionar métodos POST y DELETE, ejecución CGI, páginas de error personalizadas, múltiples clientes, análisis de bloques de ubicación y servidor, restricciones de métodos. Comportamiento prohibido: Sin fugas de memoria, sin fallos de segmentación/comportamiento indefinido, sin bucles infinitos ni bloqueos, sin usar llamadas al sistema prohibidas, sin usar bibliotecas de alto nivel.

Restricciones: Solo C++98, sin contenedores STL excepto los permitidos, sin usar std::thread ni E/S asíncronas, sin boost.

Criterios de evaluación: compilación sin advertencias/errores, fugas de memoria: probar con valgrind, comportamiento correcto en casos extremos, respetar las especificaciones HTTP/1.1, gestionar correctamente la entrada malformada, legibilidad, estructura y organización del código.

Ejemplos o casos extremos: Se proporcionaron ejemplos de entrada/salida, sugerencias para casos extremos, comportamientos específicos para la compatibilidad, comportamiento predeterminado requerido cuando la configuración está incompleta.

Características obligatorias:

Funcionalidades principales: GET, POST, DELETE

Análisis de archivos de configuración: Analizar el archivo de configuración, crear estructuras internas para los bloques de servidor y ubicación, aplicar la lógica de herencia.

Compatibilidad con CGI: Compatibilidad con scripts cgi-bin. Uso de fork, execve, dup2 y variables de entorno.
- Gestión de errores: códigos de estado y páginas de error personalizadas.
- Codificación de transferencia fragmentada: Gestión de solicitudes/respuestas fragmentadas entrantes y salientes (especialmente para cuerpos POST).
- Tiempos de espera y robustez: Cierre los tiempos de espera de las conexiones de cliente. Evita fugas de memoria o fallos en solicitudes mal formadas.

RESTRICCIONES Y LIMITACIONES
- Solo C++98
- Sin bibliotecas externas
- Debe ser compatible con varios clientes (poll())
- Todo es personalizado
- Comportamiento determinista

MÓDULOS INTERNOS RECOMENDADOS
- ServerManager -> Escucha, acepta conexiones y gestiona poll()
- RequestParser -> Analiza las solicitudes HTTP línea por línea
- Router -> Empareja la solicitud con el servidor y la ubicación
- ResponseBuilder -> Genera respuestas HTTP/1.1 válidas
- CGIHandler -> Ejecuta scripts CGI y obtiene la salida
- ConfigParser -> Analiza un archivo de configuración similar a NGINX
- ClientSession -> Monitorea el estado de cada conexión
- Utils -> Ayudantes: división de cadenas, tipos MIME, páginas de error

ServerManager acepta nuevos clientes y crea una ClientSession.

ClientSession lee datos sin procesar del socket.

RequestParser, dentro de ClientSession, analiza la solicitud HTTP.

El enrutador determina qué bloque de servidor y ubicación se aplica.

ResponseBuilder prepara los encabezados y el cuerpo de la respuesta.

Si se requiere CGI, CGIHandler ejecuta un script y recopila la salida.

ResponseBuilder envía la respuesta al cliente.

ServerManager supervisa el tiempo de espera o el cierre.

PLAN DE PRUEBAS SUGERIDO
-Servir archivo estático
-Subir con POST
-Eliminar un archivo
-Solicitud mal formada
-Prueba CGI
-Configurar con múltiples servidores
-Autoindexar

········································
Paso 2: Planificar la arquitectura········
·······································

APRENDER DE ANTEMANO:
-Cómo NGINX modulariza sus procesos de trabajo y la canalización de gestión de solicitudes
-Proceso maestro: Lee la configuración y gestiona los procesos de trabajo
-Procesos de trabajo: Realizan el trabajo real (no se bifurcarán varios procesos de trabajo, pero el bucle de eventos cumple la misma función)
-El bucle de eventos: cada proceso de trabajo ejecuta un bucle de eventos, utilizando poll() para monitorizar todos los sockets abiertos
-Para cada Conexión: gestiona eventos de E/S y los pasa a través de un flujo de fases de procesamiento de solicitudes.

- Debido a que tenemos muchos clientes conectados simultáneamente, si se gestiona uno a la vez (bloqueo), los demás tendrían que esperar.

- Con un enfoque basado en eventos: Permite supervisar todos los sockets, ver quién está listo, gestionarlos en cuanto estén listos y gestionarlos sin bloqueo.

- poll() es una llamada al sistema que notifica cuando un socket está listo. Ese es el evento en el bucle de eventos.
- CENTRO DE LLAMADAS:
- Llamadas entrantes -> POLLIN en el socket del servidor -> Aceptar llamada (accept())
- Inicia la conversación -> POLLIN en el socket del cliente -> Escucha (recv())
- Espera respuesta -> POLLOUT -> Responder (send())
- POR CLIENTE:

1. El cliente se conecta -> accept() -> nuevo socket
2. El cliente envía una solicitud HTTP -> recv() -> solicitud de búfer
3. El servidor analiza la solicitud y prepara la respuesta
4. El servidor escribe la respuesta -> send()
5. Cierra la conexión (a menos que se mantenga activa)
Cada una de estas etapas corresponde a una fase y un evento de E/S diferentes
- CANAL DE GESTIÓN DE SOLICITUDES:
1. Lee la solicitud -> recv(fd, buffer, ...) sin bloqueos

Debido a que el cliente puede enviar la solicitud en partes, el búfer podría no contener la solicitud completa; debe esperar hasta tener suficiente para analizarla.

Almacene los datos en un búfer por cliente.

2. Analice la línea de solicitud -> GET /index.html HTTP/1.1 (ejemplo).

Use std::istringstream y la división simple de cadenas.

3. Analice los encabezados.

Almacenelos en un archivo std::map<std::string, std::string> para facilitar el acceso.

Los encabezados se separan con \r\n y la sección termina en \r\n\r\n.

4. Elija un bloque de servidor -> Observe la IP del socket.
Puerto y encabezado del host
-Seleccione el mejor bloque de servidor del archivo de configuración
-Si dos servidores coinciden con el mismo puerto pero con diferente nombre_de_servidor, use Host: para desambiguar

5. Elija un bloque de ubicación -> Dentro del bloque de servidor seleccionado, haga coincidir la URI
-Coincidir por el prefijo más largo
-Añadir posteriormente compatibilidad con expresiones regulares si es necesario

6. Verificar métodos permitidos -> Dentro del bloque de ubicación, comprobar si el método está permitido

Si el cliente envía un método no permitido, devuelve 405 Método no permitido

7. Servir archivo estático o ejecutar CGI

Si es un archivo estático: Cree una ruta completa, compruebe si el archivo existe, ábralo y léalo, cree una respuesta con Content-Type, etc.

Si es CGI: Prepare las variables de entorno, use fork() + execve() para ejecutar el script, capture su salida y conviértala en una respuesta HTTP

8. Generar respuesta

Componga la línea de estado, los encabezados y el cuerpo en un solo búfer

9. Escribir respuesta -> Use send(fd, response_buffer, ...) sin bloqueo
-Puede aceptar múltiples envíos, manteniendo un registro de cuánto se ha enviado por socket
10. Cerrar o mantener activo
-Si se mantiene activo, mantener el socket abierto para la siguiente solicitud
-De lo contrario, cerrarlo después de enviar la respuesta

switch(client_state) {
case READING:
recv(fd, buffer);
if(full_request_received)
client_state = PARSING;
break;

case PARSING:
parse_request_line_and_headers();
select_server_and_location_block();
validate_method();
client_state = PROCESSING;
break;

case PROCESSING:
if(is_cgi) run_cgi();
other load_static_file();
build_http_response();
client_state = WRITING;
break;

case ESCRIBIR:
send(fd, response);
if (connection_close)
close(fd);
de lo contrario
client_state = READING;
break;
}
-DISEÑO DE WEBSERV
1. Proceso de trabajo -> bucle poll() en ServerManager. Acepta conexiones y lee/escribe a los clientes.
2. Gestión de conexiones -> clase ClientConnection. Almacena el estado de cada cliente: búfer, FD del socket, estado de análisis, tiempo de espera.
3. Fases de la solicitud -> Máquina de estados en RequestParser. Analiza las solicitudes: línea->encabezados->cuerpo. Se puede reutilizar entre clientes.
4. Módulos -> CGIHandler, ResponseBuilder, etc. Clases modulares para gestionar esto.
5. Jerarquía de configuración -> ServerConfig, LocationConfig. Se analiza desde el archivo de configuración. Cada servidor tiene múltiples ubicaciones.
6. Lógica de enrutamiento -> clase Router. Selecciona ServerConfig + LocationConfig coincidentes. Aplica raíz, índice, etc.

DESGLOSE CLASE POR CLASE:
1. ServerManager:

Inicia todos los sockets de escucha desde la configuración

Ejecuta el bucle poll()

Acepta nuevos clientes

Transfiere el control a ClientConnection
RESPONSABILIDAD: Controlador principal del servidor: configura los sockets y gestiona el bucle poll()
TAREAS PRINCIPALES:

Cargar las configuraciones del servidor

Crear/vincular/escuchar sockets para cada (IP, puerto)

Añadir sockets del servidor a la lista pollfd

En el bucle principal: Aceptar nuevos clientes si un socket de escucha está listo, leer/escribir desde el client