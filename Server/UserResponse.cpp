#include "UserResponse.hpp"

UserResponse::UserResponse(User & user, Server const & server) 
	: _user(user), _server(server) {

	std::cout << CYAN << "\t[UserResponse] constructor called" << RESET << std::endl;
	std::cout << YELLOW << "\tuser.handshaked(): " << std::boolalpha << _user.handshaked() << RESET << std::endl;

	if (_user.handshaked() == false) {
		handshakeResponse();
	}
	else {
		responseBuilder();
	}
}

UserResponse::~UserResponse() {
}

void UserResponse::handshakeResponse() {

	// std::string serverName = _server.getServerName();
	std::string serverVersion = "_server.getServerVersion()";
	std::string serverCreated = "_server.getCreationDate()";
	std::string hostName = _user.getHostName();
	std::string nickName = _user.getNickName();
	std::string userName = _user.getUserName();

	std::stringstream reply_buffer;
	reply_buffer << RPL_WELCOME(nickName, hostName) << RPL_YOURHOST(nickName)
	<< RPL_CREATED(nickName, serverCreated) << RPL_MYINFO(nickName);
	_user.responseBuffer = reply_buffer.str();
	std::cout << _user.responseBuffer;
	_user.setHandshaked(true);
}

void UserResponse::responseBuilder() {

	// BUILDING RESPONSES BASED ON THE COMMANDS RECEIVED FROM THE CLIENT
	(void) _server; //delete
	if (_user.pinged())
	{
		std::cout << CYAN << "PINGED" << RESET << std::endl;
		_user.responseBuffer = getPrefix() + " PONG " + _server.getServerName() + "\r\n";
		std::cout << _user.responseBuffer << std::endl;
		_user.setPinged(false);
	}
	else if (_user.userMessageBuffer.empty()) {

		_user.responseBuffer = "\t<empty request>\n";
	}
	else {

		_user.responseBuffer = "\t..coucou. The Matrix has you.. waiting for command..\n";
	}
}

std::string	UserResponse::getPrefix()
{
	std::stringstream prefix;
	prefix << ":" << _user.getNickName() << "!" << _user.getUserName();
	std::string hostName = _user.getHostName();
	if (!hostName.empty())
		prefix << "@" << hostName;
	return (prefix.str());
}
