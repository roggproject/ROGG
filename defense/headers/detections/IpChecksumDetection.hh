#pragma once

#include "ADetection.hh"
#include "State.hh"

using namespace boost::asio;

class IpChecksumDetection : public ADetection
{
private:
  
  void testChecksums(boost::ptr_vector<State>&);
  
public:
  IpChecksumDetection();
  ~IpChecksumDetection();
  void work(boost::ptr_vector<State>);
};
