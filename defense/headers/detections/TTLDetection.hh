#pragma once

#include <vector>
#include "ADetection.hh"
#include "State.hh"

using namespace boost::asio;

class TTLDetection : public ADetection
{
private:

  struct TTLVariation
  {
    ip::address from;
    ip::address to;
    unsigned int min;
    unsigned int max;
    double mean;
    double variance;
    
    TTLVariation(ip::address f_ip, ip::address t_ip)
      : from(f_ip), to(t_ip), min(255), max(0), mean(0), variance(0)
    {
    }
  };

  std::vector<TTLVariation> _variations;

  void testTTLValues(boost::ptr_vector<State>&);
  void findTTLVariations(boost::ptr_vector<State>&);
  void registerTTLVariations(TTLVariation&, boost::ptr_vector<State>&);
  void analyseTTLVariations();

public:
  TTLDetection();
  ~TTLDetection();
  void work(boost::ptr_vector<State>);
};
