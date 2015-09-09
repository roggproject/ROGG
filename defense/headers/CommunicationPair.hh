#pragma once

#include <list>
#include <utility>
#include <boost/asio.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/thread/mutex.hpp>
#include "State.hh"

using namespace boost::asio;

class CommunicationPair
{
private:  
  void operator=(const CommunicationPair&);

  boost::mutex _mtx;
  boost::ptr_vector<State> _states;
  std::list<unsigned short> _usedPorts;
  ip::address _first;
  ip::address _second;

  unsigned int _counter;

public:
  CommunicationPair();
  CommunicationPair(const CommunicationPair&);
  CommunicationPair(ip::address&, ip::address&);
  
  ~CommunicationPair();

  const boost::ptr_vector<State> &getStates() const;
  boost::ptr_vector<State> getSubStates(int);
  boost::ptr_vector<State> getSubStates(int, const ip::address&);
  bool addState(State*);

  bool checkPair(const ip::address&, const ip::address&) const;
  const std::pair<ip::address, ip::address> getPair() const;
  void setPair(ip::address&, ip::address&);
  void setPair(std::pair<ip::address, ip::address>&);
  
  const std::list<unsigned short> &getPorts() const;
  const unsigned int getPacketCounter() const;
};
