#pragma once

#include <list>
#include <tins/tins.h>
#include <boost/ptr_container/ptr_vector.hpp>
#include "ADetection.hh"
#include "CommunicationPair.hh"

using namespace Tins;

class AnalysisManager
{
private:
  unsigned int _packetHandled;
  boost::ptr_vector<ADetection> _detections;
  std::list<CommunicationPair> _pairs;

  bool addStateToPair(State*);
  void dispatchToDetection(CommunicationPair&);

public:
  AnalysisManager();
  virtual ~AnalysisManager();

  const unsigned int getPacketHandled() const;

  void handle(const Packet &);

};
