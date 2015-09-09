#include "Timestamp.hh"
#include "ISocket.hh"
#include "IInput.hh"
#include "IOutput.hh"
#include "Defines.hh"
#include "Exception.hh"
#include "Packet.hh"
#include "CUIPHeader.hh"
#include "CUTCPHeader.hh"
#include <netinet/in.h>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

//Initialization of the ressources and properties of the Storage TTL algorithm
Timestamp::Timestamp()
  : _socket(NULL), _input(NULL), _name("Timestamp"), _argFormat(""), _description("Storage technique using the Timestamp field (transmit 1bit per packet)")
{
  this->_packet = new Packet(new CUIPHeader, new CUTCPHeader);
}

Timestamp::~Timestamp()
{
  //Delete the packet allocated
  if (this->_packet)
    delete this->_packet;
}

//Execute the algorithm for hiding data 
bool		Timestamp::executeHiding()
{
  char				*data;
  unsigned int			sizeData;
  unsigned int			cpt = 0;
  uint8_t			value;
  struct ip_options		*options;

  //Check the ressources
  if ((!this->_socket) || (!this->_input))
    throw Exception(ERROR_RESSOURCES);
  if (!this->_packet)
    throw Exception("Packet is not initialized.");
  if ((options = (struct ip_options *)::malloc(sizeof(struct ip_options))) == NULL)
    throw Exception("ip_options is not initialized.");
  //Initialize all fields of headers within the packet
  this->initPacket();

  //Init random
  ::srand(time(NULL));
  
  //Get the data input for hiding it within the field TTL
  data = (char *)this->_input->getData();
  sizeData = this->_input->getSizeData();

  this->_packet->getIPHeader()->setOptionsField(options, sizeof(struct ip_options));
  //Processing the data for sending
  while (cpt < sizeData)
    {
      value = data[cpt];
      if (!this->computeAndSendValue(value, options))
	return (false);
      ++cpt;
    }
  return (true);
}

//Generate a number between 0-254
uint8_t		Timestamp::generateuint8_tnumber() const
{
  return ((uint8_t)(::rand() % 255));
}

//Determine the TTL field and send the packet depending of the value specified
bool		Timestamp::computeAndSendValue(uint8_t value, struct ip_options *options)
{ 
  int		cpt = 0;

  //Check bit per bit
  while (cpt < 8)
    {
      options->ts = this->generateuint8_tnumber();
      //Check bit per bit with a mask 128
      if (((128 & value) == 0) && ((options->ts % 2) != 0))
	options->ts +=1;
      else if (((128 & value) != 0) && ((options->ts % 2) == 0))
	options->ts +=1;
      //SHL instruction for the shift logical bit left
      value = value << 1;
      ++cpt;
      this->_socket->send(this->_packet->computeHeadersAndData(), this->_packet->getSizeHeadersAndData());
      this->_packet->getIPHeader()->inc_id();
      //Sync Packet
      usleep(SYNC_TIME_PACKET_SENT);
    }
  return (true);
}


//Execute the algorithm for recovering data
bool			Timestamp::executeRecovering()
{
  int			sizeReceiv = 0;
  void			*data;
  bool			initFinished = false;
  int			timeoutInit = 0;
  uint8_t		oneByte;
  int			cptByte = 0;
  bool			firstRound = true;
  struct ip_options	*optionsFields;
  
  //Check the ressources
  if ((!this->_socket) || (!this->_output))
    throw Exception(ERROR_RESSOURCES);
  this->_packet->getIPHeader()->setSizeOptionsFieldsExpected(sizeof(struct ip_options));
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
		      optionsFields = NULL;
		      optionsFields = (struct ip_options *)this->_packet->getIPHeader()->getOptionsFields();
		      if (optionsFields)
			{
			  oneByte = oneByte << 1;
			  if ((optionsFields->ts % 2) != 0)
			    oneByte += 1;
			  ++cptByte;
			}
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
void		Timestamp::setRessources(ISocket *socket, IInput *input)
{
  this->_socket = socket;
  this->_input = input;
}

//Mutator method for the ressources of the algorithm
void		Timestamp::setRessources(ISocket *socket, IOutput *output)
{
  this->_socket = socket;
  this->_output = output;
}

//Check the syntax of the algorithm
bool		Timestamp::checkSyntax(std::vector<std::string> &args)
{
  args.erase(args.begin());
  return (true);
}

//Accessor method for the name of the algorithm
const std::string	&Timestamp::getName() const
{
  return (this->_name);
}

//Accessor method for arguments format of the algorithm
const std::string	&Timestamp::getArgFormat() const
{
  return (this->_argFormat);
}

//Accessor method for descritpion of the algorithm
const std::string	&Timestamp::getDescription() const
{
  return (this->_description);
}

//Initialize all fields of headers within the packet
void			Timestamp::initPacket()
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

bool				Timestamp::checkSource()
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
