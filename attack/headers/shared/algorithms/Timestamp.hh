
#pragma once

/*
  It's an algorithm Storage technique using the Time-To-Live filled (transmit 1 bit per packet).
*/

#include "IAlgorithm.hh"
#include "Packet.hh"

//From the ip.h system within the man of IP
struct ip_options {
  uint32_t         faddr;                          /* Saved first hop address */
  unsigned char optlen;
  unsigned char srr;
  unsigned char rr;
  unsigned char ts;
  unsigned char is_setbyuser:1,                 /* Set by setsockopt?                   */
    is_data:1,                      /* Options in __data, rather than skb   */
    is_strictroute:1,               /* Strict source route                  */
    srr_is_hit:1,                   /* Packet destination addr was our one  */
    is_changed:1,                   /* IP checksum more not valid           */
    rr_needaddr:1,                  /* Need to record addr of outgoing dev  */
    ts_needtime:1,                  /* Need to record timestamp             */
    ts_needaddr:1;                  /* Need to record addr of outgoing dev  */
  unsigned char router_alert;
  unsigned char __pad1;
  unsigned char __pad2;
  unsigned char __data[0];
};

class	Timestamp : public IAlgorithm 
{
private:
  //Ressources
  ISocket	*_socket;
  IInput	*_input;
  IOutput	*_output;
  std::string	_name;
  std::string	_argFormat;
  std::string	_description;
  //Packet TCP/IP
  Packet	*_packet;

  //Initialize all fields of headers within the packet
  void		initPacket();
  //Determine the TTL field and send the packet depending of the value specified
  bool		computeAndSendValue(uint8_t value, struct ip_options *options);
  //Check the source of the packet received
  bool		checkSource();
  //Generate a number between 0-254
  uint8_t	generateuint8_tnumber() const;
  
public:
  Timestamp ();
  virtual ~Timestamp();
  //Execute the algorithm for hiding data
  virtual bool                  executeHiding();
  //Execute the algorithm for recovering data
  virtual bool                  executeRecovering();
  //Mutator method for the ressources of the algorithm
  virtual void			setRessources(ISocket *socket, IInput *input);
    //Mutator method for the ressources of the algorithm
  virtual void			setRessources(ISocket *socket, IOutput *output);
  //Check the syntax of the algorithm
  virtual bool			checkSyntax(std::vector<std::string> &args);
  //Accessor method for the name of the algorithm
  virtual const std::string	&getName() const;
  //Accessor method for arguments format of the algorithm
  virtual const std::string     &getArgFormat() const;
  //Accessor method for descritpion of the algorithm
  virtual const std::string     &getDescription() const;
};
