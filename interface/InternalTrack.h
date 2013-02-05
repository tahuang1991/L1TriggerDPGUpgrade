#ifndef __L1ITMU_INTERNALTRACK_H__
#define __L1ITMU_INTERNALTRACK_H__
// 
// Class: L1ITMu::InternalTrack
//
// Info: This class represents (one of the) internal tracks processed
//       by L1ITMu before sending off to the GT or whatever comes after.
//       As such, they are similar to the L1MuRegionalCands in terms of
//       their meaning.
//       To exploit that we also allow these tracks to be built out of
//       old style regional cands so we can perform studies using them
//       in the new framework
//
// Author: L. Gray (FNAL)
//

#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitiveFwd.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitive.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"

class L1MuDTTrackCand;
namespace csc {
  class L1Track;
}

namespace L1ITMu{
  class InternalTrack : public L1MuRegionalCand {
    typedef std::vector<TriggerPrimitiveRef> stublist;
  public:
    InternalTrack() {}
    
    InternalTrack(const L1MuDTTrackCand&);
    InternalTrack(const csc::L1Track&);
    InternalTrack(const L1MuRegionalCand&); // for RPCs
    
    void addStub(const TriggerPrimitiveRef& stub) 
         { _associatedStubs.push_back(stub); }
    const stublist& getStubs() const { return _associatedStubs; }

  private:
    stublist _associatedStubs;
  };
}

#endif
