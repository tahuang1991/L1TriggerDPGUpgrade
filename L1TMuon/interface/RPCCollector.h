#ifndef __L1ITMU_RPCCOLLECTOR_H__
#define __L1ITMU_RPCCOLLECTOR_H__
// 
// Class: L1ITMu::RPCCollector
//
// Info: Processes RPC digis into ITMu trigger primitives. 
//       Positional information is not assigned here.
//
// Author: L. Gray (FNAL)
//
#include <vector>
#include "L1Trigger/L1IntegratedMuonTrigger/interface/SubsystemCollector.h"
#include "FWCore/Utilities/interface/InputTag.h"

namespace L1ITMu {
  
  class RPCCollector: public SubsystemCollector {
  public:
    RPCCollector(const edm::ParameterSet&);
    ~RPCCollector() {}

    virtual void extractPrimitives(const edm::Event&, const edm::EventSetup&, 
				   std::vector<TriggerPrimitive>&) const;
  private:    
  };
}

#endif
