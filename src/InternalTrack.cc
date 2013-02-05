#include "L1Trigger/L1IntegratedMuonTrigger/interface/InternalTrack.h"

#include "DataFormats/L1DTTrackFinder/interface/L1MuDTTrackCand.h"
#include "DataFormats/L1CSCTrackFinder/interface/L1Track.h"

using namespace L1ITMu;

InternalTrack::InternalTrack(const L1MuDTTrackCand& dttrack):
  L1MuRegionalCand(dttrack) {
}

InternalTrack::InternalTrack(const csc::L1Track& csctrack):
  L1MuRegionalCand(csctrack) {
}

InternalTrack::InternalTrack(const L1MuRegionalCand& rpctrack):
  L1MuRegionalCand(rpctrack) {
}
