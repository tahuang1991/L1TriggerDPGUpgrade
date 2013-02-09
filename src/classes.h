#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitive.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitiveFwd.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/InternalTrack.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/InternalTrackFwd.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/CandidateTrack.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/CandidateTrackFwd.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/RefToBase.h"

#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"

namespace {
  using namespace L1ITMu;
  struct dictionary {
    // trigger primitive information
    TriggerPrimitive::RPCData rpcD;
    TriggerPrimitive::CSCData cscD;
    TriggerPrimitive::DTData  dtD;
    
    edm::Wrapper<TriggerPrimitive::RPCData> wrpcD;
    edm::Wrapper<TriggerPrimitive::CSCData> wcscD;
    edm::Wrapper<TriggerPrimitive::DTData>  wdtD;

    TriggerPrimitive trigPrim;
    edm::Wrapper<TriggerPrimitive> wtrigPrim;
    TriggerPrimitiveCollection vtrigPrim;
    edm::Wrapper<TriggerPrimitiveCollection> wvtrigPrim;

    TriggerPrimitiveRef rtrigPrim;
    TriggerPrimitiveList rvtrigPrim;
    TriggerPrimitiveStationMap smtrigPrim;
    
    TriggerPrimitivePtr ptrigPrim;   

    // internal tracks
    InternalTrack tk;
    edm::Wrapper<InternalTrack> wtk;

    InternalTrackCollection ctk;
    edm::Wrapper<InternalTrackCollection> wctk;

    InternalTrackRef rtk;
    InternalTrackPtr ptk;

    // Candidate tracks
    CandidateTrack cctk;
    edm::Wrapper<CandidateTrack> wcctk;

    CandidateTrackCollection ccctk;
    edm::Wrapper<CandidateTrackCollection> wccctk;

    CandidateTrackRef rcctk;
    CandidateTrackPtr pcctk;
    
    edm::RefToBase<L1MuRegionalCand> r2bregcand;
  };
}
