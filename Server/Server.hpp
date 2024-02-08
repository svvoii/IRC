/*
** This class represents one server with Listening socket.
** If several servers are needed, several instances of this class
** will be created.
**
** After the data was parsed from the config file, the ammount of servers
** will be known. 
**
** This class is created once the data is parsed from the config file.
** Therefore the `t_serverData` struct will be passed to the constructor.
** On the classs instance creation, the Listening socket is created 
** as well, also in the constructor of this class.
**
** This class shall be instantiated in the `ServerManager` class,
** along with the management of the `Config` class...
*/
#ifndef SERVER_HPP
#define SERVER_HPP

#include "../Sockets/HeaderSockets.hpp"
#include "../Config/Config.hpp"

/* THE FOLLOWING DEFINITIONS SHALL BE CHANGED ..*/
#define PROTOCOL	0
#define BACKLOG		100
#define PORT 		55555
#define SERVER_NAME	"localhost"
#define SERVER_PASSWORD "password"


class Server {

	private:
		ListeningSocket							_listenSocket;
		int										_serverFd;
		int										_port;
		std::string								_serverName;
		std::string								_serverPassword;
		
	public:
		Server();
		~Server();

		// Getters
		int					getServerFd() const;
		std::string			getServerName() const;

		// DEBUG
		void				printServerData() const;

};



#endif
