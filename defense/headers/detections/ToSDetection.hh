#pragma once

#include <vector>
#include "ADetection.hh"
#include "State.hh"

using namespace boost::asio;

class ToSDetection : public ADetection
{
private:

  struct ToSVariation
  {
    ip::address from;
    ip::address to;
    double variance;

    ToSVariation(ip::address f_ip, ip::address t_ip)
      : from(f_ip), to(t_ip), variance(0)
    {
    }
  };

  std::vector<ToSVariation> _variations;

  void testToSValues(boost::ptr_vector<State>&);
  void findToSVariations(boost::ptr_vector<State>&);
  void registerToSVariations(ToSVariation&, boost::ptr_vector<State>&);
  void analyseToSVariations();

public:
  ToSDetection();
  ~ToSDetection();
  void work(boost::ptr_vector<State>);
};
