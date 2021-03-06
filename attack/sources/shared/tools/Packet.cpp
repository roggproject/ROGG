#include "Packet.hh"
#include "IIPHeader.hh"
#include "ITCPHeader.hh"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

//Initialization of the both headers
Packet::Packet(IIPHeader *ipHeader, ITCPHeader *tcpHeader)
  : _ipHeader(ipHeader), _tcpHeader(tcpHeader)
{

}

Packet::~Packet()
{
  //Free the ressources headers allocated
  if (this->_ipHeader)
    delete this->_ipHeader;
  if (this->_tcpHeader)
    delete this->_tcpHeader;
}

//Compute and return the data of the packet for sending
void		*Packet::computeHeadersAndData() const
{
  void		*finalData = NULL;

  if ((finalData = malloc(this->getSizeHeadersAndData() + 1)) != NULL)
    {
      if (::memcpy(finalData, this->_ipHeader->getData(), this->_ipHeader->getSize()) == NULL)
	return (NULL);
      if (::memcpy(((char *) finalData) + this->_ipHeader->getSize(), this->_tcpHeader->getData(), this->_tcpHeader->getSize()) == NULL)
	return (NULL);
      if (::memcpy(((char *) finalData) + (this->_ipHeader->getSize() + this->_tcpHeader->getSize()), this->_data, this->_sizeData) == NULL)
	return (NULL);
    }
  return (finalData);
} 

//Return the size of packet for sending 
unsigned int	Packet::getSizeHeadersAndData() const
{
  return (this->_ipHeader->getSize() + this->_tcpHeader->getSize() + this->_sizeData);
}

//Mutator method to set the data within the packet
void		Packet::setData(void *data, unsigned int sizeData)
{
  this->_data = data;
  this->_sizeData = sizeData;
}

//Accessor method for the IP header
IIPHeader	*Packet::getIPHeader()
{
  return (this->_ipHeader);
}

//Accessor metho for the TCP header
ITCPHeader	*Packet::getTCPHeader()
{
  return (this->_tcpHeader);
}

//Return the value for the check field within the TCP/IP headers
unsigned short	Packet::checkSum(unsigned short *ptr, int nbytes)
{
  register long		sum;
  unsigned short	oddbyte;
  register short	answer;

  sum = 0;
  while (nbytes > 1)
    {
      sum += *ptr++;
      nbytes -= 2;
    }
  if (nbytes == 1)
    {
      oddbyte = 0;
      *((u_char*) &oddbyte) = *(u_char*) ptr;
      sum += oddbyte;
  }
  sum = (sum >> 16) + (sum & 0xffff);
  sum = sum + (sum >> 16);
  answer = (short) ~sum;
  return (answer);
}

//Return the size of the data within the packet
unsigned int	Packet::getSizeData() const
{
  return (this->_sizeData);
}

//Accessor methode only on the TCP header and the data
void            *Packet::getTCPHeaderAndData() const
{
  void          *finalData = NULL;

  if ((finalData = malloc(this->_sizeData + this->_tcpHeader->getSize() + 1)) != NULL)
    {
      if (::memcpy(finalData, this->_tcpHeader->getData(), this->_tcpHeader->getSize()) == NULL)
	return (NULL);
      if (::memcpy(((char *) finalData) + this->_tcpHeader->getSize(), this->_data, this->_sizeData) == NULL)
	return (NULL);
    }
  return (finalData);
}

//Load the headers from the packet receiv
bool			Packet::loadPacket(const void *data, int sizeReceiv)
{
  unsigned int		sizeHeaders = this->_ipHeader->getSize() + this->_tcpHeader->getSize();

  if (((unsigned int)sizeReceiv) < sizeHeaders)
    return (false);
  this->_data = (char *)data + sizeHeaders;
  if (!this->_ipHeader->load(data))
    return (false);
  if (!this->_tcpHeader->load((char *)data + this->_ipHeader->getSize()))
    return (false);
  return (true);
}
