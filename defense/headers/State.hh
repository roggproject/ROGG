#pragma once

#include <boost/asio.hpp>
#include <list>

using namespace boost;
using namespace boost::asio;

struct IpOption
{
  unsigned char nb;
  unsigned char op;
  unsigned char copied;

  IpOption(unsigned char o_nb, unsigned char o_op, unsigned char o_copied)
  {
    nb = o_nb;
    op = o_op;
    copied = o_copied;
  }
};

class State
{
private:
  ip::address _from;
  ip::address _to;
  unsigned short _port;
  unsigned int _ttl;
  unsigned char _tos;
  unsigned short _id;
  long int _timestamp;
  std::list<IpOption> _ipOptions;

public:
  State();
  State(ip::address&, ip::address&, unsigned short);
  ~State();

  unsigned int id;

  const ip::address &getFrom() const;
  const ip::address &getTo() const;
  unsigned short getPort() const;
  unsigned int getTtl() const;
  unsigned char getTos() const;
  unsigned short getId() const;
  long int getTimestamp() const;
  std::list<IpOption> &getIpOptions();

  void setFrom(ip::address&);
  void setTo(ip::address&);
  void setPort(unsigned short);
  void setTtl(unsigned int);
  void setTos(unsigned char);
  void setId(unsigned short);
  void setTimestamp(long int);
};
