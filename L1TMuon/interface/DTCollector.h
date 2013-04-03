#ifndef __L1ITMU_DTCOLLECTOR_H__
#define __L1ITMU_DTCOLLECTOR_H__
// 
// Class: L1ITMu::DTCollector
//
// Info: Processes the DT digis into ITMu trigger primitives. 
//       Positional information is not assigned here.
//
// Author: L. Gray (FNAL)
//
#include <vector>
#include <memory>
#include "L1Trigger/L1IntegratedMuonTrigger/interface/SubsystemCollector.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/DTBunchCrossingCleaner.h"
#include "FWCore/Utilities/interface/InputTag.h"

class L1MuDTChambPhDigi;
class L1MuDTChambThDigi;

namespace L1ITMu {

  class DTCollector: public SubsystemCollector {
  public:
    DTCollector(const edm::ParameterSet&);
    ~DTCollector() {}

    virtual void extractPrimitives(const edm::Event&, const edm::EventSetup&, 
				   std::vector<TriggerPrimitive>&) const;
  private:
    TriggerPrimitive processDigis(const L1MuDTChambPhDigi&,
				  const int segment_number) const;
    TriggerPrimitive processDigis(const L1MuDTChambThDigi&,
				  const int bti_group) const;
    TriggerPrimitive processDigis(const L1MuDTChambPhDigi&,
				  const L1MuDTChambThDigi&,
				  const int bti_group) const;    
    int findBTIGroupForThetaDigi(const L1MuDTChambThDigi&,
				 const int position) const;    
    const int bx_min, bx_max;
    std::unique_ptr<DTBunchCrossingCleaner> _bxc;
  };
}

#endif
