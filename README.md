# 42_webserver
🧠 Learn Before:
How NGINX uses server {} and location {} blocks

Recursion-based config parsing

Tokenization, bracket matching

🔧 Actions:
Create a lexer/tokenizer

Create a recursive parser that builds a tree of configs

Validate inheritance of configs (e.g., root, index from server to location)

✅ Step 4: Set Up the Server Socket(s)
Goal: Bind sockets based on the config file.

🧠 Learn Before:
socket(), bind(), listen(), accept()

poll() vs select() vs epoll() (choose poll for POSIX portability)

Handling multiple ports (e.g., multiple listen blocks)

🔧 Actions:
Loop over server configs → create listening sockets

Use poll() to monitor incoming connections

✅ Step 5: Implement Connection Management
Goal: Accept clients and handle requests non-blockingly.

🧠 Learn Before:
Non-blocking sockets

Reusing file descriptors and buffers

Handling multiple clients efficiently

🔧 Actions:
Accept new connections

Read from sockets, parse requests

Store partial reads (chunked or large bodies)

Detect client disconnections

✅ Step 6: Parse HTTP Requests
Goal: Build a robust request parser.

🧠 Learn Before:
HTTP request format: Request-Line, headers, optional body

Chunked transfer encoding

URL decoding and header validation

🔧 Actions:
Use a state machine to parse request lines, headers, and body

Handle edge cases: malformed requests, unsupported methods

✅ Step 7: Route the Request
Goal: Match the request to the correct server and location block.

🧠 Learn Before:
NGINX’s routing logic: listen, server_name, location /foo

How wildcard matching works (location /path/ {} vs exact match)

🔧 Actions:
Implement logic to match requests to the best-fitting config block

Apply inherited values (e.g., root, methods, index)

✅ Step 8: Generate Responses
Goal: Build responses for GET, POST, DELETE.

🧠 Learn Before:
How file serving works (open/read/send)

Autoindex behavior (when index file missing)

Common HTTP status codes

🔧 Actions:
For GET: serve static file or autoindex directory

For POST: handle form data and store it if needed

For DELETE: delete file and return proper code

Add headers (Content-Type, Content-Length, Date, etc.)

✅ Step 9: Implement CGI Handling
Goal: Support execution of CGI scripts.

🧠 Learn Before:
CGI environment variables (REQUEST_METHOD, SCRIPT_FILENAME, etc.)

How to fork(), execve(), and set up pipes

Timeouts and input buffering

🔧 Actions:
On matching location, fork a process to run the CGI

Pipe the request body to stdin, read from stdout

Parse and forward headers in CGI output

✅ Step 10: Handle Errors and Status Codes
Goal: Show correct status and custom error pages.

🧠 Learn Before:
NGINX’s error_page directive behavior

What 404, 405, 500, 413, etc. mean

🔧 Actions:
Return correct error codes

If error_page 404 /404.html; is defined, serve that page

Default to generic error message if not defined

✅ Step 11: Test Thoroughly
Goal: Ensure robustness and NGINX-like behavior.

🧪 Actions:
Use curl, ab, or Postman to test:

Chunked uploads

CGI execution

Invalid requests

Multiple simultaneous connections

Write tests for config parsing edge cases

✅ Step 12: Optimize and Polish
Goal: Handle edge cases and scale gracefully.

⚙️ Tips:
Implement timeout handling (e.g., 408)

Cap max body size (413)

Clean up zombie CGI processes

Log requests and errors like access.log/error.log

✅ Step 13: Bonus / Advanced
If you want to go beyond:

Support redirects (return 301 ...)

Use epoll (Linux) or kqueue (BSD/Mac)

IPv6 support

Add reverse proxy features (simulate proxy_pass)

🚀 Final Advice
Phase	Focus
Week 1	HTTP, config parsing, sockets
Week 2	request parsing, response, CGI
Week 3	error handling, edge cases
Week 4	optimization, testing, polish

Take inspiration from NGINX, but don’t overengineer. Focus on matching the subject's requirements first, then add complexity.

Would you like a diagram or starter code structure for this plan?


···········································
Step 1: Understand Project Requirements····
···········································

