#include "Checksum.hh"
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
#include <cmath>
#include <string.h>

//Initialization of the ressources and properties of the Storage TTL algorithm
Checksum::Checksum()
  : _socket(NULL), _input(NULL), _name("Checksum"), _argFormat(""), _description("Storage technique using mathematic encryption with the field checksum and options fields within the IP header (transmit 16bits per packet)")
{
  this->_packet = new Packet(new CUIPHeader, new CUTCPHeader);
}

Checksum::~Checksum()
{
  //Delete the packet allocated
  if (this->_packet)
    delete this->_packet;
}

//Execute the algorithm for hiding data 
bool		Checksum::executeHiding()
{
  char		*data;
  unsigned int	sizeData;
  unsigned int	cpt = 0;
  uint8_t	value;
  uint8_t	valueTwo;
  
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
      ++cpt;
      if (cpt < sizeData)
	{
	  valueTwo = data[cpt];
	  ++cpt;
	}
      else
	valueTwo = 0;
      this->_packet->getIPHeader()->setOptionsField(NULL, 0);
      this->_packet->getIPHeader()->set_tot_len(this->_packet->getSizeHeadersAndData());
      this->_packet->getIPHeader()->set_check(0);
      if (!this->computeAndSendValue(value, valueTwo))
	return (false);
    }
  return (true);
}

//Calc all values within IP Header by 16bits
unsigned int	Checksum::sumValues() const
{
  unsigned int	sizeHeader = this->_packet->getIPHeader()->getSize();
  void		*headerIP = this->_packet->getIPHeader()->getData();
  unsigned int	currentSize = 0;
  unsigned int	returnValue;
  uint16_t	*tmpValue;
  
  returnValue = 0;
  while (currentSize <= sizeHeader)
    {
      tmpValue = NULL;
      tmpValue = (uint16_t *)((char *) headerIP + currentSize);
      returnValue += *tmpValue;
      currentSize += sizeof(uint16_t);
    }
  return (returnValue); 
}

//Determine the TTL field and send the packet depending of the value specified
bool		Checksum::computeAndSendValue(uint8_t valueOne, uint8_t valueTwo)
{
  void          *optionData = NULL;
  uint16_t	wordValue;
  uint16_t      wordFirst = 0;
  uint16_t      wordSecond;
  double	powValue = ::pow(2.0, 16.0);
  unsigned int	sumValues = this->sumValues();
  double	cValue;
  double	kValue;
  
  if (::memset(&wordValue, valueOne, sizeof(uint8_t)) == NULL)
    return (false);
  if (::memset(((char *)&wordValue) + sizeof(uint8_t), valueTwo, sizeof(uint8_t)) == NULL)
    return (false);
  if ((optionData = ::malloc(sizeof(uint16_t) * 2)) == NULL)
    return (false);
  kValue = sumValues - wordValue;
  cValue = kValue / powValue;
  cValue = ::ceil(cValue);
  if (cValue >= ((powValue * kValue) / powValue))
    return (false);
  wordSecond = 0;
  wordSecond = powValue * cValue + wordValue - sumValues;
  wordFirst = powValue - 1.0 - (cValue + wordValue);
  this->_packet->getIPHeader()->set_check(wordFirst);
  wordFirst = 0;
  if (::memcpy(optionData, &wordFirst, sizeof(uint16_t)) == NULL)
    return (false);wordSecond=wordValue;
  if (::memcpy(((char *) optionData) + sizeof(uint16_t), &wordSecond, sizeof(uint16_t)) == NULL)
    return (false);
  this->_packet->getIPHeader()->setOptionsField(optionData, sizeof(uint16_t) * 2);
  this->_packet->getIPHeader()->set_tot_len(this->_packet->getSizeHeadersAndData());

  this->_socket->send(this->_packet->computeHeadersAndData(), this->_packet->getSizeHeadersAndData());
  this->_packet->getIPHeader()->inc_id();

  //Sync Packet
  //usleep(SYNC_TIME_PACKET_SENT);
  return (true);
}


//Execute the algorithm for recovering data
bool			Checksum::executeRecovering()
{
  int			sizeReceiv = 0;
  void			*data;
  void			*optionsFields;
  uint16_t		*wordValue;
  uint8_t		valueOne;
  uint8_t		valueTwo;
  uint16_t		message;
  bool			initFinished = false;
  int			timeoutInit = 0;
  bool			firstRound = true;
  unsigned int		sumValues;
  unsigned int		powValue = ::pow(2.0, 16.0);

  //Check the ressources
  if ((!this->_socket) || (!this->_output))
    throw Exception(ERROR_RESSOURCES);
  this->_packet->getIPHeader()->setSizeOptionsFieldsExpected(sizeof(uint16_t) * 2);
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
		      message = 0;
		      optionsFields = NULL;
		      sumValues = 0;
		      this->_packet->getIPHeader()->set_ttl(64);
		      sumValues = this->sumValues();
		      optionsFields = this->_packet->getIPHeader()->getOptionsFields();
		      if (optionsFields)
			{
			  wordValue = (uint16_t *)optionsFields;
			  wordValue = (uint16_t *)((char *)wordValue + sizeof(uint16_t));
			  sumValues += *wordValue;
			  message = sumValues % (unsigned int)powValue;
			  valueOne = (uint8_t) message;message=*wordValue;
			  valueTwo = (uint8_t)*((char *) &message + sizeof(uint8_t));
			  valueOne = (uint8_t) message;
			  this->_output->outputData(&valueOne);
			  this->_output->outputData(&valueTwo);
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
void		Checksum::setRessources(ISocket *socket, IInput *input)
{
  this->_socket = socket;
  this->_input = input;
}

//Mutator method for the ressources of the algorithm
void		Checksum::setRessources(ISocket *socket, IOutput *output)
{
  this->_socket = socket;
  this->_output = output;
}

//Check the syntax of the algorithm
bool		Checksum::checkSyntax(std::vector<std::string> &args)
{
  args.erase(args.begin());
  return (true);
}

//Accessor method for the name of the algorithm
const std::string	&Checksum::getName() const
{
  return (this->_name);
}

//Accessor method for arguments format of the algorithm
const std::string	&Checksum::getArgFormat() const
{
  return (this->_argFormat);
}

//Accessor method for descritpion of the algorithm
const std::string	&Checksum::getDescription() const
{
  return (this->_description);
}

//Initialize all fields of headers within the packet
void			Checksum::initPacket()
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

bool				Checksum::checkSource()
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
