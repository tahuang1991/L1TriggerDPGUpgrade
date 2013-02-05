#ifndef __L1ITMU_CANDIDATETRACK_H__
#define __L1ITMU_CANDIDATETRACK_H__
// 
// Class: L1ITMu::CandidateTrack
//
// Info: This class represents (one of the) final tracks output by
//       L1ITMu after sorting. It is just a L1MuGMTCand with a few
//       extra frills.
//
// Author: L. Gray (FNAL)
//

#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitiveFwd.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitive.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTCand.h"

namespace L1ITMu{
  class CandidateTrack : public L1MuGMTCand {
  public:
    CandidateTrack() {}
  private:
    std::vector<TriggerPrimitiveRef> _associatedStubs;
  };
}

#endif
