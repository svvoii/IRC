#include "Sockets/HeaderSockets.hpp"
// #include "Servers/includes/ServersManager.hpp"

#define PROTOCOL 0
#define PORT 8080
#define BACKLOG 10

int	main() {

	/*
	std::cout << "Starting server..." << std::endl;

	std::cout << "Binding socket..." << std::endl;
	BindingSocket	bindingSocket(AF_INET, SOCK_STREAM, 0, 8081, INADDR_ANY);

	std::cout << "Listening to the network..." << std::endl;
	ListeningSocket	listeningSocket(AF_INET, SOCK_STREAM, 0, 8080, INADDR_ANY, 10);

	std::cout << "Success!" << std::endl;
	*/

	//TestServer	server;
	
	// All arguments are for TESTING PURPOSES, declared in `TestServer.hpp`
	//HttpServer server(AF_INET, SOCK_STREAM, PROTOCOL, PORT, INADDR_ANY, BACKLOG);

	// Several servers test
	// ServersManager	serverManager;

	return 0;
}