#ifndef __L1ITMU_CSCCOLLECTOR_H__
#define __L1ITMU_CSCCOLLECTOR_H__
// 
// Class: L1ITMu::CSCCollector
//
// Info: Processes CSC digis into ITMu trigger primitives. 
//       Positional information is not assigned here.
//
// Author: L. Gray (FNAL)
//
#include <vector>
#include "L1Trigger/L1IntegratedMuonTrigger/interface/SubsystemCollector.h"
#include "FWCore/Utilities/interface/InputTag.h"

namespace L1ITMu {
  
  class CSCCollector: public SubsystemCollector {
  public:
    CSCCollector(const edm::ParameterSet&);
    ~CSCCollector() {}

    virtual void extractPrimitives(const edm::Event&, const edm::EventSetup&, 
				   std::vector<TriggerPrimitive>&) const;
  private:    
  };
}

#endif
