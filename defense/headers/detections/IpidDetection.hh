#pragma once

#include <vector>
#include "ADetection.hh"
#include "State.hh"

using namespace boost::asio;

class IpidDetection : public ADetection
{
private:

  struct IpidVariation
  {
    ip::address from;
    ip::address to;
    unsigned int abnormalInc;

    IpidVariation(ip::address f_ip, ip::address t_ip)
      : from(f_ip), to(t_ip), abnormalInc(0)
    {
    }
  };

  std::vector<IpidVariation> _variations;

  void findIpidVariations(boost::ptr_vector<State>&);
  void registerIpidVariations(IpidVariation&, boost::ptr_vector<State>&);
  void analyseIpidVariations();

public:
  IpidDetection();
  ~IpidDetection();
  void work(boost::ptr_vector<State>);
};
