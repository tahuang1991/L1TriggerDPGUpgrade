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
#include "L1Trigger/L1IntegratedMuonTrigger/interface/InternalTrackFwd.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/InternalTrack.h"
#include "DataFormats/Common/interface/Ref.h"

namespace L1ITMu{
  
  class CandidateTrack : public L1MuGMTCand {
  public:
    CandidateTrack() {}
    ~CandidateTrack() {}

    CandidateTrack(const edm::Ref<InternalTrackCollection>&);
    
    edm::Ref<InternalTrackCollection> parent() const { return _parent; }
             
    const TriggerPrimitiveStationMap& getStubs() const 
      { return _parent->getStubs(); }

    unsigned long mode()     const { return _parent->mode(); }
    unsigned long dtMode()   const { return _parent->dtMode(); }
    unsigned long cscMode()  const { return _parent->cscMode(); }
    unsigned long rpcbMode() const { return _parent->rpcbMode(); }
    unsigned long rpcfMode() const { return _parent->rpcfMode(); }

  private:    
    edm::Ref<InternalTrackCollection> _parent;
  };
}

#endif
