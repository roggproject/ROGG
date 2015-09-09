#include "TypeOfService.hh"
#include "ISocket.hh"
#include "IInput.hh"
#include "IOutput.hh"
#include "Defines.hh"
#include "Exception.hh"
#include "Packet.hh"
#include "CUIPHeader.hh"
#include "CUTCPHeader.hh"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <string.h>

//Initialization of the ressources and properties of the Storage TTL algorithm
TypeOfService::TypeOfService()
  : _socket(NULL), _input(NULL), _name("TypeOfService"), _argFormat(""), _description("Storage technique using the TypeOfService(tos) field with encryption (transmit 2bits per packet)")
{
  this->_packet = new Packet(new CUIPHeader, new CUTCPHeader);
}

TypeOfService::~TypeOfService()
{
  //Delete the packet allocated
  if (this->_packet)
    delete this->_packet;
}

//Execute the algorithm for hiding data 
bool		TypeOfService::executeHiding()
{
  char		*data;
  unsigned int	sizeData;
  unsigned int	cpt = 0;
  uint8_t	value;

  //Check the ressources
  if ((!this->_socket) || (!this->_input))
    throw Exception(ERROR_RESSOURCES);
  if (!this->_packet)
    throw Exception("Packet is not initialized.");
  
  //Initialize all fields of headers within the packet
  this->initPacket();
  
  //Get the data input for hiding it within the field TTL
  data = (char *)this->_input->getData();
  sizeData = this->_input->getSizeData();
  
  //Processing the data for sending
  while (cpt < sizeData)
    {
      value = data[cpt];
      if (!this->computeAndSendValue(value))
	return (false);
      ++cpt;
    }
  return (true);
}

//Determine the TTL field and send the packet depending of the value specified
bool		TypeOfService::computeAndSendValue(uint8_t value)
{
  uint8_t	tmpTOS;
  int		cpt = 0;
  
  //Check bit per bit
  while (cpt < 8)
    {
      tmpTOS = this->_packet->getIPHeader()->get_tos();
      tmpTOS = tmpTOS << 1;
      if (value & 128)
	tmpTOS += 1;
      value = value << 1;
      ++cpt;
      tmpTOS = tmpTOS << 1;
      if (value & 128)
	tmpTOS += 1;
      value = value << 1;
      ++cpt;
      this->_packet->getIPHeader()->set_tos(tmpTOS);
      this->_socket->send(this->_packet->computeHeadersAndData(), this->_packet->getSizeHeadersAndData());
      this->_packet->getIPHeader()->inc_id();
      this->_packet->getIPHeader()->set_tos(0);
      //Sync Packet
      usleep(SYNC_TIME_PACKET_SENT);
    }
  return (true);
}


//Execute the algorithm for recovering data
bool			TypeOfService::executeRecovering()
{
  int			sizeReceiv = 0;
  void			*data;
  bool			initFinished = false;
  int			timeoutInit = 0;
  uint8_t		oneByte = 0;
  int			cptByte = 0;
  uint8_t		valueTOS;
  bool			firstRound = true;
  
  //Check the ressources
  if ((!this->_socket) || (!this->_output))
    throw Exception(ERROR_RESSOURCES);
  while (42)
    {
      this->_socket->resetSockets(true);
      if (this->_socket->select() == -1)
	throw Exception("Select has failed.");
      if (initFinished)
	{
	  ++timeoutInit;
	  if (timeoutInit >= TIMEOUT_INIT)
	    {
	      this->_output->outputMemoryTimeout();
	      oneByte = 0;
	      cptByte = 0;
	      timeoutInit = 0;
	      initFinished = false;
	    }
	}
      if (this->_socket->inputFdReadable())
	return (true);
      if (this->_socket->socketReadable())
	{
	  if ((data = this->_socket->receive(&sizeReceiv)) == NULL)
	    return (false);
	  if (this->_packet->loadPacket(data, sizeReceiv))
	    {
	      //Check if the packet is a packet with data hiden
	      if (this->checkSource())
		{
		  //Init algorithm
		  if (!initFinished)
		    {
		      initFinished = true;
		      if (firstRound)
			{
			  std::cout << LISTEN_RECEPTBEGIN << std::endl;
			  firstRound = false;
			}
		      else
			std::cout << std::endl << LISTEN_RECEPTBEGIN << std::endl;
		    }
		  //Algorithm
		  if (initFinished)
		    {
		      valueTOS = this->_packet->getIPHeader()->get_tos();
		      oneByte = oneByte << 1;
		      if ((2 & valueTOS) != 0)
			oneByte += 1;
		      ++cptByte;
		      oneByte = oneByte << 1;
		      if ((1 & valueTOS) != 0)
			oneByte += 1;
		      ++cptByte;
		      if (cptByte >= 8)
			{
			  cptByte = 0;
			  this->_output->outputData(&oneByte);
			  oneByte = 0;
			}
		    }
		  timeoutInit = 0;
		}
	    }
	}
    }
  return (true);
}

