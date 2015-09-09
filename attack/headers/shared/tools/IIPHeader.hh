#pragma once

/*
  It's an interface for IP headers used within the project.
  It allow to create different IP headers for different platforms, consequently, the cross-platform is easily implementable.
  All IP headers within in the project must inherit of this interface.
*/

#include <stdint.h>

class	IIPHeader
{
public:
  virtual ~IIPHeader() {}
  //Mutators methods, set all fields for the IP header
  virtual void          set_ihl(uint8_t ihl) = 0;
  virtual void          set_version(uint8_t version) = 0;
  virtual void          set_tos(uint8_t tos) = 0;
  virtual void          set_tot_len(uint16_t tot_len) = 0;
  virtual void          set_id(uint16_t id) = 0;
  virtual void          inc_id() = 0;
  virtual void          set_frag_off(uint16_t frag_off) = 0;
  virtual void          set_ttl(uint8_t ttl) = 0;
  virtual void          set_protocol(uint8_t protocol) = 0;
  virtual void          set_check(uint16_t check) = 0;
  virtual void          set_saddr(uint32_t saddr) = 0;
  virtual void          set_daddr(uint32_t daddr) = 0;
  virtual void		setOptionsField(void *data, unsigned int size) = 0;
  virtual void		setSizeOptionsFieldsExpected(unsigned int size) = 0;
  
  //Accessors methods for the data header
  virtual void		*getData() = 0;
  virtual void          *getOptionsFields() = 0;
  virtual unsigned int	getSize() const = 0;
  virtual uint8_t	get_ihl() const = 0;
  virtual uint8_t	get_version() const = 0;
  virtual uint8_t	get_tos() const = 0;
  virtual uint16_t	get_tot_len() const = 0;
  virtual uint16_t	get_id() const = 0;
  virtual uint16_t	get_frag_off() const = 0;
  virtual uint8_t	get_ttl() const = 0;
  virtual uint8_t	get_protocol() const = 0;
  virtual uint16_t	get_check() const = 0;
  virtual uint32_t	get_saddr() const = 0;
  virtual uint32_t	get_daddr() const = 0;

  //Load the header from the data received
  virtual bool		load(const void *data) = 0;
};
