#include "User.hpp"
#include "../Channel/Channel.hpp"
#include <iostream> 

User::User() : 
	_port(0), 
	_socket(0), 
	_hostName(""), 
	_nickName(""), 
	_userName(""), 
	_password(""), 
	_authenticated(false), 
	_handshaked(false) {

}

User::User(const int& fd) : _socket(fd)
{
	//std::cout << "User has been created" << std::endl;
}

User::~User()
{
	//std::cout << "User " << _userName << " has been destructed" << std::endl;
}

// --------------------------------------- SETTERS ---------------------------------------- // 

void	User::setPort(const int& port)
{
	_port = port;
}

void	User::setSocket(const int& socket)
{
	_socket = socket;
}

void 	User::setNickname(const std::string& nickname)
{
	_nickName = nickname;
}

void 	User::setUsername(const std::string& username)
{
	_userName = username;
}

void 	User::setHostname(const std::string& hostname)
{
	_hostName = hostname;
}

void 	User::setPassword(const std::string& password)
{
	_password = password;
}

void	User::setChannel(Channel& channel)
{
	//_channels.insert(std::make_pair(channel.getName(), channel));
	_channels[channel.getName()] = &channel;
	// checker si le channel existe deja et voir quel comportement je veux
}

void	User::setIsOp( const bool& isOp )
{
	_isOp = isOp;
}

void	User::setCanModifyTopic( const bool& topic )
{
	_canModifyTopic = topic;
}

void	User::setAuthenticated(bool authenticated)
{
	_authenticated = authenticated;
}

void	User::setHandshaked(bool handshaked)
{
	_handshaked = handshaked;
}

// ---------------------------------------- GETTERS ----------------------------------------- // 

const int& User::getPort( void ) const
{
	return _port;
}

const int& User::getSocket( void ) const
{
	return _socket;
}

const std::string& User::getNickName( void ) const
{
	return _nickName;
}

const std::string& User::getUserName( void ) const
{
	return _userName;
}

const std::string& User::getHostName( void ) const
{
	return _hostName;
}

const std::string& User::getPassword( void ) const
{
	return _password;
}

Channel& User::getChannel( const std::string& name ) const
{
	return *_channels.at(name);
}

const std::map<std::string, Channel*>& User::getChannels( void ) const
{
	return _channels;
} 

const bool& User::getIsOp( void ) const
{
	return _isOp;
}

const bool&	User::getCanModifyTopic( void ) const
{
	return _canModifyTopic;
}

// fonction membres 

void	User::removeChannel(Channel& channel)
{
	std::map<std::string, Channel*>::iterator it;
	it = _channels.find(channel.getName());
    if (it != _channels.end())
	{
		channel.removeUser(*this);
        _channels.erase(it);
	}
}

void User::printChannels( void ) const
{
	std::map<std::string, Channel*>::const_iterator it;

    std::cout << "Channels in this user:" << std::endl;
    for ( it = _channels.begin(); it != _channels.end(); ++it)
        std::cout << it->second->getName() << std::endl;
}

bool	User::authenticated() 
{
	return this->_authenticated;
}

bool	User::handshaked() 
{
	return this->_handshaked;
}
