#include <algorithm>
#include "Log.hpp"
#include "CommunicationPair.hh"

using namespace boost::asio;

CommunicationPair::CommunicationPair()
{
  this->_counter = 0;
}

CommunicationPair::CommunicationPair(const CommunicationPair &other)
{
  this->_first = other.getPair().first;
  this->_second = other.getPair().second;
  this->_states = other.getStates();
  this->_usedPorts = other.getPorts();
  this->_counter = other.getPacketCounter();
}

CommunicationPair::CommunicationPair(ip::address &first, ip::address &second)
{
  this->_counter = 0;
  this->setPair(first, second);
}

CommunicationPair::~CommunicationPair()
{
}

void CommunicationPair::operator=(const CommunicationPair &other)
{
  (void)other;
}

const boost::ptr_vector<State> &CommunicationPair::getStates() const
{
  return this->_states;
}

boost::ptr_vector<State> CommunicationPair::getSubStates(int nb)
{
  int start = this->_states.size() - nb;

  boost::ptr_vector<State> sub(this->_states.begin() + start, this->_states.end());
  return sub;
}

boost::ptr_vector<State> CommunicationPair::getSubStates(int nb, const ip::address &ip)
{
  if (this->_first != ip && this->_second != ip)
    Log::get_mutable_instance().write("Error when getting sub states with: " + ip.to_string());

  int start = this->_states.size() - nb;

  boost::ptr_vector<State> sub = this->getSubStates(nb);
  boost::ptr_vector<State>::iterator it = sub.begin();

  while (it != sub.end())
    {
      if (it->getFrom() != ip)
	it = sub.erase(it);
      else
	it++;
    }

  return sub;
}

bool CommunicationPair::addState(State *state)
{
  this->_mtx.lock();
  this->_counter++;


  if (std::find(this->_usedPorts.begin(), this->_usedPorts.end(), state->getPort()) == this->_usedPorts.end())
    {
      this->_usedPorts.push_back(state->getPort());
    }
  state->id = this->_counter;
  this->_states.push_back(state);
  this->_mtx.unlock();
  return true;
}

bool CommunicationPair::checkPair(const ip::address &first, const ip::address &second) const
{
  if ((first == this->_first && second == this->_second) || (first == this->_second && second == this->_first))
    return true;
  return false;
}

const std::pair<ip::address, ip::address> CommunicationPair::getPair() const
{
  return std::make_pair(this->_first, this->_second);
}

void CommunicationPair::setPair(ip::address &first, ip::address &second)
{
  this->_first = first;
  this->_second = second;
}

void CommunicationPair::setPair(std::pair<ip::address, ip::address> &pair)
{
  this->_first = pair.first;
  this->_second = pair.second;
}

const std::list<unsigned short> &CommunicationPair::getPorts() const
{
  return this->_usedPorts;
}

const unsigned int CommunicationPair::getPacketCounter() const
{
  return this->_states.size();
}