GOAL: HTTP 1.1 web server in c++98.
	-It supports multiple clients
	-Parses a config file
	-Serves static files and runs CGI
	-Handles common HTTP methods (GET, POST, DELETE)
	-Manages errors gracefully
1. NETWORKING CORE -> Create server sockets, use poll() and accept connections non-blockingly
2. CONFIG PARSER -> Parse NGINX-style configuration. Support server, location, listen, etc. and store inheritance relationships
3. REQUEST HANDLING -> Parse raw HTTP requests (Request-Line + Headers + Body). Handle GET, POST, DELETE. Store partially received requests if needed
4. CGI HANDLER -> Use fork()/execve() to run scripts. Use pipe() and dup2() to capture input/output. Set environment variables from request
5. RESPONSE GENERATOR -> Build a valid HTTP/1.1 response with status, headers, body. Handle chunked encoding if needed. Support Connection: keep-alive or close 
6. ERROR HANDLING -> Detect and handle malformed requests, suport custom error_page per status code, return valid HTTP error responses
7. ROUTING LOGIC -> Match request to correct server and location based on port, host and request path. Apply inherited config values

WHAT TO LEARN:
Look for:
	-Required features: Serve static files (GET), handle POST and DELETE methods, CGI execution, custom error pages, multiple clients, location and server block parsing, method restrictions.
	-Forbidden behavior: No memory leaks, no segmentation faults/undefined behavior, no infinite loops or hangs, not use forbidden system calls, don't use high-level libraries
	-Restrictions: C++98 only, no STL containers except the allowed, no use of std::thread or async I/O, no boost
	-Evaluation criteria: compilation without warnings/errors, memory leaks: test with valgrind, correct behavior under edge cases, respect HTTP/1.1 specifications, handling malformed input correctly, code readability, structure and organization.
	-Examples or edge cases: Provided examples of input/output, edge case hints, specific behaviors to match, required default behavior when config is incomplete

Mandatory features:
	-Core functionalities: GET, POST, DELETE
	-Config file parsing: Parse config file, create internal structures for server and location blocks, apply inheritance logic
	-CGI support: support cgi-bin scripts. Use fork, execve, dup2 and environment variables
	-Error handling: status codes and custom error pages
	-Chunked transfer encoding: Handle incoming/outgoing chunked requests/responses. (especially for POST bodies)
	-Timeouts and Robustness: Close timeouts client connections. Avoid memory leaks or crashing on malformed requests

CONSTRAINTS & LIMITATIONS
	-c++98 only
	-No external libraries
	-Must support multiple clients (poll())
	-Everything custom 
	-Deterministic behavior

RECOMMENDED INTERNAL MODULES
	-ServerManager -> Listens, accepts connections and manages poll()
	-RequestParser -> Parses HTTP requests line by line
	-Router -> Matches request to server + location
	-ResponseBuilder -> Generates valid HTTP/1.1 responses
	-CGIHandler -> Runs CGI scripts and gets outputt
	-ConfigParser -> Parses NGINX-like config file
	-ClientSession -> Tracks state of each connection
	-Utils -> Helpers: string split, MIME types, error pages
ServerManager accepts new clients and creates a ClientSession.

ClientSession reads raw data from socket.

RequestParser inside ClientSession parses HTTP request.

Router determines which server and location block applies.

ResponseBuilder prepares response headers and body.

If CGI needed, CGIHandler runs script and collects output.

ResponseBuilder sends response to client.

ServerManager monitors for timeout or closure.



SUGGESTED TESTING PLAN
	-Serve static file
	-Upload with POST
	-Delete a file
	-Malformed request
	-CGI test
	-Config with multiple servers
	-Autoindex


·····································
Step 2: Plan the Architecture········
·····································

