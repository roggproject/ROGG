#pragma once

/*
  It's an interface for TCP headers used within the project.
  It allow to create different TCP headers for different platforms, consequently, the cross-platform is easily implementable.
  All TCP headers within in the project must inherit of this interface.
*/

#include <stdint.h>

class	ITCPHeader
{
public:
  virtual ~ITCPHeader() {}
  //Mutators methods, set all fields for the TCP header
  virtual void		set_portSource(uint16_t source) = 0;
  virtual void		set_portDest(uint16_t dest) = 0;
  virtual void		set_seq(uint32_t seq) = 0;
  virtual void		set_ack_seq(uint32_t ack_seq) = 0;
  virtual void		set_doff(uint16_t doff) = 0;
  virtual void		set_fin(uint16_t fin) = 0;
  virtual void		set_syn(uint16_t syn) = 0;
  virtual void		set_rst(uint16_t rst) = 0;
  virtual void		set_psh(uint16_t psh) = 0;
  virtual void		set_ack(uint16_t ack) = 0;
  virtual void		set_urg(uint16_t urg) = 0;
  virtual void		set_window(uint16_t window) = 0;
  virtual void		set_check(uint16_t check) = 0;
  virtual void		set_urg_ptr(uint16_t urg_ptr) = 0;
  virtual void		set_addressSource(uint32_t source) = 0;
  virtual void		set_addressDest(uint32_t dest) = 0;
  virtual void		set_placeHolder(uint8_t placeHolder) = 0;
  virtual void		set_protocol(uint8_t protocol) = 0;
  virtual void		set_tcpLength(uint16_t tcpLength) = 0;
  //Accessors methods for the data header
  virtual void          *getData() const = 0;
  virtual unsigned int	getSize() const = 0;
  virtual uint16_t      get_portSource() const = 0;
  virtual uint16_t      get_portDest() const = 0;
  virtual uint32_t      get_seq() const = 0;
  virtual uint32_t      get_ack_seq() const = 0;
  virtual uint16_t      get_doff() const = 0;
  virtual uint16_t      get_fin() const = 0;
  virtual uint16_t      get_syn() const = 0;
  virtual uint16_t      get_rst() const = 0;
  virtual uint16_t      get_psh() const = 0;
  virtual uint16_t      get_ack() const = 0;
  virtual uint16_t      get_urg() const = 0;
  virtual uint16_t      get_window() const = 0;
  virtual uint16_t      get_check() const = 0;
  virtual uint16_t      get_urg_ptr() const = 0;
  virtual uint32_t      get_addressSource() const = 0;
  virtual uint32_t      get_addressDest() const = 0;
  virtual uint8_t       get_placeHolder() const = 0;
  virtual uint8_t       get_protocol() const = 0;
  virtual uint16_t      get_tcpLength() const = 0;
  
  //Load the header from the data received
  virtual bool          load(const void *data) = 0;
};
