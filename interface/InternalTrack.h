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
#include "L1Trigger/L1IntegratedMuonTrigger/interface/RegionalTracksFwd.h"
#include "DataFormats/Common/interface/RefToBase.h"

class L1MuDTTrackCand;
namespace csc {
  class L1Track;
}

namespace L1ITMu{
  class InternalTrack : public L1MuRegionalCand {
    enum subsystem_offset{ kDT, kCSC, kRPCb, kRPCf };
  public:
    InternalTrack() {}
    ~InternalTrack() {}
    
    InternalTrack(const L1MuDTTrackCand&);
    InternalTrack(const csc::L1Track&);
    InternalTrack(const L1MuRegionalCand&); // for RPCs
    
    // return the persistent pointer to the parent of this internal track
    // may be null if this has no parent
    RegionalCandBaseRef parent() const { return _parent; }
    void setParent(const RegionalCandBaseRef& parent)
       { _parent = parent; }

    void addStub(const TriggerPrimitiveRef& stub) ;
         
    const TriggerPrimitiveStationMap& getStubs() const 
      { return _associatedStubs; }

    unsigned long mode()     const { return (_mode & 0xffff); }
    unsigned long dtMode()   const { return (_mode & 0x000f); }
    unsigned long cscMode()  const { return (_mode & 0x00f0)>>4; }
    unsigned long rpcbMode() const { return (_mode & 0x0f00)>>8; }
    unsigned long rpcfMode() const { return (_mode & 0xf000)>>12; }

  private:
    TriggerPrimitiveStationMap _associatedStubs;
    // this represents the mode considering all available muon detector types
    // 0 DT 4 bits | CSC 4 bits | RPCb 4 bits | RPC f 4 bits
    // using an unsigned long since we may want to add GEMs later
    unsigned long _mode; 
    //pointer to parent, if this was created from a CSC/DT/RPC track
    RegionalCandBaseRef _parent;
  };
}

#endif