LEARN BEFOREHAND:
	-How NGINX modularizes its worker processes and request handling pipeline
		-Master process: Reads config, manages worker processes 
		-Workers processes: Do the actual work (you won't fork multiple workers, but your event loop plays the same role)
		-The event loop: each worker process runs an event loop, using poll() to monitor all open sockets 
		-For each connection, it handles I/O events and passes them through a pipeline of request processing phases 
		-Because we have many clients connected at once if you handled one at a time (blocking), others would have to wait. 
		-With an event-driven approach : Can watch all sockets, see who is ready, handle them as soon as they are ready, can handle them without blocking 
		-poll() is a system call that notifies you when one socket is ready. That's the event in the event loop 
		-CALL CENTER: 
			-Calls in -> POLLIN on server socket -> Accept call (accept())
			-Starts talking -> POLLIN on client socket -> Listen (recv())
			-Waits for answer -> POLLOUT -> Respond (send())
		-PER CLIENT: 
			1. Client connects -> accept() -> new socket 
			2. Client sends HTTP request -> recv() -> buffer request 
			3. Server parses request and prepares response 
			4. Server writes response -> send()
			5. Close connection (unless keep-alive)
			Each of these stages correspond to a different phase and I/O event 
		-REQUEST HANDLING PIPE: 
			1. Read the request -> recv(fd, buffer, ...) non-blockingly.
				-Because: client may send request in parts, buffer may not have the full request, you have to wait until you have enough to parse the request 
				-Store the data in a per-client buffer 
			2. Parse the request line -> GET /index.html HTTP/1.1 (example)
				-Use std::istringstream and simple string splitting
			3. Parse headers 
				-Store them in a std::map<std::string, std::string> for easy access 
				-Headers are separated by \r\n and section ends at \r\n\r\n
			4. Choose a server block -> Look at socket IP + port and the Host header 
				-Select the best server block from the config file 
				-If two servers match same port but different server_name, use Host: to disambiguate
			5. Choose a location block -> Within the selected server block, match the URI
				-Match by longest prefix
				-Later add regex support if needed 
			6. Check allowed methods -> Inside location block, check if method is allowed 
				-If client sends method not allowed return 405 Method Not Allowed 
			7. Serve static file or run CGI  
				-If it's static file: Build a full path, check if file exists, open and read the file, build a response with Content-Type, etc.
				-If CGI: Prepare environmentt variables, use fork() + execve() to run the script, capture its output and turn it into HTTP response 
			8. Generate response  
				-Compose status line, headers and body into a single buffer 
			9. Write response  -> Use send(fd, response_buffer, ...) non-blockingly 
				-May take multiple sends, keep track of how much has been sent per socket
			10. Close or keep-alive 
				-If keep-alive, keep socket open for next request 
				-Otherwise, close it after the response is sent

			switch (client_state) {
    			case READING:
    			    recv(fd, buffer);
    			    if (full_request_received)
    			        client_state = PARSING;
    			    break;

    			case PARSING:
    			    parse_request_line_and_headers();
    			    select_server_and_location_block();
    			    validate_method();
    			    client_state = PROCESSING;
    			    break;

    			case PROCESSING:
    			    if (is_cgi) run_cgi();
    			    else load_static_file();
    			    build_http_response();
    			    client_state = WRITING;
    			    break;

    			case WRITING:
    			    send(fd, response);
    			    if (connection_close)
    			        close(fd);
    			    else
    			        client_state = READING;
    			    break;
			}
		-WEBSERV DESIGN 
			1. Worker process -> poll() loop in ServerManager. Accepts connections, reads/writes to clients 
			2. Connection handling -> ClientConnection class. Holds per-client state: buffer, socket FD, parsing state, timeout 
			3. Request phases -> State machine in RequestParser. Parse requests: line->headers->body. Can reuse between clients  
			4. Modules -> CGIHandler, ResponseBuilder, etc. Modular classes to handle this. 
			5. Config hierarchy -> ServerConfig, LocationConfig. Parsed from your config file. Each server has multiple locations 
			6. Routing logic -> Router class. Chooses matching ServerConfig + LocationConfig. Applies root, index, etc.  
		
		-CLASS BY CLASS BREAKDOWN :
			1. ServerManager: 
				-Starts all listening sockets from config 
				-Runs poll() loop 
				-Accepts new clients 
				-Passes controll to ClientConnection 
				RESPONSIBILITY: Main controller of your server - sets up sockets and drives poll() loop 
				CORE TASKS: 
					-Load server configs 
					-Create/bind/listen sockets for each (IP, port) 
					-Add server sockets to the pollfd list 
					-In the main loop: Accept new clients if a listening socket is ready, read/write from client