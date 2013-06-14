#ifndef __L1TMUON_HCALCOLLECTOR_H__
#define __L1TMUON_HCALCOLLECTOR_H__
// 
// Class: L1TMuon::HCALCollector
//
// Info: Processes HCAL digis into L1TMuon trigger primitives. 
//       Positional information is not assigned here.
//
// Author: L. Gray (FNAL)
//
#include <vector>
#include "L1TriggerDPGUpgrade/L1TMuon/interface/SubsystemCollector.h"
#include "FWCore/Utilities/interface/InputTag.h"

namespace L1TMuon {
  
  class HCALCollector: public SubsystemCollector {
  public:
    HCALCollector(const edm::ParameterSet&);
    ~HCALCollector() {}

    virtual void extractPrimitives(const edm::Event&, const edm::EventSetup&, 
				   std::vector<TriggerPrimitive>&) const;
  private:    
  };
}

#endif
