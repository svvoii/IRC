#include "IrcBot.hpp"

//Global variable for signal handling
// bool signalFlag = false;

IrcBot* IrcBot::ircBotInstance = NULL;

IrcBot::IrcBot(const std::string& serverName, int port, const std::string& pass, const std::string& botName)
	: 
	_serverSocket(0),
	_serverPort(port),
	_serverName(serverName),
	_serverPass(pass),
	_botName(botName),
	_serverRequestBuffer(""),
	_responseGPT(""),
	_authenticated(false),
	signalErrorFlag(false) {
	
	ircBotInstance = this;

	// Setting up the signal handler
	signal(SIGINT, IrcBot::signalHandler);

	// Initializing the client socket
	initSocket();

	// Establishing a connection to the server
	connectToServer();
}

IrcBot::~IrcBot() {
	close(_serverSocket);
}

void IrcBot::initSocket() {

	// ..well, it is actually a socket the bot is using to connect to the server

	_serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	checkErrorAndExit(_serverSocket, "ERROR: Failure opening socket for IrcBot client.\n");
}

/*
** This function is used to establish a connection to the server given the server's IP address and port.
*/
void IrcBot::connectToServer() {

	struct sockaddr_in serv_addr;

	memset(&serv_addr, 0, sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(_serverPort);
	serv_addr.sin_addr.s_addr = inet_addr(_serverName.c_str()); // This works only for IPv4 in `127.0.0.1` format

	int returnValue = connect(_serverSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	checkErrorAndExit(returnValue, "ERROR: connecting to server [" + _serverName + "] failed. Make sure the server is up..\n");

	if (!signalErrorFlag) {
		std::cout << "Connected to server: " << _serverName << " on port: " << _serverPort << std::endl;
		std::cout << "Bot name: " << _botName << std::endl;
		std::cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << std::endl;
	}

}

void IrcBot::sendHandshake() {

	sendIrcMessage("PASS " + _serverPass);
	sendIrcMessage("NICK " + _botName);
	sendIrcMessage("USER " + _botName + " 0 * :" + _botName);
}

void IrcBot::joinChannel(const std::string& channel) {

	sendIrcMessage("JOIN " + channel);
}

void IrcBot::sendIrcMessage(const std::string& message) {

	std::string fullMessage = message + "\r\n";
	
	int returnValue = write(_serverSocket, fullMessage.c_str(), fullMessage.length());
	checkErrorAndExit(returnValue, "ERROR: writing to socket failed\n");

}

void IrcBot::sendMessage(const std::string& channel, const std::string& message) {

	sendIrcMessage("PRIVMSG " + channel + " :" + message);
}

void IrcBot::handleServerRequest() {

	char		buffer[BUFFER_SIZE];

	memset(buffer, 0, BUFFER_SIZE);

	// normaly the `read` will block the program until there is something to read from the socket..
	// ..this is expected behavior for the bot, so we don't need to set the socket to non-blocking mode with `fcntl`..
	int bytes = read(_serverSocket, buffer, BUFFER_SIZE - 1);
	checkErrorAndExit(bytes, "ERROR: reading from socket failed\n"); // check if `-1`

	if (bytes == 0) {
		checkErrorAndExit(-1, "ERROR: server closed the connection\n");
	}

	// once the code made it this far means `read` was successful and we can process the buffer
	buffer[bytes] = '\0';
	_serverRequestBuffer = buffer;

}

void IrcBot::handleResponse() {

	std::cout << "Request from the server, RAW: " << _serverRequestBuffer << std::endl;

	/* DEBUG */
	// Check the string for (001 or 002, 003, 004).. if there means the bot is authenticated/registered on the server side
	if (_serverRequestBuffer.find("001") != std::string::npos) {
		_authenticated = true;

		joinChannel("#helpdesk");
		sendMessage("#helpdesk", "Hi there! I'm NeoBot. I'm here to help. Type 'DO_THE_THING: <question>' to get started.");
	}
	
	/* ***** */

	std::string request = _serverRequestBuffer;

	if (request.find("PRIVMSG") != std::string::npos) {
		std::string sender = request.substr(1, request.find("!") - 1);
		std::string message = request.substr(request.find("PRIVMSG") + 8);
		std::cout << "Message from " << sender << ": " << message << std::endl;
	}
	else if (request.find("PING") != std::string::npos) {
		sendIrcMessage("PONG " + request.substr(5));
	}
	else if (request.find("DO_THE_THING") != std::string::npos) {

		std::string toErase = "DO_THE_THING:";
		size_t pos = _serverRequestBuffer.find(toErase);
		if (pos != std::string::npos) {
			_serverRequestBuffer.erase(pos, toErase.length());
		}

		// handleGPT();

	}
}

void IrcBot::checkErrorAndExit(int returnValue, const std::string& message) {

	if (returnValue < 0) {

		std::cerr << RED << message << RESET << std::endl;

		signalErrorFlag = true;
	}
}

void IrcBot::signalHandler(int signal) {
	std::cout << RED << "\nInterrupt signal (" << signal << ") received." << RESET << std::endl;
	
	ircBotInstance->handleSignal();

	// exit will show as if there is a memory leak..
	// exit(signal);
}

void IrcBot::handleSignal() {

	std::cout << YELLOW << "Sending QUIT request to the server.." << RESET << std::endl;
	sendIrcMessage("QUIT :NeoBot is leaving the building..");	

	signalErrorFlag = true;
	
} 

/*
** Here we use so called `named pipes` to communicate with the GPT container.
** Also known as `FIFO` (First In First Out).
** The host_to_container.fifo is used to send/write the request string to the container.
** The container_to_host.fifo is used to receive/read the response from the container.
** Inside the container environment we have a python app that handles the requests to 
** and responce from OpenAI's GPT via API.
*/
void IrcBot::handleGPT() {

	std::ofstream host_to_container("./GPT/host_to_container.fifo");
	host_to_container << _serverRequestBuffer;
	host_to_container.close();

	std::ifstream container_to_host("./GPT/container_to_host.fifo");
	std::getline(container_to_host, _responseGPT);
	container_to_host.close();

	// _responseGPT must be sent back to the server..
	std::cout << "GPT response: " << _responseGPT << std::endl;

}

/*
** This function is used to establish a connection to the server.
** It uses the `getaddrinfo` function to get the server address and port.
** The `struct addrinfo` is used to identify the server name/address and port.
** The `getaddrinfo` function returns a list of `struct addrinfo` structures, each of which contains an Internet address that can be specified in a call to `connect`.
** This slows down the program since it has to be free'd with `freeaddrinfo` after the connection is established.
** ..we will use older method where the address is given directly to the `connect` function in `127.0.0.1` format.
void IrcBot::connectToServer() {

	struct addrinfo		hints;
	struct addrinfo*	result;

	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	int returnValue = getaddrinfo(_serverName.c_str(), NULL, &hints, &result);
	checkErrorAndExit(returnValue, "ERROR: getaddrinfo failed. No such host [" + _serverName + "]\n");

	struct sockaddr_in* serv_addr = (struct sockaddr_in*)result->ai_addr;
	serv_addr->sin_port = htons(_serverPort);

	returnValue = connect(_serverSocket, (struct sockaddr*)serv_addr, sizeof(*serv_addr));
	checkErrorAndExit(returnValue, "ERROR: connecting to server [" + _serverName + "] failed. Make sure the server is up..\n");

	if (!signalErrorFlag) {
		std::cout << "Connected to server: " << _serverName << " on port: " << _serverPort << std::endl;
		std::cout << "Bot name: " << _botName << std::endl;
		std::cout << "- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - " << std::endl;
	}

	freeaddrinfo(result);

}
*/
