#include "CUTCPHeader.hh"
#include "Exception.hh"
#include <unistd.h>

//Initialization of the ressources and properties of the UNIX TCP headers
CUTCPHeader::CUTCPHeader()
  : _data(NULL)
{
  if ((this->_data = malloc(this->getSize() + 1)) == NULL)
    throw Exception("TCPHeader initialization has failed.");
  this->_sysHeader = (struct tcphdr *) this->_data;
  this->_customHeader = (struct TCPHeaderCustom *) (((char *) this->_data) + sizeof(*this->_sysHeader));
}

CUTCPHeader::~CUTCPHeader()
{
  //Free the memory of the header allocated
  if (this->_data)
    free(this->_data);
}

//
//Mutators methods, set all fields for the TCP header
//

void          CUTCPHeader::set_portSource(uint16_t source)
{
  this->_sysHeader->source = source;
}

void          CUTCPHeader::set_portDest(uint16_t dest)
{
   this->_sysHeader->dest = dest;
}

void          CUTCPHeader::set_seq(uint32_t seq)
{
  this->_sysHeader->seq = seq;
}

void          CUTCPHeader::set_ack_seq(uint32_t ack_seq)
{
   this->_sysHeader->ack_seq = ack_seq;
}

void          CUTCPHeader::set_doff(uint16_t doff)
{
  this->_sysHeader->doff = doff;
}

void          CUTCPHeader::set_fin(uint16_t fin)
{
  this->_sysHeader->fin = fin;
}

void          CUTCPHeader::set_syn(uint16_t syn)
{
  this->_sysHeader->syn = syn;
}

void          CUTCPHeader::set_rst(uint16_t rst)
{
  this->_sysHeader->rst = rst;
}

void          CUTCPHeader::set_psh(uint16_t psh)
{
  this->_sysHeader->psh = psh;
}

void          CUTCPHeader::set_ack(uint16_t ack)
{
  this->_sysHeader->ack = ack;
}

void          CUTCPHeader::set_urg(uint16_t urg)
{
  this->_sysHeader->urg = urg;
}

void          CUTCPHeader::set_window(uint16_t window)
{
  this->_sysHeader->window = window;
}

void          CUTCPHeader::set_check(uint16_t check)
{
  this->_sysHeader->check = check;
}

void          CUTCPHeader::set_urg_ptr(uint16_t urg_ptr)
{
  this->_sysHeader->urg_ptr = urg_ptr;
}

void          CUTCPHeader::set_addressSource(uint32_t source)
{
  this->_customHeader->addressSource = source;
}

void          CUTCPHeader::set_addressDest(uint32_t dest)
{
  this->_customHeader->addressDest = dest;
}

void          CUTCPHeader::set_placeHolder(uint8_t placeHolder)
{
  this->_customHeader->placeHolder = placeHolder;
}

void          CUTCPHeader::set_protocol(uint8_t protocol)
{
  this->_customHeader->protocol = protocol;
}

void          CUTCPHeader::set_tcpLength(uint16_t tcpLength)
{
  this->_customHeader->tcpLength = tcpLength;
}

//
//Accessors methods for the data headers 
//

void          *CUTCPHeader::getData() const
{
  return (this->_data);
}

unsigned int	CUTCPHeader::getSize() const
{
  return (sizeof(*this->_sysHeader) + sizeof(*this->_customHeader));
}

uint16_t	CUTCPHeader::get_portSource() const
{
  return (this->_sysHeader->source);
}

uint16_t	CUTCPHeader::get_portDest() const
{
  return (this->_sysHeader->dest);
}

uint32_t	CUTCPHeader::get_seq() const
{
  return (this->_sysHeader->seq);
}

uint32_t	CUTCPHeader::get_ack_seq() const
{
  return (this->_sysHeader->ack_seq);
}

uint16_t	CUTCPHeader::get_doff() const
{
  return (this->_sysHeader->doff);
}

uint16_t	CUTCPHeader::get_fin() const
{
  return (this->_sysHeader->fin);
}

uint16_t	CUTCPHeader::get_syn() const
{
  return (this->_sysHeader->syn);
}

uint16_t	CUTCPHeader::get_rst() const
{
  return (this->_sysHeader->rst);
}

uint16_t	CUTCPHeader::get_psh() const
{
  return (this->_sysHeader->psh);
}

uint16_t	CUTCPHeader::get_ack() const
{
  return (this->_sysHeader->ack);
}

uint16_t	CUTCPHeader::get_urg() const
{
  return (this->_sysHeader->urg);
}

uint16_t	CUTCPHeader::get_window() const
{
  return (this->_sysHeader->window);
}

uint16_t	CUTCPHeader::get_check() const
{
  return (this->_sysHeader->check);
}

uint16_t	CUTCPHeader::get_urg_ptr() const
{
  return (this->_sysHeader->urg_ptr);
}

uint32_t	CUTCPHeader::get_addressSource() const
{
  return (this->_customHeader->addressSource);
}

uint32_t	CUTCPHeader::get_addressDest() const
{
  return (this->_customHeader->addressDest);
}

uint8_t		CUTCPHeader::get_placeHolder() const
{
  return (this->_customHeader->placeHolder);
}

uint8_t		CUTCPHeader::get_protocol() const
{
  return (this->_customHeader->protocol);
}

uint16_t	CUTCPHeader::get_tcpLength() const
{
  return (this->_customHeader->tcpLength);
}


//Load the header from the data received
bool		CUTCPHeader::load(const void *data)
{
  this->_sysHeader = (struct tcphdr *) data;
  this->_customHeader = (struct TCPHeaderCustom *) ((char *)data + sizeof(struct tcphdr));
  return (true);
}