//Mutator method for the ressources of the algorithm
void		TypeOfService::setRessources(ISocket *socket, IInput *input)
{
  this->_socket = socket;
  this->_input = input;
}

//Mutator method for the ressources of the algorithm
void		TypeOfService::setRessources(ISocket *socket, IOutput *output)
{
  this->_socket = socket;
  this->_output = output;
}

//Check the syntax of the algorithm
bool		TypeOfService::checkSyntax(std::vector<std::string> &args)
{
  args.erase(args.begin());
  return (true);
}

//Accessor method for the name of the algorithm
const std::string	&TypeOfService::getName() const
{
  return (this->_name);
}

//Accessor method for arguments format of the algorithm
const std::string	&TypeOfService::getArgFormat() const
{
  return (this->_argFormat);
}

//Accessor method for descritpion of the algorithm
const std::string	&TypeOfService::getDescription() const
{
  return (this->_description);
}

//Initialize all fields of headers within the packet
void			TypeOfService::initPacket()
{
  uint32_t		myAddress = this->_socket->convertAddress("42.42.42.42");

  //Initialization of the IP header
  this->_packet->getIPHeader()->set_ihl(5);
  this->_packet->getIPHeader()->set_version(4);
  this->_packet->getIPHeader()->set_tos(0);
  this->_packet->getIPHeader()->set_tot_len(this->_packet->getSizeHeadersAndData());
  this->_packet->getIPHeader()->set_id(1);
  this->_packet->getIPHeader()->set_frag_off(0);
  this->_packet->getIPHeader()->set_ttl(64);
  this->_packet->getIPHeader()->set_protocol(IPPROTO_TCP);
  this->_packet->getIPHeader()->set_check(0);
  this->_packet->getIPHeader()->set_saddr(myAddress);
  this->_packet->getIPHeader()->set_daddr(this->_socket->getDestAddress());
  this->_packet->getIPHeader()->set_check(this->_packet->checkSum((unsigned short *) this->_packet->computeHeadersAndData(), this->_packet->getIPHeader()->get_tot_len()));

  //Initialization of the TCP header
  this->_packet->getTCPHeader()->set_portSource(::htons(1337));
  this->_packet->getTCPHeader()->set_portDest(::htons(this->_socket->getDestPort()));
  this->_packet->getTCPHeader()->set_seq(0);
  this->_packet->getTCPHeader()->set_ack_seq(0);
  this->_packet->getTCPHeader()->set_doff(5);
  this->_packet->getTCPHeader()->set_fin(0);
  this->_packet->getTCPHeader()->set_syn(1);
  this->_packet->getTCPHeader()->set_rst(0);
  this->_packet->getTCPHeader()->set_psh(0);
  this->_packet->getTCPHeader()->set_ack(0);
  this->_packet->getTCPHeader()->set_urg(0);
  this->_packet->getTCPHeader()->set_window(::htons(6666));
  this->_packet->getTCPHeader()->set_check(0);
  this->_packet->getTCPHeader()->set_urg_ptr(0);
  this->_packet->getTCPHeader()->set_addressSource(myAddress);
  this->_packet->getTCPHeader()->set_addressDest(this->_socket->getDestAddress());
  this->_packet->getTCPHeader()->set_placeHolder(0);
  this->_packet->getTCPHeader()->set_protocol(IPPROTO_TCP);
  this->_packet->getTCPHeader()->set_tcpLength(::htons(this->_packet->getTCPHeader()->getSize()));
  this->_packet->getTCPHeader()->set_check(this->_packet->checkSum((unsigned short *) this->_packet->getTCPHeaderAndData(), this->_packet->getTCPHeader()->getSize() + this->_packet->getSizeData()));
}

bool				TypeOfService::checkSource()
{
  uint32_t		sourceAddress = this->_socket->convertAddress("42.42.42.42");

  //Check of the IP header
  if (this->_packet->getIPHeader()->get_ihl() != 5)
    return (false);
  if (this->_packet->getIPHeader()->get_version() != 4)
    return (false);
  if (this->_packet->getIPHeader()->get_protocol() != IPPROTO_TCP)
    return (false);
  if (this->_packet->getIPHeader()->get_saddr() != sourceAddress)
    return (false);
  //Check of the TCP header
  if (this->_packet->getTCPHeader()->get_portSource() != ::htons(1337))
    return (false);
  if (this->_packet->getTCPHeader()->get_portDest() != ::htons(this->_socket->getDestPort()))
    return (false);
  if (this->_packet->getTCPHeader()->get_window() != ::htons(6666))
    return (false);
  if (this->_packet->getTCPHeader()->get_addressSource() != sourceAddress)
    return (false);
  if (this->_packet->getTCPHeader()->get_protocol() != IPPROTO_TCP)
    return (false);
  if (this->_packet->getTCPHeader()->get_tcpLength() != ::htons(this->_packet->getTCPHeader()->getSize()))
    return (false);
  return (true);
}
