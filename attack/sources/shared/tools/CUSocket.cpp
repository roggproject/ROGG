#include "CUSocket.hh"
#include "Exception.hh"
#include "Defines.hh"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>

//Initialization of the ressources and properties of the UNIX socket
CUSocket::CUSocket(bool isServer)
{
  int		one = 1;
  const int *val = &one;
  
  this->_sock = -1;
  this->_info.sin_family = AF_INET;
  this->_timeout.tv_sec = 0;
  this->_timeout.tv_usec = TIMEOUT_PACKET;
  if (getuid() != 0)
    throw Exception("With the raw sockets you have to run the client in administrator rights.");
  if (isServer)
    this->_sock = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
  else
    this->_sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
  if (this->_sock == -1)
    {
      this->_sock = -1;
      throw Exception("socket has failed.");
    }
  if ((setsockopt(this->_sock, IPPROTO_IP, IP_HDRINCL, val, sizeof(int))) == -1)
    throw Exception("setsockopt has failed.");
}

CUSocket::~CUSocket()
{
  this->closeSocket();
}

//Send the data with the size specified
void		CUSocket::send(const void *data, size_t size)
{
  if (this->_sock == -1)
    throw Exception("This socket type isn't initialized.");
  if (::sendto(this->_sock, data, size, 0, (SOCKADDR *)&this->_info, sizeof(this->_info)) == -1)
    throw Exception("send has failed.");
}

//Close the socket
void		CUSocket::closeSocket()
{
  if ((this->_sock != -1) && (close(this->_sock) != 0))
    throw Exception("closesocket of sock has failed.");
  this->_sock = -1;
}

//Set the socket attribute
void		CUSocket::setSocket(int sock)
{
  this->_sock = sock;
}

//Accessor method for the socket attribute
int		CUSocket::getSocket() const
{
  return (this->_sock);
}

//Accessor method for the info attribute
const struct sockaddr_in	&CUSocket::getInfo()
{
  return (this->_info);
}

//Check if the address specified is valid
bool		CUSocket::isValidIPAddress(const char *ipAddress) const
{
  struct sockaddr_in		tmp;
  bool				ret = true;
  
  if (::inet_pton(AF_INET, ipAddress, &tmp.sin_addr) != 1)
    {
      throw Exception("Address is invalid.");
      ret = false;
    }
  return (ret);
}

//Mutator method for the port within the socket
void			CUSocket::setPort(int port)
{
  this->_port = port;
  this->_info.sin_port = ::htons(port);
}

//Mutator method for the address within the socket
void			CUSocket::setAddress(const std::string &address)
{
  this->_info.sin_addr.s_addr = ::inet_addr(address.c_str());
}

//Accessor method for the address of the socket
uint32_t		CUSocket::getDestAddress() const
{
  return (this->_info.sin_addr.s_addr);
}

//Convert the address specified for using with the socket
uint32_t		CUSocket::convertAddress(const char *address) const
{
  return (::inet_addr(address));
}

//Accessor method for the port of the socket
int			CUSocket::getDestPort() const
{
  return (this->_port);
}

//Receive data
void			*CUSocket::receive(int *sizeReceiv)
{
  char			*data = new char[PACKET_SIZE_RECV];
  socklen_t		fromlen;
  
  if (this->_sock == -1)
    throw Exception("This socket type isn't initialized.");
  if ((*sizeReceiv = ::recvfrom(this->_sock, data, PACKET_SIZE_RECV, 0, (SOCKADDR *)&this->_info, &fromlen)) == -1)
    throw Exception("recvfrom has failed.");
  if (*sizeReceiv <= 0)
    {
      delete data;
      return (NULL);
    }
  return (data);
}

//Wait the read
int                           CUSocket::select(long int *usecTimeout)
{
  int				ret;
  
  this->_timeout.tv_usec = 1000;
  ret = ::select(this->_sock + 1, &this->_fds, NULL, NULL, &this->_timeout);
  *usecTimeout += (1000 - this->_timeout.tv_usec);
  return (ret);
}

//Wait the read
int                           CUSocket::select(long int usecTimeout)
{
  if (usecTimeout >= 0)
    this->_timeout.tv_usec = usecTimeout;
  else
    this->_timeout.tv_usec = TIMEOUT_PACKET;
  return (::select(this->_sock + 1, &this->_fds, NULL, NULL, &this->_timeout));
}

//Reset sockets, with input fd or not
void                          CUSocket::resetSockets(bool input)
{
  FD_ZERO(&this->_fds);
  if (input)
    FD_SET(0, &this->_fds);
  FD_SET(this->_sock, &this->_fds);
}
    
//Check if the input fd is readable
bool                          CUSocket::inputFdReadable() const
{
  return (FD_ISSET(0, &this->_fds));
}

//Check if the socket is readable
bool                          CUSocket::socketReadable() const
{
  return (FD_ISSET(this->_sock, &this->_fds));
}
