#ifndef __L1ITMU_DTBUNCHCROSSINGCLEANER_H__
#define __L1ITMU_DTBUNCHCROSSINGCLEANER_H__
// 
// Class: L1ITMu::DTBunchCrossingCleaner
//
// Info: This class analyzes the output of a DT chamber and produces
//       a reduced set of trigger primitives combining theta and phi
//       trigger primitives that are likely to be associated.
//
// Author: L. Gray (FNAL)
//
#include <vector>
#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitiveFwd.h"
#include "FWCore/Utilities/interface/InputTag.h"

namespace edm{
  class ParameterSet;
}

namespace L1ITMu {
  
  class DTBunchCrossingCleaner {
  public:
    DTBunchCrossingCleaner(const edm::ParameterSet&);
    ~DTBunchCrossingCleaner() {}

    TriggerPrimitiveCollection clean(const TriggerPrimitiveCollection&) 
      const;
    
  private:	
    const int bx_window_size;
  };

}

#endif
