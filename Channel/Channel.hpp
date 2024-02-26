#ifndef CHANNEL_HPP
# define CHANNEL_HPP

	#include <map>
	#include <string>

	class User;
	class Operator;

	class Channel {

		private :
			std::string						_name;
			std::string						_theme; 	// theme and/or rules of channel
			std::string						_key; 		// channel password
			std::map<std::string, User*> 	_users;
			std::map<std::string, User*>	_ops;
			int								_nb_users;
			int								_limit; 	// max number of users
			bool							_limited; 
			bool							_invit_only; 

		public :
			Channel(const std::string& name);
			~Channel();
			// setters 
			void	setName(const std::string & name);
			void	setTheme(const std::string & theme);
			void	setKey(const std::string & key);
			void	setUser(User& user);
			void	setOp(User& op);
			void	setNbUsers(const int& nb);
			void	setLimit(const int & limit);
			void	setInvit(const bool & invit);
			// getters
			const std::string& 	getName( void ) const;
			const std::string& 	getTheme( void ) const;
			const std::string&	getKey( void ) const;
			User& getUser( const std::string & username ) const;
			const std::map<std::string, User*>& getUsers( void ) const;
			User& getOp( const std::string & username ) const;
			const int& getNbUsers( void ) const;
			const int& getLimit( void ) const;
			const bool& getInvit( void ) const;

			void	removeUser(User& user);
			void	printUsers( void ) const; // for debug
	};

#endif