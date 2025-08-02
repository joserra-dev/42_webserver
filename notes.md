// server {
//     listen 8080;                    #  selected port
//     listen localhost:8080;    
//     listen 127.0.0.1:8080;          # host +  possible port //v6
//     server_name localhost test.i.2; # option //checker con otro servidor si ok mm nom
//     root /www;
//     index index.html;
//     error_page 404 /404.html;
//     error_page 500 501 /50x.html;
//     error_page 400 401 403 405 410 413 414 /4xx.html;
//     client_max_body_size 1m;

//     location / {
//         autoindex off;
//         allow_methods GET POST;
//     }
//     location /static/ {
//         root /static;
//         allow_methods GET;
//     }
//     location /cgi-bin/ {
//         cgi_pass .py /usr/bin/python3;
//         cgi_pass .php /usr/bin/php-cgi;
//         allow_methods GET POST;
//     }

//     location /uploads/ {
//         client_max_body_size 10k; #nginx max 1M for eveything
//         root /uploads;
//         allow_methods POST;
//     }
//     location /redirect/ {
//         return 301 http://localhost:8080/; #define the direction we want
//     }
// }


//Pasar el primer argumento a la función buffer[i + 1]
//Comprobar el número correcto de argumentos y el ";" al final
//Comprobar si los argumentos son del tipo correcto
//Analizar/transformar los argumentos
//Comprobar si el argumento ya existe y añadirlo
//Comprobar si hay líneas duplicadas en el archivo de configuración
//Añadir a los atributos de clase asociados
//Comprobar si el argumento es válido (puerto no cerrado, archivos existentes, etc.)

//Administrar los cambios en los bloques de servidor y ubicación modificando también sus iteradores

--> listen <--

//¿Qué puerto y dirección IP escuchar para la conexión?

// default ip address: 0.0.0.0
// default port : 80
// socket address : 0.0.0.0:80
//Es posible muchas escuchas solo en el bloque del servidor (sin ubicación).
//No es posible tener la misma dirección IP y puerto dos veces.
//Predeterminado: 80 y 0.0.0.0.


--> serverName <--

//Asociar una configuración a un nombre de dominio
//Este servidor responderá a una URL o nombre de dominio específico, lo que significa
// que esta configuración solo se aplicará a las solicitudes realizadas a los argumentos pasados.

// www.example.com
// www es una etiqueta, example es otra etiqueta, com es otra etiqueta.

// Cada etiqueta debe seguir las reglas de DNS:
// Contener solo letras, números y guiones.
// No empezar ni terminar con un guion.
// Tener entre 1 y 63 caracteres.
// Longitud total del nombre ≤ 255 caracteres.
//Puede haber muchos argumentos; verifique con el estándar DNS todas las 'etiquetas' de todos los argumentos.
//Solo puede ser una vez por bloque de servidor (sin ubicación).

--> autoindex <--

//Controla si el servidor debe mostrar automáticamente una lista de archivos
//en una carpeta cuando no se encuentra ningún archivo de índice (como index.html)
//Navegación visual por las carpetas (como un explorador de archivos).

// 'on' -> Muestra los archivos de la carpeta si no se encuentra ningún archivo de índice
// 'off' -> (Predeterminado) Oculta archivos; en su lugar, devuelve un error 403

//Solo uno por bloque, solo se permite 'on' o 'off', solo un argumento

--> root <--

//La ruta base (en el sistema de archivos) donde Nginx buscará
//Archivos estáticos (HTML, CSS, imágenes, JS, etc.) para servir al cliente

//Palabra clave en una solicitud HTTP que indica la intención del cliente
//(a menudo un navegador u otro programa) hacia un recurso

// GET Recuperar un recurso (lectura)
// POST Enviar datos (p. ej., formulario)
// DELETE Eliminar un recurso
// PUT Reemplazar un recurso completo
// PATCH Modificar parcialmente un recurso
// HEAD Igual que GET sin el cuerpo de la respuesta. // ¿AÑADIR?
// OPTIONS Preguntar al servidor qué métodos están permitidos

//Puedes escribir el mismo método varias veces en la directiva; el método duplicado simplemente se ignora.
//Puedes generar un error si lo deseas, pero nginx no lo hace.
//Una directiva por bloque.

--> return <--
// -> Error del servidor <-
//500 Error interno del servidor
//501 No implementado -> Cleo

--> TO DO

//Mantener la dirección IP como ip y no std::string // una versión

///¿Comprobar si una línea es tan larga que la función la devuelve dos veces?
--> TO ASK


--> TO KNOW

//ruta absoluta = /blabla
//ruta relativa = ./blabla

//carácter de prueba no imprimible
// printf "server\x01{}\n" > ../conf/test.conf

//si al archivo le faltan argumentos, como "listen",
//los mensajes de error se generan al iniciar el servidor, no al analizarlo.