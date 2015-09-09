#include <list>
#include <iterator>
#include <sstream>
#include "IpChecksumDetection.hh"
#include "Log.hpp"

IpChecksumDetection::IpChecksumDetection()
  : ADetection()
{
  this->setPace(50);
  this->setName(std::string("IpChecksumDetection"));
  this->worksWithPair(true);
}

IpChecksumDetection::~IpChecksumDetection()
{
}

void IpChecksumDetection::testChecksums(boost::ptr_vector<State>& states)
{
  boost::ptr_vector<State>::iterator ite = states.begin();

  for (ite; ite != states.end(); ite++)
    {
      if (ite->getIpOptions().size() > 1)
	{
	  std::list<IpOption>::iterator o_ite = ite->getIpOptions().begin();
	  
	  for (o_ite; o_ite != ite->getIpOptions().end(); o_ite++)
	    {
	      if ((std::next(o_ite, 1) != ite->getIpOptions().end()) && o_ite->nb == 0)
		{
		  std::stringstream ss;
		  ss << " - IP Checksum Detection - Abnormal IP Option detected - from:" << ite->getFrom().to_string() 
		     << " to:" << ite->getTo().to_string() << " port:" << ite->getPort();
		  std::cout << ss.str() << std::endl;
		  Log::get_mutable_instance().write(ss.str());
		}
	    }
	}
    }
}

void IpChecksumDetection::work(boost::ptr_vector<State> states)
{
  this->testChecksums(states);
}
