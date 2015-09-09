#pragma once

/*
  This class is a command which to allow to check and manage the argument "destination address".
  This command allow to set especially the address and port of the destination.
*/

#include "ACommand.hpp"

#define	FORMAT_ADDRESS	"<address:port>"

class	ISocket;

class	AddressManager : public ACommand
{
private:
  std::string		_address;
  int			_port;
  ISocket		*_socket;

public:
  AddressManager();
  virtual ~AddressManager();
  //Check the syntax of the command Address Manager
  virtual bool          checkSyntax(std::vector<std::string> &args);
  //Accessor method for the address attribute
  const std::string	&getAddress() const;
  //Accessor method for the port attribute
  int			getPort() const;
  //Mutator method for the socket attribute
  void			setSocket(ISocket *socket);
  //Accessor method for format of the command Address Manager
  std::string		getFormat() const;
  //Display the usage of the command Address Manager
  virtual void		displayUsage() const;
};
