#include <boost/asio.hpp>
#include "AnalysisManager.hh"
#include "TTLDetection.hh"
#include "ToSDetection.hh"
#include "IpidDetection.hh"
#include "TimeTCPDetection.hh"
#include "IpChecksumDetection.hh"
#include "Log.hpp"

using namespace boost;
using namespace boost::asio;
using namespace Tins;

AnalysisManager::AnalysisManager()
{
  this->_detections.push_back(new TTLDetection());
  this->_detections.push_back(new ToSDetection());
  this->_detections.push_back(new IpidDetection());
  this->_detections.push_back(new TimeTCPDetection());
  this->_detections.push_back(new IpChecksumDetection());
}

AnalysisManager::~AnalysisManager()
{
}

bool AnalysisManager::addStateToPair(State *state)
{
  std::list<CommunicationPair>::iterator ite = this->_pairs.begin();
  for (ite; ite != this->_pairs.end(); ite++)
    {
      if (ite->checkPair(state->getFrom(), state->getTo()))
	{
	  ite->addState(state);
	  this->dispatchToDetection(*ite);
	  return true;
	}
    }
  return false;
}

void AnalysisManager::dispatchToDetection(CommunicationPair& pair)
{
  boost::ptr_vector<ADetection>::iterator ite = this->_detections.begin();

  for (ite; ite != this->_detections.end(); ite++)
    {
      if (pair.getPacketCounter() % ite->getPace() == 0)
	{
	  if (ite->worksWithPair())
	    ite->work(pair.getSubStates(ite->getPace()));
	  else
	    {
	      ite->work(pair.getSubStates(ite->getPace(), pair.getPair().first));
	      ite->work(pair.getSubStates(ite->getPace(), pair.getPair().second));
	    }
	}
    }
}

const unsigned int AnalysisManager::getPacketHandled() const
{
  return this->_packetHandled;
}

void AnalysisManager::handle(const Packet &packet)
{
  this->_packetHandled++;
  const PDU &pdu = *packet.pdu();
  const IP &ip = pdu.rfind_pdu<IP>();
  const TCP &tcp = pdu.rfind_pdu<TCP>();

  ip::address src = ip::address::from_string(ip.src_addr().to_string());
  ip::address dest = ip::address::from_string(ip.dst_addr().to_string());
  unsigned short port = (unsigned short)tcp.dport();

  State *state = new State(src, dest, port);
  state->setTtl((unsigned int)ip.ttl());
  state->setTos((unsigned char)ip.tos());
  state->setId((unsigned short)ip.id());
  state->setTimestamp((long int)packet.timestamp().microseconds());

  const std::list<IP::option> &ops = ip.options();
  std::list<IP::option>::const_iterator ite = ops.begin();
  for (ite; ite != ops.end(); ite++)
    {
      IP::option_identifier op_id = ite->option();
      state->getIpOptions().push_back(IpOption(op_id.number, op_id.op_class, op_id.copied));
    }
  
  if (!this->addStateToPair(state))
    {
      CommunicationPair pair(src, dest);
      pair.addState(state);
      this->_pairs.push_back(pair);
      this->dispatchToDetection(this->_pairs.back());
      Log::get_mutable_instance().write("new pair created: " + pair.getPair().first.to_string() + " - " + pair.getPair().second.to_string());
    }
}
