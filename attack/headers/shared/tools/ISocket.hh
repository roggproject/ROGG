#pragma once

/*
  It's an interface for sockets used within the project.
  It allow to create different socket for different platforms, consequently, the cross-platform is easily implementable.
  All sockets within in the project must inherit of this interface.
*/

#include <string>

class	ISocket
{
public:
  virtual		~ISocket() {}
  //Send the data with the size specified
  virtual void		send(const void *data, size_t size) = 0;
  //Close the socket
  virtual void		closeSocket() = 0;
  //Check if the address specified is valid
  virtual bool		isValidIPAddress(const char *ipAddress) const = 0;
  //Mutator method for the port within the socket
  virtual void		setPort(int port) = 0;
  //Mutator method for the address within the socket
  virtual void		setAddress(const std::string &address) = 0;
  //Accessor method for the address of the socket
  virtual uint32_t	getDestAddress() const = 0;
  //Accessor method for the port of the socket
  virtual int		getDestPort() const = 0;
  //Convert the address specified for using with the socket
  virtual uint32_t	convertAddress(const char *address) const = 0;
  //Receive data
  virtual void		*receive(int *sizeReceiv) = 0;
    //Wait the read
  virtual int		select(long int *usecTimeout) = 0;
  //Wait the read
  virtual int		select(long int usecTimeout = -1) = 0;
  //Reset sockets, with input fd or not 
  virtual void		resetSockets(bool input = false) = 0;
  //Check if the input fd is readable
  virtual bool		inputFdReadable() const = 0;
  //Check if the socket is readable
  virtual bool		socketReadable() const = 0;	
};
