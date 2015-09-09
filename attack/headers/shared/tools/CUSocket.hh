#pragma	once

/*
  It's a class for UNIX sockets.
*/

#include "ISocket.hh"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>

typedef struct sockaddr	SOCKADDR;

class	CUSocket : public ISocket
{
protected:
  int			_sock;
  struct sockaddr_in	_info;
  int			_port;
  fd_set		_fds;
  struct timeval	_timeout;
  
  
public:
  CUSocket(bool isServer = false);
  virtual				~CUSocket();
  //Send the data with the size specified 
  virtual void				send(const void *data, size_t size);
  //Close the socket
  virtual void				closeSocket();
  //Set the socket attribute
  virtual void				setSocket(int sock);
  //Accessor method for the info attribute
  virtual const struct sockaddr_in	&getInfo();
  //Accessor method for the socket attribute
  virtual int				getSocket() const;
  //Check if the address specified is valid
  virtual bool				isValidIPAddress(const char *ipAddress) const;
  //Mutator method for the port within the socket
  virtual void				setPort(int port);
  //Mutator method for the address within the socket
  virtual void				setAddress(const std::string &address);
  //Accessor method for the address of the socket
  virtual uint32_t			getDestAddress() const;
  //Convert the address specified for using with the socket
  virtual uint32_t			convertAddress(const char *address) const;
  //Accessor method for the port of the socket
  virtual int				getDestPort() const;
  //Receive data
  virtual void				*receive(int *sizeReceiv);
  //Wait the read
  virtual int                           select(long int *usecTimeout);
  //Wait the read
  virtual int				select(long int usecTimeout = -1);
  //Reset sockets, with input fd or not
  virtual void				resetSockets(bool input = false);
  //Check if the input fd is readable
  virtual bool				inputFdReadable() const;
  //Check if the socket is readable
  virtual bool				socketReadable() const;
};
