#pragma once

/*
  It's a class which encapsulate the IP header for a UNIX platform.
*/

#include "IIPHeader.hh"
#include <netinet/ip.h>

class   CUIPHeader : public IIPHeader
{
protected:
  void			*_tmpData;
  struct iphdr		_sysHeader;
  void			*_optionsFields;
  unsigned int		_sizeOptionsFields;
  unsigned int		_sizeOptionsFieldsExcepted;
  
public:
  CUIPHeader();
  virtual ~CUIPHeader();
  //Mutators methods, set all fields for the IP header
  virtual void          set_ihl(uint8_t ihl);
  virtual void          set_version(uint8_t version);
  virtual void          set_tos(uint8_t tos);
  virtual void          set_tot_len(uint16_t tot_len);
  virtual void          set_id(uint16_t id);
  virtual void          inc_id();
  virtual void          set_frag_off(uint16_t frag_off);
  virtual void          set_ttl(uint8_t ttl);
  virtual void          set_protocol(uint8_t protocol);
  virtual void          set_check(uint16_t check);
  virtual void          set_saddr(uint32_t saddr);
  virtual void          set_daddr(uint32_t daddr);
  virtual void          setOptionsField(void *data, unsigned int size);
  virtual void          setSizeOptionsFieldsExpected(unsigned int size);
  
  //Accessors methods for the data header
  virtual void          *getData();
  virtual unsigned int	getSize() const;
  virtual void		*getOptionsFields();
  virtual uint8_t       get_ihl() const;
  virtual uint8_t       get_version() const;
  virtual uint8_t       get_tos() const;
  virtual uint16_t      get_tot_len() const;
  virtual uint16_t      get_id() const;
  virtual uint16_t      get_frag_off() const;
  virtual uint8_t       get_ttl() const;
  virtual uint8_t       get_protocol() const;
  virtual uint16_t      get_check() const;
  virtual uint32_t      get_saddr() const;
  virtual uint32_t      get_daddr() const;
  
  //Load the header from the data received
  virtual bool          load(const void *data);
};
