#pragma once

/*
  It's a packet class, which implement TCP and IP header with the data.
  This class allow to create and modify a packet TCP/IP entirely.
*/

class	IIPHeader;
class	ITCPHeader;

class	Packet
{
protected:
  IIPHeader	*_ipHeader;
  ITCPHeader	*_tcpHeader;
  void		*_data;
  unsigned int	_sizeData;
  
public:
  //Constructor need to provite the both headers
  Packet(IIPHeader *ipHeader, ITCPHeader *tcpHeader);
  virtual ~Packet();
  //Compute and return the data of the packet for sending
  virtual void			*computeHeadersAndData() const;
  //Return the size of packet for sending 
  virtual unsigned int		getSizeHeadersAndData() const;
  //Mutator method to set the data within the packet
  virtual void			setData(void *data, unsigned int sizeData);
  //Accessor method for the IP header
  virtual IIPHeader		*getIPHeader();
  //Accessor metho for the TCP header
  virtual ITCPHeader		*getTCPHeader();
  //Return the value for the check field within the TCP/IP headers
  virtual unsigned short	checkSum(unsigned short *ptr, int nbytes);
  //Return the size of the data within the packet
  virtual unsigned int		getSizeData() const;
  //Accessor methode only on the TCP header and the data
  virtual void			*getTCPHeaderAndData() const;
  //Load the headers from the packet receiv
  virtual bool			loadPacket(const void *data, int sizeReceiv);
};
