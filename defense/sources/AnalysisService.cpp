#include "AnalysisService.hh"
#include "Log.hpp"
#include "Defines.hh"

using namespace Tins;

AnalysisService::AnalysisService()
  : _filter(), _analysisManager(), _net(""), _init(false)
{
}

AnalysisService::AnalysisService(const std::string &net)
  : _filter(), _analysisManager(), _net(net), _init(false)
{
}

AnalysisService::~AnalysisService()
{
}

const std::string &AnalysisService::getNetInterface() const
{
  return this->_net;
}

void AnalysisService::setNetInterface(const std::string &net)
{
  this->_net = net;
}

void AnalysisService::setFilter(const std::string &mask)
{
  this->_filter.set(mask);
}

void AnalysisService::setFilter(const std::string &min, const std::string &max)
{
  this->_filter.set(min, max);
}

bool AnalysisService::packetSniffedCallback(Packet &packet)
{
  try
    {
      const IP &ip = packet.pdu()->rfind_pdu<IP>();

      if (this->_filter.match(ip.src_addr().to_string()) && this->_filter.match(ip.dst_addr().to_string()))
	{
	  this->_analysisManager.handle(packet);
	}
      return true;
    }
  catch (Tins::pdu_not_found e)
    {
      return true;
    }
}

void AnalysisService::init(int ac, char **av)
{
  std::string net;

  if (ac == 2)
    {
      net = std::string(av[1]);
      this->setNetInterface(net);
      Log::get_mutable_instance().setFile("./log.txt");
      this->_init = true;
    }
}

void AnalysisService::start()
{
  if (this->_init)
    {
      Sniffer sniffer(this->_net);
      while (Packet packet = sniffer.next_packet())
	{
	  this->packetSniffedCallback(packet);
	}
    }
  else
    Log::get_mutable_instance().write(INIT_ERROR);
}
