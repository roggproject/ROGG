#pragma once

#include <vector>
#include "ADetection.hh"
#include "State.hh"

using namespace boost::asio;

class TimeTCPDetection : public ADetection
{
private:

  struct TimeTCPVariation
  {
    ip::address from;
    ip::address to;
    unsigned short port;
    std::vector<long int> timestamps;

    TimeTCPVariation(ip::address f_ip, ip::address t_ip, unsigned short port)
      : from(f_ip), to(t_ip), port(port)
    {
    }
  };

  std::vector<TimeTCPVariation> _variations;

  void findTimeTCPVariations(boost::ptr_vector<State>&);
  void registerTimeTCPVariations(TimeTCPVariation&, boost::ptr_vector<State>&);
  void analyseTimeTCPVariations(TimeTCPVariation&);

public:
  TimeTCPDetection();
  ~TimeTCPDetection();
  void work(boost::ptr_vector<State>);
};
