#include "L1Trigger/L1IntegratedMuonTrigger/interface/CandidateTrack.h"

using namespace L1ITMu;

CandidateTrack::
CandidateTrack(const edm::Ref<InternalTrackCollection>& parent):
  L1MuGMTCand(),
  _parent(parent){
}




