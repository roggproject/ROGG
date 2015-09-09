#include "CUIPHeader.hh"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

CUIPHeader::CUIPHeader()
{
  this->_tmpData = NULL;
  this->_optionsFields = NULL;
  this->_sizeOptionsFields = 0;
  this->_sizeOptionsFieldsExcepted = 0;
}

CUIPHeader::~CUIPHeader()
{
  if (this->_tmpData)
    free(this->_tmpData);
  if (this->_optionsFields)
    free(this->_optionsFields);
}

//
//Mutators methods, set all fields for the IP header
//

void          CUIPHeader::set_ihl(uint8_t ihl)
{
  this->_sysHeader.ihl = ihl;
}

void          CUIPHeader::set_version(uint8_t version)
{
  this->_sysHeader.version = version;
}

void          CUIPHeader::set_tos(uint8_t tos)
{
  this->_sysHeader.tos = tos;
}

void          CUIPHeader::set_tot_len(uint16_t tot_len)
{
  this->_sysHeader.tot_len = tot_len;
}

void          CUIPHeader::set_id(uint16_t id)
{
  this->_sysHeader.id = id;
}

void          CUIPHeader::inc_id()
{
  ++this->_sysHeader.id;
}

void          CUIPHeader::set_frag_off(uint16_t frag_off)
{
  this->_sysHeader.frag_off = frag_off;
}

void          CUIPHeader::set_ttl(uint8_t ttl)
{
  this->_sysHeader.ttl = ttl;
}

void          CUIPHeader::set_protocol(uint8_t protocol)
{
  this->_sysHeader.protocol = protocol;
}

void          CUIPHeader::set_check(uint16_t check)
{
  this->_sysHeader.check = check;
}

void          CUIPHeader::set_saddr(uint32_t saddr)
{
  this->_sysHeader.saddr = saddr;
}

void          CUIPHeader::set_daddr(uint32_t daddr)
{
  this->_sysHeader.daddr = daddr;
}

void          CUIPHeader::setSizeOptionsFieldsExpected(unsigned int size)
{
  this->_sizeOptionsFieldsExcepted = size;
}

//
//Accessors methods for the data header
//

void          *CUIPHeader::getData()
{
  if (this->_tmpData)
    {
      free(this->_tmpData);
      this->_tmpData = NULL;
    }
  if (this->_sizeOptionsFields == 0)
    return (&this->_sysHeader);
  else
    {
      if ((this->_tmpData = malloc(sizeof(this->_sysHeader) + this->_sizeOptionsFields)) == NULL)
	return (&this->_sysHeader);
      if (::memcpy(this->_tmpData, &this->_sysHeader, sizeof(this->_sysHeader)) == NULL)
	return (&this->_sysHeader);
      if (::memcpy(((char *) this->_tmpData) + sizeof(this->_sysHeader), this->_optionsFields, this->_sizeOptionsFields) == NULL)
      	return (&this->_sysHeader);
      return (this->_tmpData);
    }
}

void		*CUIPHeader::getOptionsFields()
{
  return (this->_optionsFields);
}

unsigned int	CUIPHeader::getSize() const
{
  return (sizeof(this->_sysHeader) + this->_sizeOptionsFields);
}

uint8_t		CUIPHeader::get_ihl() const
{
  return (this->_sysHeader.ihl);
}

uint8_t		CUIPHeader::get_version() const
{
  return (this->_sysHeader.version);
}

uint8_t		CUIPHeader::get_tos() const
{
  return (this->_sysHeader.tos);
}

uint16_t	CUIPHeader::get_tot_len() const
{
  return (this->_sysHeader.tot_len);
}

uint16_t	CUIPHeader::get_id() const
{
  return (this->_sysHeader.id);
}

uint16_t	CUIPHeader::get_frag_off() const
{
  return (this->_sysHeader.frag_off);
}

uint8_t		CUIPHeader::get_ttl() const
{
  return (this->_sysHeader.ttl);
}

uint8_t		CUIPHeader::get_protocol() const
{
  return (this->_sysHeader.protocol);
}

uint16_t	CUIPHeader::get_check() const
{
  return (this->_sysHeader.check);
}

uint32_t	CUIPHeader::get_saddr() const
{
  return (this->_sysHeader.saddr);
}

uint32_t	CUIPHeader::get_daddr() const
{
  return (this->_sysHeader.daddr);
}

//Load the header from the data received
bool		CUIPHeader::load(const void *data)
{
  this->_sysHeader = *((struct iphdr *) data);
  if (this->_sizeOptionsFieldsExcepted > 0)
    {
      if ((this->_optionsFields = malloc(this->_sizeOptionsFieldsExcepted)) == NULL)
	return (false);
      if (::memcpy(this->_optionsFields, ((char *) data) + sizeof(this->_sysHeader), this->_sizeOptionsFieldsExcepted) == NULL)
	return (false);
      this->_sizeOptionsFields = this->_sizeOptionsFieldsExcepted;
    }
  return (true);
}

void		CUIPHeader::setOptionsField(void *data, unsigned int size)
{
  if (this->_optionsFields)
    {
      free(this->_optionsFields);
      this->_optionsFields = NULL;
    }  
  this->_optionsFields = data;
  this->_sizeOptionsFields = size;
}
