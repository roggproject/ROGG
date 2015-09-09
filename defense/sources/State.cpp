#include "State.hh"

using namespace boost;
using namespace boost::asio;

State::State()
{
}

State::State(ip::address &from, ip::address &to, unsigned short port)
{
  this->_from = from;
  this->_to = to;
  this->_port = port;
}

State::~State()
{
}

const ip::address &State::getFrom() const
{
  return this->_from;
}

const ip::address &State::getTo() const
{
  return this->_to;
}

unsigned short State::getPort() const
{
  return this->_port;
}

unsigned int State::getTtl() const
{
  return this->_ttl;
}

unsigned char State::getTos() const
{
  return this->_tos;
}

unsigned short State::getId() const
{
  return this->_id;
}

long int State::getTimestamp() const
{
  return this->_timestamp;
}

std::list<IpOption> &State::getIpOptions()
{
  return this->_ipOptions;
}

void State::setFrom(ip::address &from)
{
  this->_from = from;
}

void State::setTo(ip::address &to)
{
  this->_to = to;
}

void State::setPort(unsigned short port)
{
  this->_port = port;
}

void State::setTtl(unsigned int ttl)
{
  this->_ttl = ttl;
}

void State::setTos(unsigned char tos)
{
  this->_tos = tos;
}

void State::setId(unsigned short id)
{
  this->_id = id;
}

void State::setTimestamp(long int timestamp)
{
  this->_timestamp = timestamp;
}
