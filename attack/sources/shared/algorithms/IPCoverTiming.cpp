#include "IPCoverTiming.hh"
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

//Initialization of the ressources and properties of the Storage TTL algorithm
IPCoverTiming::IPCoverTiming()
  : _socket(NULL), _input(NULL), _name("IPCoverTiming"), _argFormat(""), _description("Timing interval technique (transmit 1bit per packet)")
{
  this->_packet = new Packet(new CUIPHeader, new CUTCPHeader);
}

IPCoverTiming::~IPCoverTiming()
{
  //Delete the packet allocated
  if (this->_packet)
    delete this->_packet;
}

//Execute the algorithm for hiding data 
bool		IPCoverTiming::executeHiding()
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

  //Init
  this->_socket->send(this->_packet->computeHeadersAndData(), this->_packet->getSizeHeadersAndData());
  this->_packet->getIPHeader()->inc_id();

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
bool		IPCoverTiming::computeAndSendValue(uint8_t value)
{
  int		cpt = 0;

  //Check bit per bit
  while (cpt < 8)
    {
      //Check bit per bit with a mask 128
      if ((128 & value) != 0)
	::usleep(1000);
      //SHL instruction for the shift logical bit left
      value = value << 1;
      ++cpt;
      this->_socket->send(this->_packet->computeHeadersAndData(), this->_packet->getSizeHeadersAndData());
      this->_packet->getIPHeader()->inc_id();
    }
  return (true);
}


//Execute the algorithm for recovering data
bool			IPCoverTiming::executeRecovering()
{
  int			sizeReceiv = 0;
  void			*data;
  bool			initFinished = false;
  uint8_t		oneByte;
  int			cptByte = 0;
  bool			firstRound = true;
  long int		timeout = 0;
  
  //Check the ressources
  if ((!this->_socket) || (!this->_output))
    throw Exception(ERROR_RESSOURCES);
  while (42)
    {
      this->_socket->resetSockets(true);
      if (this->_socket->select(&timeout) == -1)
	throw Exception("Select has failed.");
      if (initFinished)
	{
	  if (timeout >= (TIMEOUT_INIT * TIMEOUT_PACKET))
	    {
	      this->_output->outputMemoryTimeout();
	      oneByte = 0;
	      cptByte = 0;
	      timeout = 0;
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
		      timeout = 0;
		    }
		  //Algorithm
		  else
		    {			
		      oneByte = oneByte << 1;
		      if (timeout > 950)
			oneByte += 1;
		      ++cptByte;
		      if (cptByte >= 8)
			{
			  cptByte = 0;
			  this->_output->outputData(&oneByte);
			  oneByte = 0;
			}
		    }
		  timeout = 0;
		}
	    }
	}
    }
  return (true);
}

//Mutator method for the ressources of the algorithm
void		IPCoverTiming::setRessources(ISocket *socket, IInput *input)
{
  this->_socket = socket;
  this->_input = input;
}

//Mutator method for the ressources of the algorithm
void		IPCoverTiming::setRessources(ISocket *socket, IOutput *output)
{
  this->_socket = socket;
  this->_output = output;
}

//Check the syntax of the algorithm
bool		IPCoverTiming::checkSyntax(std::vector<std::string> &args)
{
  args.erase(args.begin());
  return (true);
}

//Accessor method for the name of the algorithm
const std::string	&IPCoverTiming::getName() const
{
  return (this->_name);
}

//Accessor method for arguments format of the algorithm
const std::string	&IPCoverTiming::getArgFormat() const
{
  return (this->_argFormat);
}

//Accessor method for descritpion of the algorithm
const std::string	&IPCoverTiming::getDescription() const
{
  return (this->_description);
}

//Initialize all fields of headers within the packet
void			IPCoverTiming::initPacket()
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

bool				IPCoverTiming::checkSource()
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
