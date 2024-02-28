#ifndef USER_HPP
# define USER_HPP

	#include <string>
	#include <map>
	#include "../Channel/Channel.hpp"

	class User {

		private :
		int								_port;
		int								_socket;
		std::string						_hostName; // ..parsed in `UserRequestParsing` class..
		std::string 					_nickName;	// ..parsed in `UserRequestParsing` class..
		std::string 					_userName; // ..parsed in `UserRequestParsing` class..
		std::string						_realName; // ..parsed in `UserRequestParsing` class..
		std::string 					_password; // ..parsed in `UserRequestParsing` class..
		// ..to use for composing the first response message to the client (RPL_WELCOME, RPL_YOURHOST, RPL_CREATED, RPL_MYINFO..)

		public :
		User() : _port(0), _socket(0), _hostName(""), _nickName(""), _userName(""), _password(""), _authenticated(false), _handshaked(false) {};
		~User() {};
		std::string						userMessageBuffer; // il y aura un type message ?
		std::string						responseBuffer;
		std::map<std::string, Channel*>	_channels;
		bool							_authenticated;
		bool							_handshaked;
		// Setters //
		void	setPort(const int& port);
		void	setSocket(const int& socket);
		void 	setNickName(const std::string& nickname);
		void 	setUserName(const std::string& username);
		void 	setHostName(const std::string& hostname);
		void 	setPassword(const std::string& password);
		void	setChannel(Channel& channel);
		void	setAuthenticated(bool authenticated);
		void	setHandshaked(bool handshaked);
		// Getters //
		const int& getPort( void ) const;
		const int& getSocket( void ) const;
		const std::string& getNickName( void ) const;
		const std::string& getUserName( void ) const;
		const std::string& getHostName( void ) const;
		const std::string& getPassword( void ) const;
		Channel& getChannel( const std::string& name ) const;
		const std::map<std::string, Channel*>& getChannels( void ) const;
		bool	authenticated();
		bool	handshaked();

		void	printChannels( void ) const;   // for debug
		void	removeChannel(const std::string& channelName);
		
	};

#endif