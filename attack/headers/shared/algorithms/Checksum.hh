#pragma once

/*
  It's an algorithm Storage technique using the Time-To-Live filled (transmit 1 bit per packet).
*/

#include "IAlgorithm.hh"
#include "Packet.hh"

class	Checksum : public IAlgorithm 
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

  //Calc all values within IP Header by 16bits
  unsigned int	sumValues() const;
  //Initialize all fields of headers within the packet
  void		initPacket();
  //Determine the TTL field and send the packet depending of the value specified
  bool		computeAndSendValue(uint8_t valueOne, uint8_t valueTwo);
  //Check the source of the packet received
  bool		checkSource();
  
public:
  Checksum();
  virtual ~Checksum();
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
