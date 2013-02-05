#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitive.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/TriggerPrimitiveFwd.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/InternalTrack.h"
#include "L1Trigger/L1IntegratedMuonTrigger/interface/InternalTrackFwd.h"
#include "DataFormats/Common/interface/Wrapper.h"
#include "DataFormats/Common/interface/RefToBase.h"

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
    std::vector<TriggerPrimitiveRef> rvtripPrim;
    
    TriggerPrimitivePtr ptrigPrim;   

    // internal tracks
    InternalTrack tk;
    edm::Wrapper<InternalTrack> wtk;

    InternalTrackCollection ctk;
    edm::Wrapper<InternalTrackCollection> wctk;

    InternalTrackRef rtk;
    InternalTrackPtr ptk;
  };
}
