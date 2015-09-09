
#pragma once

/*
  It's a class which encapsulate the TCP header with a custom header for a UNIX platform.
*/

#include "ITCPHeader.hh"
#include <netinet/tcp.h>

class   CUTCPHeader : public ITCPHeader
{
protected:
  struct TCPHeaderCustom
  {
    uint32_t	addressSource;
    uint32_t	addressDest;
    uint8_t	placeHolder;
    uint8_t	protocol;
    uint16_t	tcpLength;
  };
  void				*_data;
  struct tcphdr			*_sysHeader;
  struct TCPHeaderCustom	*_customHeader;
  
public:
  CUTCPHeader();
  virtual ~CUTCPHeader();
  //Mutators methods, set all fields for the TCP header 
  virtual void          set_portSource(uint16_t source);
  virtual void          set_portDest(uint16_t dest);
  virtual void          set_seq(uint32_t seq);
  virtual void          set_ack_seq(uint32_t ack_seq);
  virtual void          set_doff(uint16_t doff);
  virtual void          set_fin(uint16_t fin);
  virtual void          set_syn(uint16_t syn);
  virtual void          set_rst(uint16_t rst);
  virtual void          set_psh(uint16_t psh);
  virtual void          set_ack(uint16_t ack);
  virtual void          set_urg(uint16_t urg);
  virtual void          set_window(uint16_t window);
  virtual void          set_check(uint16_t check);
  virtual void          set_urg_ptr(uint16_t urg_ptr);
  virtual void          set_addressSource(uint32_t source);
  virtual void          set_addressDest(uint32_t dest);
  virtual void          set_placeHolder(uint8_t placeHolder);
  virtual void          set_protocol(uint8_t protocol);
  virtual void          set_tcpLength(uint16_t tcpLength);

  //Accessors methods for the data headers
  virtual void          *getData() const;
  virtual unsigned int	getSize() const;
  virtual uint16_t	get_portSource() const;
  virtual uint16_t	get_portDest() const;
  virtual uint32_t	get_seq() const;
  virtual uint32_t	get_ack_seq() const;
  virtual uint16_t	get_doff() const;
  virtual uint16_t	get_fin() const;
  virtual uint16_t	get_syn() const;
  virtual uint16_t	get_rst() const;
  virtual uint16_t	get_psh() const;
  virtual uint16_t	get_ack() const;
  virtual uint16_t	get_urg() const;
  virtual uint16_t	get_window() const;
  virtual uint16_t	get_check() const;
  virtual uint16_t	get_urg_ptr() const;
  virtual uint32_t	get_addressSource() const;
  virtual uint32_t	get_addressDest() const;
  virtual uint8_t	get_placeHolder() const;
  virtual uint8_t	get_protocol() const;
  virtual uint16_t	get_tcpLength() const;

  //Load the header from the data received
  virtual bool          load(const void *data);
};
