#ifndef __L1ITMU_CANDIDATETRACKFWD_H__
#define __L1ITMU_CANDIDATETRACKFWD_H__

#include <vector>
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/Ptr.h"
namespace L1ITMu {
  class CandidateTrack;

  typedef std::vector<CandidateTrack> CandidateTrackCollection;
  typedef edm::Ref<CandidateTrackCollection> CandidateTrackRef;
  typedef edm::Ptr<CandidateTrack> CandidateTrackPtr;
}

#endif
