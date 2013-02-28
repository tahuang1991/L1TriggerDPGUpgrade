#ifndef __L1ITMU_TRACKSEED_H__
#define __L1ITMU_TRACKSEED_H__
// 
// Class: L1ITMu::TrackSeed 
//
// Info: This track represents a DT(1 station), CSC (1 station), 
//       CSC+RPC (2 station), DT+RPC (2 station), or RPC+RPC (2 station)
//       based track seed, from which a full multi-station track can be
//       built.
//
// Author: L. Gray (FNAL)
//

#include <iostream>

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
  class TrackSeed {   
  public:
    enum seed_type{ kDTOnly, kCSCOnly, kCSCRPC, kDTRPC };
    enum subsystem_offset{ kDT, kRPCb, kCSC, kRPCf };
    TrackSeed():_endcap(0),_wheel(0),_sector(0),_type(5),_mode(0) {}
    ~TrackSeed() {}
    
    void setType(unsigned type) { _type = type; }
    unsigned type_idx() const;

    // return the persistent pointer to the parent of this internal track
    // may be null if this has no parent
    RegionalCandBaseRef parent() const { return _parent; }
    void setParent(const RegionalCandBaseRef& parent)
       { _parent = parent; }

    RPCL1LinkRef parentRPCLink() const { return _parentlink; }

    void addStub(const TriggerPrimitiveRef& stub) ;
         
    const TriggerPrimitiveStationMap& getStubs() const 
      { return _associatedStubs; }

    unsigned long mode()     const { return (_mode & 0xffff); }
    unsigned long dtMode()   const { return (_mode & 0xf<<4*kDT )>>4*kDT; }
    unsigned long cscMode()  const { return (_mode & 0xf<<4*kCSC)>>4*kCSC; }
    unsigned long rpcbMode() const { return (_mode & 0xf<<4*kRPCb)>>4*kRPCb; }
    unsigned long rpcfMode() const { return (_mode & 0xf<<4*kRPCf)>>4*kRPCf; }

    void print(std::ostream&) const;

  private:
    TriggerPrimitiveStationMap _associatedStubs;
    int _endcap, _wheel, _sector;
    unsigned _type;
    // this represents the mode considering all available muon detector types
    // 0 DT 4 bits | RPCb 4 bits | CSC 4 bits | RPC f 4 bits
    // using an unsigned long since we may want to add GEMs later
    // so cscMode() will return only the CSC part of the tracks contributing
    // to a CSC track (if this track was built from one)
    unsigned long _mode; 
    //pointer to parent, if this was created from a CSC/DT/RPC track
    RegionalCandBaseRef _parent;
    //pointer to RPC-L1 link, if an rpc track
    RPCL1LinkRef _parentlink;
  };
}

#endif
