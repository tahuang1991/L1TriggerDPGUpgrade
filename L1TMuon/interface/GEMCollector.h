#ifndef __L1TMUON_GEMCOLLECTOR_H__
#define __L1TMUON_GEMCOLLECTOR_H__
// 
// Class: L1TMuon::GEMCollector
//
// Info: Processes GEM digis into ITMu trigger primitives. 
//       Positional information is not assigned here.
//
//
#include <vector>
#include "L1TriggerDPGUpgrade/L1TMuon/interface/SubsystemCollector.h"
#include "FWCore/Utilities/interface/InputTag.h"

namespace L1TMuon {
  
  class GEMCollector: public SubsystemCollector {
  public:
    GEMCollector(const edm::ParameterSet&);
    ~GEMCollector() {}

    virtual void extractPrimitives(const edm::Event&, const edm::EventSetup&, 
				   std::vector<TriggerPrimitive>&) const;
  private:    
  };
}

#endif
